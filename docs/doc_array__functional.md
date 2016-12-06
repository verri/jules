---
---

# Header file `array/functional.hpp`<a id="array/functional.hpp"></a>

``` cpp
#define JULES_ARRAY_FUNCTIONAL_H

#define UNPACK

#define OPERATIONS_LIST

#define BASE_NDARRAY

#define REF_NDARRAY

#define INDIRECT_NDARRAY

#define BINARY_EXPR_NDARRAY

#define UNARY_EXPR_NDARRAY

#define BINARY_OPERATIONS_LIST

#define BINARY_APPLY_COMBINATIONS

#define BINARY_COMBINATIONS

#define BINARY_TYPE_OPERATION

#define BINARY_APPLY_OPERATION

#define BINARY_OPERATION

#define BINARY_RIGHT_TYPE_OPERATION

#define BINARY_LEFT_TYPE_OPERATION

#define UNARY_OPERATIONS_LIST

#define UNARY_APPLY_OPERATION

#define UNARY_OPERATION

namespace jules
{
    template <typename T, std::size_t N, typename Operator>
    auto apply(const base_array<T, N>& operand, const Operator& op);
    
    template <typename T, std::size_t N>
    auto operator+(const base_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator-(const base_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator~(const base_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator!(const base_array<T, N>& operand);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const base_array<T, N>& lhs, const base_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const base_array<T, N>& lhs, const ref_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const base_array<T, N>& lhs, const ind_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename A, typename B, typename Operator>
    auto apply(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename A, typename B, typename C, typename Operator>
    auto apply(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator+(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator+(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator-(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator-(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator*(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator*(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator/(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator/(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator%(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator%(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator&(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator&(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator|(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator|(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator^(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator^(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator&&(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator&&(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator||(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator||(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator==(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator==(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator!=(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator!=(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator<(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator<(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator<=(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator<=(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator>(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator>(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const base_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const base_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const base_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator>=(const base_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator>=(const base_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const base_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const U& lhs, const base_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename Operator>
    auto apply(const ref_array<T, N>& operand, const Operator& op);
    
    template <typename T, std::size_t N>
    auto operator+(const ref_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator-(const ref_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator~(const ref_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator!(const ref_array<T, N>& operand);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const ref_array<T, N>& lhs, const base_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename A, typename B, typename Operator>
    auto apply(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename A, typename B, typename C, typename Operator>
    auto apply(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator+(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator+(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator-(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator-(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator*(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator*(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator/(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator/(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator%(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator%(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator&(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator&(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator|(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator|(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator^(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator^(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator&&(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator&&(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator||(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator||(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator==(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator==(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator!=(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator!=(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator<(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator<(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator<=(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator<=(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator>(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator>(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const ref_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const ref_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const ref_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator>=(const ref_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator>=(const ref_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const ref_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const U& lhs, const ref_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename Operator>
    auto apply(const ind_array<T, N>& operand, const Operator& op);
    
    template <typename T, std::size_t N>
    auto operator+(const ind_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator-(const ind_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator~(const ind_array<T, N>& operand);
    
    template <typename T, std::size_t N>
    auto operator!(const ind_array<T, N>& operand);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const ind_array<T, N>& lhs, const base_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U, typename Operator>
    auto apply(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename A, typename B, typename Operator>
    auto apply(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename A, typename B, typename C, typename Operator>
    auto apply(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs, const Operator& op);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator+(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator+(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator+(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator-(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator-(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator-(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator*(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator*(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator*(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator/(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator/(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator/(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator%(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator%(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator%(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator&(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator&(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator|(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator|(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator|(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator^(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator^(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator^(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator&&(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator&&(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator&&(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator||(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator||(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator||(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator==(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator==(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator==(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator!=(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator!=(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator!=(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator<(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator<(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator<=(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator<=(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator<=(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator>(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator>(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const ind_array<T, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const ind_array<T, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename U>
    auto operator>=(const ind_array<T, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B>
    auto operator>=(const ind_array<T, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename T, std::size_t N, typename A, typename B, typename C>
    auto operator>=(const ind_array<T, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const ind_array<T, N>& lhs, const U& rhs);
    
    template <typename T, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const U& lhs, const ind_array<T, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename Operator>
    auto apply(const unary_expr_array<It, Op, N>& operand, const Operator& op);
    
    template <typename It, typename Op, std::size_t N>
    auto operator+(const unary_expr_array<It, Op, N>& operand);
    
    template <typename It, typename Op, std::size_t N>
    auto operator-(const unary_expr_array<It, Op, N>& operand);
    
    template <typename It, typename Op, std::size_t N>
    auto operator~(const unary_expr_array<It, Op, N>& operand);
    
    template <typename It, typename Op, std::size_t N>
    auto operator!(const unary_expr_array<It, Op, N>& operand);
    
    template <typename It, typename Op, std::size_t N, typename U, typename Operator>
    auto apply(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs, const Operator& op);
    
    template <typename It, typename Op, std::size_t N, typename U, typename Operator>
    auto apply(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs, const Operator& op);
    
    template <typename It, typename Op, std::size_t N, typename U, typename Operator>
    auto apply(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs, const Operator& op);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename Operator>
    auto apply(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs, const Operator& op);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C, typename Operator>
    auto apply(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs, const Operator& op);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator+(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator+(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator+(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator+(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator+(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator-(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator-(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator-(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator-(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator-(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator*(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator*(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator*(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator*(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator*(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator/(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator/(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator/(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator/(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator/(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator%(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator%(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator%(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator%(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator%(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator&(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator&(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator&(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator&(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator&(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator|(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator|(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator|(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator|(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator|(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator^(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator^(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator^(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator^(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator^(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator&&(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator&&(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator&&(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator&&(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator&&(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator||(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator||(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator||(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator||(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator||(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator==(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator==(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator==(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator==(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator==(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator!=(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator!=(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator!=(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator!=(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator!=(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator<(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator<(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator<(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator<(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator<(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator<=(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator<=(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator<=(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator<=(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator<=(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator>(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator>(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator>(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator>(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator>(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator>=(const unary_expr_array<It, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator>=(const unary_expr_array<It, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U>
    auto operator>=(const unary_expr_array<It, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B>
    auto operator>=(const unary_expr_array<It, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator>=(const unary_expr_array<It, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const unary_expr_array<It, Op, N>& lhs, const U& rhs);
    
    template <typename It, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const U& lhs, const unary_expr_array<It, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename Operator>
    auto apply(const binary_expr_array<LhsIt, RhsIt, Op, N>& operand, const Operator& op);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N>
    auto operator+(const binary_expr_array<LhsIt, RhsIt, Op, N>& operand);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N>
    auto operator-(const binary_expr_array<LhsIt, RhsIt, Op, N>& operand);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N>
    auto operator~(const binary_expr_array<LhsIt, RhsIt, Op, N>& operand);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N>
    auto operator!(const binary_expr_array<LhsIt, RhsIt, Op, N>& operand);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename Operator>
    auto apply(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs, const Operator& op);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename Operator>
    auto apply(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs, const Operator& op);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename Operator>
    auto apply(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs, const Operator& op);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename Operator>
    auto apply(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs, const Operator& op);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C, typename Operator>
    auto apply(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs, const Operator& op);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator+(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator+(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator+(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator+(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator+(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator+(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator-(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator-(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator-(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator-(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator-(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator-(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator*(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator*(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator*(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator*(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator*(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator*(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator/(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator/(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator/(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator/(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator/(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator/(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator%(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator%(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator%(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator%(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator%(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator%(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator|(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator|(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator|(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator|(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator|(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator|(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator^(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator^(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator^(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator^(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator^(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator^(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator&&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator&&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator&&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator&&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator&&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator&&(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator||(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator||(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator||(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator||(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator||(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator||(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator==(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator==(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator==(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator==(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator==(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator==(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator!=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator!=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator!=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator!=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator!=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator!=(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator<(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator<(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator<(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator<(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator<(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator<=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator<=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator<=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator<=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator<=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator<=(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator>(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator>(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator>(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator>(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator>(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator>=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const base_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator>=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ref_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U>
    auto operator>=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const ind_array<U, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B>
    auto operator>=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const unary_expr_array<A, B, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename A, typename B, typename C>
    auto operator>=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const binary_expr_array<A, B, C, N>& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const binary_expr_array<LhsIt, RhsIt, Op, N>& lhs, const U& rhs);
    
    template <typename LhsIt, typename RhsIt, typename Op, std::size_t N, typename U, typename = std::enable_if_t<!is_array<U>()>>
    auto operator>=(const U& lhs, const binary_expr_array<LhsIt, RhsIt, Op, N>& rhs);
}
```
