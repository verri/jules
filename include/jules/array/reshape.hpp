// Copyright (c) 2019-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_RESHAPE_H
#define JULES_ARRAY_RESHAPE_H

#include <jules/core/type.hpp>

namespace jules
{

template <typename, std::size_t> class array;
template <typename, std::size_t> class ref_array;
template <std::size_t> class descriptor;

template <std::size_t D, typename T, std::size_t N>
auto reshape_to(array<T, N>&& source, std::array<index_t, D>) noexcept -> array<T, D>;

template <typename T, std::size_t N, typename... Dims>
auto reshape(array<T, N>&& source, Dims... dims) noexcept -> array<T, sizeof...(Dims)>
{
  return reshape_to<sizeof...(Dims)>(std::move(source), {{static_cast<index_t>(dims)...}});
}

template <std::size_t D, typename T, std::size_t N>
auto reshape_to(ref_array<T, N> source, std::array<index_t, D>) noexcept -> ref_array<T, D>;

template <typename T, std::size_t N, typename... Dims>
auto reshape(ref_array<T, N> source, Dims... dims) noexcept -> ref_array<T, sizeof...(Dims)>
{
  return reshape_to<sizeof...(Dims)>(std::move(source), {{static_cast<index_t>(dims)...}});
}

} // namespace jules

#endif // JULES_ARRAY_RESHAPE_H
