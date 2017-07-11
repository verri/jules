// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

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

template <typename Array, typename Op> static auto apply(in_place_t, common_array_base<Array>& operand, Op op) -> Array&
{
  for (auto& value : operand)
    op(value);
  return operand;
}

template <typename ArrayA, typename ArrayB, typename Op>
static auto apply(const common_array_base<ArrayA>& lhs, const common_array_base<ArrayB>& rhs, Op op)
{
  static_assert(ArrayA::order == ArrayB::order);
  DEBUG_ASSERT(lhs.dimensions() == rhs.dimensions(), debug::default_module, debug::level::extents_check, "extents mismatch");
  return binary_expr_array(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::move(op), lhs.dimensions());
}

template <typename ArrayA, typename ArrayB, typename Op>
static auto apply(in_place_t, common_array_base<ArrayA>& lhs, const common_array_base<ArrayB>& rhs, Op op) -> ArrayA&
{
  static_assert(ArrayA::order == ArrayB::order);
  DEBUG_ASSERT(lhs.dimensions() == rhs.dimensions(), debug::default_module, debug::level::extents_check, "extents mismatch");

  auto itl = lhs.begin();
  auto itr = rhs.begin();
  const auto end = lhs.end();

  while (itl != end)
    op(*itl++, *itr++);

  return lhs;
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
  BINARY_COMBINATIONS(>=, std::greater_equal<>)                                                                                  \
  BINARY_INPLACE_COMBINATIONS(+)                                                                                                 \
  BINARY_INPLACE_COMBINATIONS(-)                                                                                                 \
  BINARY_INPLACE_COMBINATIONS(*)                                                                                                 \
  BINARY_INPLACE_COMBINATIONS(/)                                                                                                 \
  BINARY_INPLACE_COMBINATIONS(%)

#define BINARY_COMBINATIONS(OP__, FUNCTOR__)                                                                                     \
  BINARY_OPERATION(OP__, FUNCTOR__)                                                                                              \
  BINARY_RIGHT_TYPE_OPERATION(OP__, FUNCTOR__)                                                                                   \
  BINARY_LEFT_TYPE_OPERATION(OP__, FUNCTOR__)

#define BINARY_INPLACE_COMBINATIONS(OP__)                                                                                        \
  BINARY_INPLACE_OPERATION(OP__)                                                                                                 \
  BINARY_INPLACE_TYPE_OPERATION(OP__)

#define BINARY_OPERATION(OP__, FUNCTOR__)                                                                                        \
  template <typename ArrayA, typename ArrayB,                                                                                    \
            typename R = std::result_of_t<FUNCTOR__(const typename ArrayA::value_type&, const typename ArrayB::value_type&)>>    \
  auto operator OP__(const common_array_base<ArrayA>& lhs, const common_array_base<ArrayB>& rhs)                                 \
  {                                                                                                                              \
    return apply(lhs, rhs, FUNCTOR__{});                                                                                         \
  }

#define BINARY_INPLACE_OPERATION(OP__)                                                                                           \
  template <typename ArrayA, typename ArrayB,                                                                                    \
            typename = decltype(std::declval<typename ArrayA::value_type&>() OP__## =                                            \
                                  std::declval<const typename ArrayB::value_type&>())>                                           \
  auto operator OP__##=(common_array_base<ArrayA>& lhs, const common_array_base<ArrayB>& rhs)->ArrayA&                           \
  {                                                                                                                              \
    return apply(in_place, lhs, rhs, [](auto& x, const auto& y) { x OP__## = y; });                                              \
  }

#define BINARY_RIGHT_TYPE_OPERATION(OP__, FUNCTOR__)                                                                             \
  template <typename Array, typename T, typename = meta::fallback<CommonArray<T>>,                                               \
            typename R = std::result_of_t<FUNCTOR__(const typename Array::value_type&, const T&)>>                               \
  auto operator OP__(const common_array_base<Array>& lhs, T rhs)                                                                 \
  {                                                                                                                              \
    return apply(lhs, right_operation<T, FUNCTOR__>{std::move(rhs), {}});                                                        \
  }

#define BINARY_LEFT_TYPE_OPERATION(OP__, FUNCTOR__)                                                                              \
  template <typename T, typename Array, typename = meta::fallback<CommonArray<T>>,                                               \
            typename R = std::result_of_t<FUNCTOR__(const T&, const typename Array::value_type&)>>                               \
  auto operator OP__(T lhs, const common_array_base<Array>& rhs)                                                                 \
  {                                                                                                                              \
    return apply(rhs, left_operation<T, FUNCTOR__>{std::move(lhs), {}});                                                         \
  }

#define BINARY_INPLACE_TYPE_OPERATION(OP__)                                                                                      \
  template <typename Array, typename T, typename = meta::fallback<CommonArray<T>>,                                               \
            typename = decltype(std::declval<typename Array::value_type&>() OP__## = std::declval<const T&>())>                  \
  auto operator OP__##=(common_array_base<Array>& lhs, const T& rhs)->Array&                                                     \
  {                                                                                                                              \
    return apply(in_place, lhs, [&rhs](auto& x) { x OP__## = rhs; });                                                            \
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
