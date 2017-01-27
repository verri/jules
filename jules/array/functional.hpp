// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_FUNCTIONAL_H
#define JULES_ARRAY_FUNCTIONAL_H

#include <jules/array/detail/common.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/meta.hpp>

// TODO: XXX: think how to reuse the memory of a base_array&&

#define UNPACK(...) __VA_ARGS__

#define OPERATIONS_LIST(TX__, X__, N__)                                                                                          \
  UNARY_OPERATIONS_LIST(TX__, X__, N__)                                                                                          \
  BINARY_OPERATIONS_LIST(TX__, X__, N__)

#define BASE_NDARRAY(X__, N__) const base_array<X__, N__>&
#define CONTIGUOUS_ARRAY(X__, N__) const contiguous_array<X__, N__>&
#define REF_NDARRAY(X__, N__) const ref_array<X__, N__>&
#define INDIRECT_NDARRAY(X__, N__) const ind_array<X__, N__>&
#define BINARY_EXPR_NDARRAY(X__, Y__, Z__, N__) const binary_expr_array<X__, Y__, Z__, N__>&
#define UNARY_EXPR_NDARRAY(X__, Y__, N__) const unary_expr_array<X__, Y__, N__>&

//=== Binary ===//

#define BINARY_OPERATIONS_LIST(TX__, X__, N__)                                                                                   \
  BINARY_APPLY_COMBINATIONS(TX__, X__, N__)                                                                                      \
  BINARY_COMBINATIONS(TX__, X__, N__, +)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, -)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, *)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, /)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, %)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, &)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, |)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, ^)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, &&)                                                                                        \
  BINARY_COMBINATIONS(TX__, X__, N__, ||)                                                                                        \
  BINARY_COMBINATIONS(TX__, X__, N__, ==)                                                                                        \
  BINARY_COMBINATIONS(TX__, X__, N__, !=)                                                                                        \
  BINARY_COMBINATIONS(TX__, X__, N__, <)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, <=)                                                                                        \
  BINARY_COMBINATIONS(TX__, X__, N__, >)                                                                                         \
  BINARY_COMBINATIONS(TX__, X__, N__, >=)

#define BINARY_APPLY_COMBINATIONS(TX__, X__, N__)                                                                                \
  BINARY_APPLY_OPERATION(TX__, X__, (typename U), (BASE_NDARRAY(U, N__)))                                                        \
  BINARY_APPLY_OPERATION(TX__, X__, (typename U), (CONTIGUOUS_ARRAY(U, N__)))                                                    \
  BINARY_APPLY_OPERATION(TX__, X__, (typename U), (REF_NDARRAY(U, N__)))                                                         \
  BINARY_APPLY_OPERATION(TX__, X__, (typename U), (INDIRECT_NDARRAY(U, N__)))                                                    \
  BINARY_APPLY_OPERATION(TX__, X__, (typename A, typename B), (UNARY_EXPR_NDARRAY(A, B, N__)))                                   \
  BINARY_APPLY_OPERATION(TX__, X__, (typename A, typename B, typename C), (BINARY_EXPR_NDARRAY(A, B, C, N__)))

#define BINARY_COMBINATIONS(TX__, X__, N__, OP__)                                                                                \
  BINARY_OPERATION(TX__, X__, (typename U), (BASE_NDARRAY(U, N__)), OP__)                                                        \
  BINARY_OPERATION(TX__, X__, (typename U), (CONTIGUOUS_ARRAY(U, N__)), OP__)                                                    \
  BINARY_OPERATION(TX__, X__, (typename U), (REF_NDARRAY(U, N__)), OP__)                                                         \
  BINARY_OPERATION(TX__, X__, (typename U), (INDIRECT_NDARRAY(U, N__)), OP__)                                                    \
  BINARY_OPERATION(TX__, X__, (typename A, typename B), (UNARY_EXPR_NDARRAY(A, B, N__)), OP__)                                   \
  BINARY_OPERATION(TX__, X__, (typename A, typename B, typename C), (BINARY_EXPR_NDARRAY(A, B, C, N__)), OP__)                   \
  BINARY_TYPE_OPERATION(TX__, X__, OP__)

#define BINARY_TYPE_OPERATION(TX__, X__, OP__)                                                                                   \
  BINARY_RIGHT_TYPE_OPERATION(TX__, X__, OP__)                                                                                   \
  BINARY_LEFT_TYPE_OPERATION(TX__, X__, OP__)

#define BINARY_APPLY_OPERATION(TX__, X__, TY__, Y__)                                                                             \
  template <UNPACK TX__, UNPACK TY__, typename Operator> auto apply(UNPACK X__ lhs, UNPACK Y__ rhs, Operator&& op)               \
  {                                                                                                                              \
    DEBUG_ASSERT(lhs.extents() == rhs.extents(), debug::default_module, debug::level::extents_check, "extents mismatch");        \
    return make_expr_array<N>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::forward<Operator>(op), lhs.extents());        \
  }

#define BINARY_OPERATION(TX__, X__, TY__, Y__, OP__)                                                                             \
  template <UNPACK TX__, UNPACK TY__> auto operator OP__(UNPACK X__ lhs, UNPACK Y__ rhs)                                         \
  {                                                                                                                              \
    using LhsValue = typename std::decay_t<UNPACK X__>::value_type;                                                              \
    using RhsValue = typename std::decay_t<UNPACK Y__>::value_type;                                                              \
    return apply(lhs, rhs, [](const LhsValue& a, const RhsValue& b) { return a OP__ b; });                                       \
  }

#define BINARY_RIGHT_TYPE_OPERATION(TX__, X__, OP__)                                                                             \
  template <UNPACK TX__, typename U, typename = meta::fallback<Array<U>>> auto operator OP__(UNPACK X__ lhs, U rhs)              \
  {                                                                                                                              \
    using Value = typename std::decay_t<UNPACK X__>::value_type;                                                                 \
    return apply(lhs, [rhs = std::move(rhs)](const Value& lhs) { return lhs OP__ rhs; });                                        \
  }

#define BINARY_LEFT_TYPE_OPERATION(TY__, Y__, OP__)                                                                              \
  template <UNPACK TY__, typename U, typename = meta::fallback<Array<U>>> auto operator OP__(U lhs, UNPACK Y__ rhs)              \
  {                                                                                                                              \
    using Value = typename std::decay_t<UNPACK Y__>::value_type;                                                                 \
    return apply(rhs, [lhs = std::move(lhs)](const Value& rhs) { return lhs OP__ rhs; });                                        \
  }

//=== Unary ===//

#define UNARY_OPERATIONS_LIST(TX__, X__, N__)                                                                                    \
  UNARY_APPLY_OPERATION(TX__, X__, N__)                                                                                          \
  UNARY_OPERATION(TX__, X__, N__, +)                                                                                             \
  UNARY_OPERATION(TX__, X__, N__, -)                                                                                             \
  UNARY_OPERATION(TX__, X__, N__, ~)                                                                                             \
  UNARY_OPERATION(TX__, X__, N__, !)

#define UNARY_APPLY_OPERATION(TX__, X__, N__)                                                                                    \
  template <UNPACK TX__, typename Operator> auto apply(UNPACK X__ operand, Operator&& op)                                        \
  {                                                                                                                              \
    return make_expr_array<N>(operand.begin(), operand.end(), std::forward<Operator>(op), operand.extents());                    \
  }

#define UNARY_OPERATION(TX__, X__, N__, OP__)                                                                                    \
  template <UNPACK TX__> auto operator OP__(UNPACK X__ operand)                                                                  \
  {                                                                                                                              \
    using Value = typename std::decay_t<UNPACK X__>::value_type;                                                                 \
    return apply(operand, [](const Value& a) { return OP__ a; });                                                                \
  }

//=== Call X-Macro ===/

namespace jules
{

OPERATIONS_LIST((typename T, std::size_t N), (const base_array<T, N>&), N)
OPERATIONS_LIST((typename T, std::size_t N), (const contiguous_array<T, N>&), N)
OPERATIONS_LIST((typename T, std::size_t N), (const ref_array<T, N>&), N)
OPERATIONS_LIST((typename T, std::size_t N), (const ind_array<T, N>&), N)
OPERATIONS_LIST((typename It, typename Op, std::size_t N), (const unary_expr_array<It, Op, N>&), N)
OPERATIONS_LIST((typename LhsIt, typename RhsIt, typename Op, std::size_t N), (const binary_expr_array<LhsIt, RhsIt, Op, N>&), N)

} // namespace jules

//=== Clean up ===/

#undef BASE_NDARRAY
#undef CONTIGUOUS_ARRAY
#undef REF_NDARRAY
#undef INDIRECT_NDARRAY
#undef BINARY_EXPR_NDARRAY
#undef UNARY_EXPR_NDARRAY

#undef UNPACK

#undef UNARY_APPLY_OPERATION
#undef UNARY_OPERATION
#undef UNARY_OPERATIONS_LIST

#undef BINARY_APPLY_OPERATION
#undef BINARY_OPERATION
#undef BINARY_LEFT_TYPE_OPERATION
#undef BINARY_RIGHT_TYPE_OPERATION
#undef BINARY_TYPE_OPERATION
#undef BINARY_COMBINATIONS
#undef BINARY_APPLY_COMBINATIONS
#undef BINARY_OPERATIONS_LIST

#undef OPERATIONS_LIST

#endif // JULES_ARRAY_FUNCTIONAL_H
