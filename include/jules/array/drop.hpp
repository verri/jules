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
template <typename, typename> class strided_ref_array;

template <std::size_t D, typename T, std::size_t N> decltype(auto) drop_to(ref_array<T, N> source);

template <typename T, std::size_t N> decltype(auto) drop(ref_array<T, N> source);

template <std::size_t D, typename U, template <std::size_t> typename MapType, std::size_t M>
decltype(auto) drop_to(strided_ref_array<U, MapType<M>> source);

template <typename U, template <std::size_t> typename MapType, std::size_t M>
decltype(auto) drop(strided_ref_array<U, MapType<M>> source);

} // namespace jules

#endif // JULES_ARRAY_DROP_H
