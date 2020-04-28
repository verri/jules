// Copyright (c) 2018 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DROP_H
#define JULES_ARRAY_DROP_H

#include <jules/core/type.hpp>

namespace jules
{

template <std::size_t D, typename T> decltype(auto) drop_to(T&& value)
{
  static_assert(D == 0);
  return static_cast<T>(value);
}

template <typename, std::size_t> class ref_array;
template <typename, typename> class mapped_ref_array;

template <std::size_t D, typename T, std::size_t N> decltype(auto) drop_to(ref_array<T, N> source);

template <typename T, std::size_t N> decltype(auto) drop(ref_array<T, N> source);

template <std::size_t D, typename U, typename Mapper> decltype(auto) drop_to(mapped_ref_array<U, Mapper> source);

template <typename U, typename Mapper> decltype(auto) drop(mapped_ref_array<U, Mapper> source);

} // namespace jules

#endif // JULES_ARRAY_DROP_H
