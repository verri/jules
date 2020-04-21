// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_COMMON_H
/// \exclude
#define JULES_ARRAY_META_COMMON_H

#include <jules/core/meta.hpp>
#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>

namespace jules
{

template <std::size_t N, typename... Dims>
concept common_dimensions = (sizeof...(Dims) == N) && (convertible_to<Dims, index_t> && ...);

// clang-format off
template <typename T> concept common_array = requires(const T& t)
{
  ranges::range<T>;

  typename T::value_type;
  typename T::size_type;

  T::order > 0u;

  { T::order } -> same_as<const std::size_t>;

  { t.length() } -> same_as<typename T::size_type>;
  { t.row_count() } -> same_as<typename T::size_type>;
  { t.column_count() } -> same_as<typename T::size_type>;

  { t.size() } -> same_as<typename T::size_type>;
  { t.dimensions() } -> same_as<std::array<typename T::size_type, T::order>>;
  { eval(t) };
};
// clang-format on

} // namespace jules

#endif // JULES_ARRAY_META_COMMON_H
