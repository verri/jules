// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_COMMON_H
/// \exclude
#define JULES_ARRAY_META_COMMON_H

#include <jules/core/concepts.hpp>
#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>

namespace jules
{

// clang-format off
template <std::size_t N, typename... Dims>
concept valid_extents_for = (sizeof...(Dims) == N) && (convertible_to<Dims, index_t> && ...);

template <typename T> concept common_array = requires(const T& t)
{
  ranges::range<T>;

  typename T::value_type;
  typename T::size_type;

  T::order > 0u;

  { T::order } -> convertible_to<typename T::size_type>;

  { t.size() } -> same_as<typename T::size_type>;
  { t.dimensions() } -> same_as<std::array<typename T::size_type, T::order>>;
};
// clang-format on

constexpr auto row_count(const common_array auto& a) noexcept { return a.dimensions()[0]; }

template <common_array Array> constexpr auto column_count(const Array& a) noexcept
{
  static_assert(Array::order >= 2);
  return a.dimensions()[1];
}

template <common_array Array> constexpr auto length(const Array& a) noexcept
{
  static_assert(Array::order == 1);
  return a.dimensions()[0];
}

} // namespace jules

#endif // JULES_ARRAY_META_COMMON_H
