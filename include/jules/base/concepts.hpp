// Copyright (c) 2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_CONCEPTS_H
/// \exclude
#define JULES_BASE_CONCEPTS_H

#include <jules/core/concepts.hpp>
#include <jules/core/type.hpp>

#include <concepts>
#include <type_traits>
#include <utility>

namespace jules
{

template <typename T> struct apply_traits
{
  using apply_type = void;
};

// clang-format off
template <typename Op, typename T, typename Apply = typename apply_traits<std::decay_t<T>>::apply_type>
concept applies_to =
  (!std::same_as<void, Apply>) && std::is_trivial_v<Apply> &&
  requires(T value, Op op, const Apply apply) {
    { Apply{} } noexcept;
    { apply(value, std::move(op)) };
  };

template <typename Op, typename T, typename U, typename Apply = typename apply_traits<std::decay_t<T>>::apply_type>
concept applies_to_by =
  (!std::same_as<void, Apply>) && std::is_trivial_v<Apply> &&
  requires(const T& lhs, const U& rhs, Op op, const Apply apply) {
    { Apply{} } noexcept;
    { apply(lhs, rhs, std::move(op)) };
  };

template <typename Op, typename T, typename Apply = typename apply_traits<std::decay_t<T>>::apply_type>
concept applies_in_place_to =
  (!std::same_as<void, Apply>) && std::is_trivial_v<Apply> &&
  requires(T& value, Op op, const Apply apply) {
    { Apply{} } noexcept;
    { apply(in_place, value, std::move(op)) };
  };

template <typename Op, typename T, typename U, typename Apply = typename apply_traits<std::decay_t<T>>::apply_type>
concept applies_in_place_to_by =
  (!std::same_as<void, Apply>) && std::is_trivial_v<Apply> &&
  requires(T& lhs, U rhs, Op op, const Apply apply) {
    { Apply{} } noexcept;
    { apply(in_place, lhs, rhs, std::move(op)) };
  };
// clang-format on

} // namespace jules

#endif // JULES_BASE_CONCEPTS_H
