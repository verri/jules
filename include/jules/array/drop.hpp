// Copyright (c) 2018 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DROP_H
#define JULES_ARRAY_DROP_H

#include <jules/core/type.hpp>

namespace jules
{

template <std::size_t D, typename T> auto drop_to(T&& value) -> decltype(auto)
{
  static_assert(D == 0);
  return static_cast<T>(value);
}

template <typename, std::size_t> class ref_array;
template <typename, typename> class mapped_ref_array;

template <std::size_t D, typename T, std::size_t N> auto drop_to(ref_array<T, N> source) -> decltype(auto);

template <typename T, std::size_t N> auto drop(ref_array<T, N> source) -> decltype(auto);

template <std::size_t D, typename U, typename Mapper> auto drop_to(mapped_ref_array<U, Mapper> source) -> decltype(auto);

template <typename U, typename Mapper> auto drop(mapped_ref_array<U, Mapper> source) -> decltype(auto);

} // namespace jules

#endif // JULES_ARRAY_DROP_H
