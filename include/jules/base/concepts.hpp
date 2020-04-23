// Copyright (c) 2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_CONCEPTS_H
/// \exclude
#define JULES_BASE_CONCEPTS_H

#include <jules/core/concepts.hpp>

namespace jules
{

constexpr auto identity = []<typename T>(const T& value) -> T { return value; };
constexpr auto lhs = []<typename T>(const T& value, const T&) -> T { return value; };
constexpr auto rhs = []<typename T>(const T&, const T& value) -> T { return value; };

template <typename T> struct apply_traits
{
  using apply_t = void;
};

// clang-format off
template <typename T, typename Vector> concept apply_for =
  requires { typename Vector::value_type; } &&
  requires(const T& apply, const Vector& vec, const typename Vector::value_type& value) {
    { apply(vec, identity) };
    { apply(vec, vec, lhs) };
    { apply(value, vec, lhs) };
    { apply(vec, value, lhs) };
  };

template <typename T> concept common_vector = ranges::range<T> && apply_for<typename apply_traits<T>::apply_t, T>;
// clang-format on

} // namespace jules

#endif // JULES_BASE_CONCEPTS_H
