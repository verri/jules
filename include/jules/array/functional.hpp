// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_FUNCTIONAL_H
#define JULES_ARRAY_FUNCTIONAL_H

#include <jules/array/binary_expr_array.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/meta/reference.hpp>
#include <jules/array/unary_expr_array.hpp>
#include <jules/core/debug.hpp>

#include <functional>

// TODO: XXX: think how to reuse the memory of a base_array&&
// TODO: better names for left_operation and right_operation

namespace jules
{

template <typename T, typename Op> struct left_operation
{
  T lhs;
  Op op;

  template <typename U>
  constexpr decltype(auto) operator()(U&& other) const
    noexcept(noexcept(std::declval<const Op&>()(std::declval<const T&>(), std::declval<U>())))
  {
    return op(lhs, std::forward<U>(other));
  }
};

template <typename T, typename Op> struct right_operation
{
  T rhs;
  Op op;

  template <typename U>
  constexpr decltype(auto) operator()(U&& other) const
    noexcept(noexcept(std::declval<const Op&>()(std::declval<U>(), std::declval<const T&>())))
  {
    return op(std::forward<U>(other), rhs);
  }
};

template <typename Op> static auto apply(const common_array auto& operand, Op op)
{
  return unary_expr_array(operand.begin(), operand.end(), std::move(op), operand.dimensions());
}

template <typename T, std::size_t N, typename Op> static auto apply(in_place_t, array<T, N>& operand, Op op) -> array<T, N>&
{
  apply(in_place, ref(operand), std::move(op));
  return operand;
}

template <typename Op> static auto apply(in_place_t, reference_array auto operand, Op op)
{
  for (auto& value : operand)
    op(value);
  return operand;
}

template <typename ArrayA, typename ArrayB, typename Op>
requires common_array<ArrayA>&& common_array<ArrayB> static auto apply(const ArrayA& lhs, const ArrayB& rhs, Op op)
{
  static_assert(ArrayA::order == ArrayB::order);
  DEBUG_ASSERT(lhs.dimensions() == rhs.dimensions(), debug::default_module, debug::level::extents_check, "extents mismatch");
  return binary_expr_array(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::move(op), lhs.dimensions());
}

template <typename T, std::size_t N, typename Op>
static auto apply(in_place_t, array<T, N>& lhs, const common_array auto& rhs, Op op) -> array<T, N>&
{
  apply(in_place, ref(lhs), rhs, std::move(op));
  return lhs;
}

template <reference_array RefArrayA, common_array ArrayB, typename Op>
static auto apply(in_place_t, RefArrayA lhs, const ArrayB& rhs, Op op)
{
  static_assert(RefArrayA::order == ArrayB::order);
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
  BINARY_INPLACE_REF_OPERATION(OP__)                                                                                             \
  BINARY_INPLACE_TYPE_OPERATION(OP__)                                                                                            \
  BINARY_INPLACE_REF_TYPE_OPERATION(OP__)

#define BINARY_OPERATION(OP__, FUNCTOR__)                                                                                        \
  template <typename ArrayA, typename ArrayB,                                                                                    \
            typename R = std::result_of_t<FUNCTOR__(const typename ArrayA::value_type&, const typename ArrayB::value_type&)>>    \
  requires common_array<ArrayA>&& common_array<ArrayB> auto operator OP__(const ArrayA& lhs, const ArrayB& rhs)                  \
  {                                                                                                                              \
    return apply(lhs, rhs, FUNCTOR__{});                                                                                         \
  }

#define BINARY_INPLACE_REF_OPERATION(OP__)                                                                                       \
  template <reference_array RefArrayA, common_array ArrayB,                                                                      \
            typename = decltype(std::declval<typename RefArrayA::value_type&>() OP__## =                                         \
                                  std::declval<const typename ArrayB::value_type&>())>                                           \
  auto operator OP__##=(RefArrayA lhs, const ArrayB& rhs)                                                                        \
  {                                                                                                                              \
    return apply(in_place, lhs, rhs, [](auto& x, const auto& y) { x OP__## = y; });                                              \
  }

#define BINARY_INPLACE_OPERATION(OP__)                                                                                           \
  template <typename T, std::size_t N, common_array ArrayB,                                                                      \
            typename = decltype(std::declval<T&>() OP__## = std::declval<const typename ArrayB::value_type&>())>                 \
  auto operator OP__##=(array<T, N>& lhs, const ArrayB& rhs)->array<T, N>&                                                       \
  {                                                                                                                              \
    return apply(in_place, lhs, rhs, [](auto& x, const auto& y) { x OP__## = y; });                                              \
  }

#define BINARY_RIGHT_TYPE_OPERATION(OP__, FUNCTOR__)                                                                             \
  template <common_array Array, typename T,                                                                                      \
            typename R = std::result_of_t<FUNCTOR__(const typename Array::value_type&, const T&)>>                               \
  requires(!common_array<T>) auto operator OP__(const Array& lhs, T rhs)                                                         \
  {                                                                                                                              \
    return apply(lhs, right_operation<T, FUNCTOR__>{std::move(rhs), {}});                                                        \
  }

#define BINARY_LEFT_TYPE_OPERATION(OP__, FUNCTOR__)                                                                              \
  template <typename T, common_array Array,                                                                                      \
            typename R = std::result_of_t<FUNCTOR__(const T&, const typename Array::value_type&)>>                               \
  requires(!common_array<T>) auto operator OP__(T lhs, const Array& rhs)                                                         \
  {                                                                                                                              \
    return apply(rhs, left_operation<T, FUNCTOR__>{std::move(lhs), {}});                                                         \
  }

#define BINARY_INPLACE_REF_TYPE_OPERATION(OP__)                                                                                  \
  template <reference_array RefArray, typename T,                                                                                \
            typename = decltype(std::declval<typename RefArray::value_type&>() OP__## = std::declval<const T&>())>               \
  requires(!common_array<T>) auto operator OP__##=(RefArray lhs, const T& rhs)                                                   \
  {                                                                                                                              \
    return apply(in_place, lhs, [&rhs](auto& x) { x OP__## = rhs; });                                                            \
  }

#define BINARY_INPLACE_TYPE_OPERATION(OP__)                                                                                      \
  template <typename T, std::size_t N, typename U, typename = decltype(std::declval<T&>() OP__## = std::declval<const U&>())>    \
  requires(!common_array<U>) auto operator OP__##=(array<T, N>& lhs, const U& rhs)->array<T, N>&                                 \
  {                                                                                                                              \
    return apply(in_place, lhs, [&rhs](auto& x) { x OP__## = rhs; });                                                            \
  }

#define UNARY_OPERATIONS_LIST                                                                                                    \
  UNARY_OPERATION(-, std::negate<>)                                                                                              \
  UNARY_OPERATION(!, std::logical_not<>)

#define UNARY_OPERATION(OP__, FUNCTOR__)                                                                                         \
  template <common_array Array> auto operator OP__(const Array& operand) { return apply(operand, FUNCTOR__{}); }

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
