// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_FUNCTIONAL_H
#define JULES_ARRAY_FUNCTIONAL_H

#include <jules/array/binary_expr_array.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/unary_expr_array.hpp>
#include <jules/core/debug.hpp>

#include <functional>

// TODO: XXX: think how to reuse the memory of a base_array&&
// TODO: better names for left_operation and right_operation

namespace jules
{

template <typename T, typename Op> struct left_operation {
  T lhs;
  Op op;

  template <typename U>
  constexpr decltype(auto) operator()(U&& other) const
    noexcept(noexcept(std::declval<const Op&>()(std::declval<const T&>(), std::declval<U>())))
  {
    return op(lhs, std::forward<U>(other));
  }
};

template <typename T, typename Op> struct right_operation {
  T rhs;
  Op op;

  template <typename U>
  constexpr decltype(auto) operator()(U&& other) const
    noexcept(noexcept(std::declval<const Op&>()(std::declval<U>(), std::declval<const T&>())))
  {
    return op(std::forward<U>(other), rhs);
  }
};

template <typename Array, typename Op> static auto apply(const common_array_base<Array>& operand, Op op)
{
  return unary_expr_array(operand.begin(), operand.end(), std::move(op), operand.dimensions());
}

template <typename ArrayA, typename ArrayB, typename Op>
static auto apply(const common_array_base<ArrayA>& lhs, const common_array_base<ArrayB>& rhs, Op op)
{
  static_assert(ArrayA::order == ArrayB::order);
  DEBUG_ASSERT(lhs.dimensions() == rhs.dimensions(), debug::default_module, debug::level::extents_check, "extents mismatch");
  return binary_expr_array(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::move(op), lhs.dimensions());
}

#define OPERATIONS_LIST                                                                                                          \
  UNARY_OPERATIONS_LIST                                                                                                          \
  BINARY_OPERATIONS_LIST

#define BINARY_OPERATIONS_LIST                                                                                                   \
  BINARY_COMBINATIONS(+, std::plus<>)                                                                                            \
  BINARY_COMBINATIONS(-, std::minus<>)                                                                                           \
  BINARY_COMBINATIONS(*, std::multiplies<>)                                                                                      \
  BINARY_COMBINATIONS(/, std::divides<>)                                                                                         \
  BINARY_COMBINATIONS(%, std::modulus<>)                                                                                         \
  BINARY_COMBINATIONS(&&, std::logical_and<>)                                                                                    \
  BINARY_COMBINATIONS(||, std::logical_or<>)                                                                                     \
  BINARY_COMBINATIONS(==, std::equal_to<>)                                                                                       \
  BINARY_COMBINATIONS(!=, std::not_equal_to<>)                                                                                   \
  BINARY_COMBINATIONS(<, std::less<>)                                                                                            \
  BINARY_COMBINATIONS(<=, std::less_equal<>)                                                                                     \
  BINARY_COMBINATIONS(>, std::greater<>)                                                                                         \
  BINARY_COMBINATIONS(>=, std::greater_equal<>)

#define BINARY_COMBINATIONS(OP__, FUNCTOR__)                                                                                     \
  BINARY_OPERATION(OP__, FUNCTOR__)                                                                                              \
  BINARY_RIGHT_TYPE_OPERATION(OP__, FUNCTOR__)                                                                                   \
  BINARY_LEFT_TYPE_OPERATION(OP__, FUNCTOR__)

#define BINARY_OPERATION(OP__, FUNCTOR__)                                                                                        \
  template <typename ArrayA, typename ArrayB>                                                                                    \
  auto operator OP__(const common_array_base<ArrayA>& lhs, const common_array_base<ArrayB>& rhs)                                 \
  {                                                                                                                              \
    return apply(lhs, rhs, FUNCTOR__{});                                                                                         \
  }

#define BINARY_RIGHT_TYPE_OPERATION(OP__, FUNCTOR__)                                                                             \
  template <typename Array, typename T, typename = meta::fallback<CommonArray<T>>>                                               \
  auto operator OP__(const common_array_base<Array>& lhs, T rhs)                                                                 \
  {                                                                                                                              \
    return apply(lhs, right_operation<T, FUNCTOR__>{std::move(rhs), {}});                                                        \
  }

#define BINARY_LEFT_TYPE_OPERATION(OP__, FUNCTOR__)                                                                              \
  template <typename T, typename Array, typename = meta::fallback<CommonArray<T>>>                                               \
  auto operator OP__(T lhs, const common_array_base<Array>& rhs)                                                                 \
  {                                                                                                                              \
    return apply(rhs, left_operation<T, FUNCTOR__>{std::move(lhs), {}});                                                         \
  }

#define UNARY_OPERATIONS_LIST                                                                                                    \
  UNARY_OPERATION(-, std::negate<>)                                                                                              \
  UNARY_OPERATION(!, std::logical_not<>)

#define UNARY_OPERATION(OP__, FUNCTOR__)                                                                                         \
  template <typename Array> auto operator OP__(const common_array_base<Array>& operand) { return apply(operand, FUNCTOR__{}); }

OPERATIONS_LIST

#undef UNARY_OPERATION
#undef UNARY_OPERATIONS_LIST

#undef BINARY_OPERATION
#undef BINARY_LEFT_TYPE_OPERATION
#undef BINARY_RIGHT_TYPE_OPERATION
#undef BINARY_COMBINATIONS
#undef BINARY_OPERATIONS_LIST

#undef OPERATIONS_LIST

} // namespace jules

#endif // JULES_ARRAY_FUNCTIONAL_H
