// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_NUMERIC_H
#define JULES_ARRAY_NUMERIC_H

#include <jules/array/builder.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/base/const_vector.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/type.hpp>

#include <iterator>
#include <type_traits>
#include <vector>

namespace jules
{

template <typename, std::size_t> class array;

static inline auto seq(const index_t start, const index_t stop, const distance_t step = 1) -> const_vector<index_t>
{
  DEBUG_ASSERT(step != 0, debug::default_module, debug::level::invalid_argument, "step cannot be zero");
  DEBUG_ASSERT(start <= stop || step < 0, debug::default_module, debug::level::invalid_argument, "stop value is unreachable");
  DEBUG_ASSERT(start >= stop || step > 0, debug::default_module, debug::level::invalid_argument, "stop value is unreachable");

  const auto size = static_cast<index_t>((static_cast<distance_t>(stop) - static_cast<distance_t>(start)) / step + 1);

  std::vector<index_t> indexes;
  indexes.reserve(size);

  for (auto i = start; indexes.size() < size; i += step)
    indexes.push_back(i);

  return {std::move(indexes)};
}

template <typename T, typename Array> auto to_vector(const common_array_base<Array>& source) -> array<T, 1u>
{
  // TODO: Optimize if A is a temporary.
  return {source.begin(), source.end()};
}

template <typename T, typename Rng, typename = meta::requires<range::SizedRange<Rng>, std::negation<CommonArray<Rng>>>>
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

template <typename Rng, typename R = range::range_value_t<Rng>> auto as_vector(const Rng& rng) -> array<R, 1u>
{
  return to_vector<R>(rng);
}

namespace detail
{

template <typename T, typename = void> struct cat_value_type {
  using type = std::decay_t<T>;
};

template <typename T> struct cat_value_type<T, meta::requires<range::SizedRange<std::decay_t<T>>>> {
  using type = range::value_type_t<T>;
};

template <typename T> using cat_value_type_t = typename cat_value_type<T>::type;

template <typename T>
constexpr auto cat_size(const T& rng) noexcept -> meta::requires_t<index_t, range::SizedRange<std::decay_t<T>>>
{
  return range::size(rng);
}

template <typename T> constexpr auto cat_size(const T&) noexcept -> meta::fallback_t<index_t, range::SizedRange<std::decay_t<T>>>
{
  return 1u;
}

template <typename T, typename Rng>
auto cat_push(array_builder<T, 1u>& builder, const Rng& rng) -> meta::requires<range::SizedRange<std::decay_t<Rng>>>
{
  range::copy(rng, range::back_inserter(builder));
}

template <typename T, typename U>
auto cat_push(array_builder<T, 1u>& builder, U&& value) -> meta::fallback<range::SizedRange<std::decay_t<U>>>
{
  builder.push_back(std::forward<U>(value));
}

} // namespace detail

template <typename... Args> auto cat(Args&&... args)
{
  using R = std::common_type_t<detail::cat_value_type_t<Args>...>;
  const auto size = (index_t{} + ... + detail::cat_size(args));

  auto builder = array_builder<R, 1u>{{size}};

  (detail::cat_push(builder, std::forward<Args>(args)), ...);

  return array<R, 1u>(std::move(builder));
}

} // namespace jules

#endif // JULES_ARRAY_NUMERIC_H
