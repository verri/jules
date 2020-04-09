// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_NUMERIC_H
#define JULES_ARRAY_NUMERIC_H

#include <jules/array/builder.hpp>
#include <jules/array/fwd.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/meta/reference.hpp>
#include <jules/array/slicing.hpp>
#include <jules/base/const_vector.hpp>
#include <jules/base/sequence.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/type.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

namespace jules
{

namespace detail
{
template <typename R, typename RefArrayA, typename RefArrayB>
auto product_impl(const RefArrayA lhs, const RefArrayB& rhs)
  -> meta::requires_t<array<R, 2u>, ReferenceArray<RefArrayA>, ReferenceArray<RefArrayB>>
{
  static_assert(RefArrayA::order == 2u && RefArrayB::order == 2u);

  DEBUG_ASSERT(lhs.size() > 0 && rhs.size() > 0, debug::default_module, debug::level::invalid_argument, "empty matrix");
  DEBUG_ASSERT(lhs.column_count() == rhs.row_count(), debug::default_module, debug::level::invalid_argument, "invalid extents");

  const auto m = lhs.row_count();
  const auto p = rhs.column_count();

  auto builder = array_builder<R, 2u>{{{m, p}}};

  for (const auto j : indices(p))
    for (const auto i : indices(m))
      builder.push_back(sum(lhs[i][every] * drop(rhs[every][j])));

  return {std::move(builder)};
}
} // namespace detail

template <typename ArrayA, typename ArrayB,
          typename R = decltype(std::declval<typename ArrayA::value_type>() * std::declval<typename ArrayB::value_type>())>
auto product(const common_array_base<ArrayA>& lhs, const common_array_base<ArrayB>& rhs) -> array<R, 2u>
{
  return detail::product_impl<R>(eval(static_cast<const ArrayA&>(lhs)), eval(static_cast<const ArrayB&>(rhs)));
}

template <typename T, typename Array> auto to_vector(const common_array_base<Array>& source) -> array<T, 1u>
{
  // TODO: Optimize if A is a temporary.
  return {source.begin(), source.end()};
}

template <typename T, typename Rng,
          typename = meta::requires_<std::bool_constant<ranges::sized_range<Rng>>, std::negation<CommonArray<Rng>>>>
auto to_vector(const Rng& rng)
{
  return array<T, 1u>(rng);
}

// If higher order, copy elements column-wise.
template <typename Array> auto as_vector(const common_array_base<Array>& source) -> array<typename Array::value_type, 1u>
{
  // TODO: Optimize if A is a temporary.
  return {source.begin(), source.end()};
}

template <typename Rng, typename R = ranges::range_value_t<Rng>> auto as_vector(const Rng& rng) -> array<R, 1u>
{
  return to_vector<R>(rng);
}

namespace detail
{

template <typename T, typename = void> struct cat_value_type
{
  using type = std::decay_t<T>;
};

template <typename T> struct cat_value_type<T, meta::requires_concept<ranges::sized_range<std::decay_t<T>>>>
{
  using type = ranges::value_type_t<T>;
};

template <typename T> using cat_value_type_t = typename cat_value_type<T>::type;

template <typename T>
constexpr auto cat_size(const T& rng) noexcept
  -> meta::requires_t<index_t, std::bool_constant<ranges::sized_range<std::decay_t<T>>>>
{
  return ranges::size(rng);
}

template <typename T>
constexpr auto cat_size(const T&) noexcept -> meta::fallback_t<index_t, std::bool_constant<ranges::sized_range<std::decay_t<T>>>>
{
  return 1u;
}

template <typename T> auto cat_push(array_builder<T, 1u>& builder, T value) { builder.push_back(std::move(value)); }

template <typename T, typename Rng>
auto cat_push(array_builder<T, 1u>& builder, const Rng& rng) -> meta::requires_concept<ranges::sized_range<Rng>>
{
  ranges::copy(rng, ranges::back_inserter(builder));
}

template <typename T, typename U>
auto cat_push(array_builder<T, 1u>& builder, U&& value) -> meta::fallback_concept<ranges::sized_range<std::decay_t<U>>>
{
  builder.push_back(std::forward<U>(value));
}

} // namespace detail

template <typename... Args> auto cat(Args&&... args)
{
  using R = std::common_type_t<detail::cat_value_type_t<Args>...>;
  const auto size = (index_t{} + ... + detail::cat_size(args));

  auto builder = array_builder<R, 1u>{{{size}}};

  (detail::cat_push(builder, std::forward<Args>(args)), ...);

  return array<R, 1u>(std::move(builder));
}

} // namespace jules

#endif // JULES_ARRAY_NUMERIC_H
