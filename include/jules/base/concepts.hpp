// Copyright (c) 2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_CONCEPTS_H
/// \exclude
#define JULES_BASE_CONCEPTS_H

#include <jules/core/concepts.hpp>
#include <jules/core/type.hpp>
#include <type_traits>
#include <utility>

namespace jules
{

template <typename T> struct apply_traits
{
  using apply_type = void;
};

// clang-format off
template <typename Op, typename T> concept applies_to =
  (!same_as<void, typename apply_traits<T>::apply_type>) &&
  requires(T value, Op op, typename apply_traits<T>::apply_type apply) {
    std::is_trivial_v<typename apply_traits<T>::apply_type>;
    { typename apply_traits<T>::apply_type{} } noexcept;
    { apply(std::forward<T>(value), std::move(op)) };
  };

template <typename Op, typename T, typename U> concept applies_to_by =
  (!same_as<void, typename apply_traits<T>::apply_type>) &&
  requires(T lhs, U rhs, Op op, typename apply_traits<T>::apply_type apply) {
    std::is_trivial_v<typename apply_traits<T>::apply_type>;
    { typename apply_traits<T>::apply_type{} } noexcept;
    { apply(std::forward<T>(lhs), std::forward<U>(rhs), std::move(op)) };
  };

template <typename Op, typename T> concept applies_in_place_to =
  (!same_as<void, typename apply_traits<T>::apply_type>) &&
  requires(T value, Op op, typename apply_traits<T>::apply_type apply) {
    std::is_trivial_v<typename apply_traits<T>::apply_type>;
    { typename apply_traits<T>::apply_type{} } noexcept;
    { apply(in_place, std::forward<T>(value), std::move(op)) };
  };

template <typename Op, typename T, typename U> concept applies_in_place_to_by =
  (!same_as<void, typename apply_traits<T>::apply_type>) &&
  requires(T lhs, U rhs, Op op, typename apply_traits<T>::apply_type apply) {
    std::is_trivial_v<typename apply_traits<T>::apply_type>;
    { typename apply_traits<T>::apply_type{} } noexcept;
    { apply(in_place, std::forward<T>(lhs), std::forward<U>(rhs), std::move(op)) };
  };
// clang-format on

} // namespace jules

#endif // JULES_BASE_CONCEPTS_H
