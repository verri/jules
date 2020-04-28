// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_NUMERIC_H
#define JULES_ARRAY_NUMERIC_H

#include <jules/array/builder.hpp>
#include <jules/array/fwd.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/meta/reference.hpp>
#include <jules/array/slicing.hpp>
#include <jules/base/sequence.hpp>
#include <jules/core/concepts.hpp>
#include <jules/core/debug.hpp>
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
requires reference_array<RefArrayA>&& reference_array<RefArrayB> auto product_impl(const RefArrayA lhs, const RefArrayB& rhs)
  -> array<R, 2u>
{
  static_assert(RefArrayA::order == 2u && RefArrayB::order == 2u);

  DEBUG_ASSERT(lhs.size() > 0 && rhs.size() > 0, debug::default_module, debug::level::invalid_argument, "empty matrix");
  DEBUG_ASSERT(column_count(lhs) == row_count(rhs), debug::default_module, debug::level::invalid_argument, "invalid extents");

  const auto m = row_count(lhs);
  const auto p = column_count(rhs);

  auto builder = array_builder<R, 2u>{{{m, p}}};

  for (const auto j : indices(p))
    for (const auto i : indices(m))
      builder.push_back(sum(lhs[i][every] * drop(rhs[every][j])));

  return {std::move(builder)};
}
} // namespace detail

template <typename ArrayA, typename ArrayB,
          typename R = decltype(std::declval<typename ArrayA::value_type>() * std::declval<typename ArrayB::value_type>())>
requires common_array<ArrayA>&& common_array<ArrayB> auto product(const ArrayA& lhs, const ArrayB& rhs) -> array<R, 2u>
{
  return detail::product_impl<R>(eval(static_cast<const ArrayA&>(lhs)), eval(static_cast<const ArrayB&>(rhs)));
}

template <typename T> auto to_vector(const common_array auto& source) -> array<T, 1u> { return {source.begin(), source.end()}; }

template <typename T, same_as<T> U, std::size_t N> auto to_vector(array<U, N>&& source) noexcept -> array<T, 1u>
{
  return array_builder<T, 1>(std::move(source).release(), {{source.size()}});
}

template <typename T, ranges::range Rng> requires(!common_array<Rng>) auto to_vector(const Rng& rng) { return array<T, 1u>(rng); }

// If higher order, copy elements column-wise.
auto as_vector(const common_array auto& source) -> array<typename decltype(source)::value_type, 1u>
{
  return {source.begin(), source.end()};
}

template <typename T, std::size_t N> auto as_vector(array<T, N>&& source) noexcept -> array<T, 1u>
{
  return array_builder<T, 1>(std::move(source).release(), {{source.size()}});
}

template <ranges::range Rng, typename R = ranges::range_value_t<Rng>> auto as_vector(const Rng& rng) -> array<R, 1u>
{
  return to_vector<R>(rng);
}

namespace detail
{

template <typename T> struct cat_value_type
{
  using type = T;
};

template <ranges::range T> struct cat_value_type<T>
{
  using type = ranges::value_type_t<T>;
};

template <typename T> using cat_value_type_t = typename cat_value_type<T>::type;

template <typename T> constexpr auto cat_size(const T& value) noexcept -> index_t
{
  if constexpr (ranges::range<T>)
    return ranges::size(value);
  else
    return 1u;
}

template <typename T> auto cat_push(array_builder<T, 1u>& builder, T value) { builder.push_back(std::move(value)); }

template <typename T, ranges::range Rng> auto cat_push(array_builder<T, 1u>& builder, const Rng& rng) -> void
{
  ranges::copy(rng, ranges::back_inserter(builder));
}

template <typename T, typename U>
requires(!ranges::range<std::decay_t<U>>) auto cat_push(array_builder<T, 1u>& builder, U&& value) -> void
{
  builder.push_back(std::forward<U>(value));
}

} // namespace detail

template <typename... Args> auto cat(Args&&... args)
{
  using R = std::common_type_t<detail::cat_value_type_t<std::decay_t<Args>>...>;
  const auto size = (index_t{} + ... + detail::cat_size(args));

  auto builder = array_builder<R, 1u>{{{size}}};

  (detail::cat_push(builder, std::forward<Args>(args)), ...);

  return array<R, 1u>(std::move(builder));
}

} // namespace jules

#endif // JULES_ARRAY_NUMERIC_H
