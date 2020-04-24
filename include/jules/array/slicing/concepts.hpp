// Copyright (c) 2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_CONCEPTS_H
/// \exclude
#define JULES_ARRAY_SLICING_CONCEPTS_H

#include <jules/core/concepts.hpp>
#include <jules/core/type.hpp>

namespace jules
{
inline namespace slicing
{

template <typename T> struct slice_traits
{
  using absolute_type = void;
  using absolutize_type = void;
};

// clang-format off
template <typename T> concept valid_slice = requires(const T& s)
{
  !default_constructible<T>;

  copy_constructible<T>;
  move_constructible<T>;

  copyable<T>;
  movable<T>;

  std::is_trivially_destructible_v<T>;

  !same_as<void, typename slice_traits<T>::absolute_type>;
  !same_as<void, typename slice_traits<T>::absolutize_type>;

  { typename slice_traits<T>::absolutize_type{}(s, index_t{}) } noexcept -> same_as<typename slice_traits<T>::absolute_type>;
};
// clang-format on

template <valid_slice Slice>
constexpr auto absolutize(Slice slice, index_t n) noexcept -> typename slice_traits<Slice>::absolute_type
{
  constexpr typename slice_traits<Slice>::absolutize_type absolutize;
  return absolutize(slice, n);
}

} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_CONCEPTS_H
