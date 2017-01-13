// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

// Whoever pursues righteousness and kindness will find life, righteousness, and honor.
// Proverbs 21:21 (ESV)

#ifndef JULES_ARRAY_DETAIL_COMMON_H
#define JULES_ARRAY_DETAIL_COMMON_H

#include <jules/base/numeric.hpp>
#include <jules/core/type.hpp>

#include <type_traits>

namespace jules
{
// Forward declaration

template <std::size_t> class base_slice;
template <> class base_slice<1>;

template <typename, std::size_t> class base_array;
template <typename, std::size_t> class contiguous_array;
template <typename, std::size_t> class ref_array;
template <typename, std::size_t> class ind_array;

template <typename T> class base_array<T, 1>;
template <typename T> class ref_array<T, 1>;
template <typename T> class ind_array<T, 1>;

template <typename, typename, typename, std::size_t> class binary_expr_array;
template <typename, typename, std::size_t> class unary_expr_array;

namespace detail
{

// Enablers
template <std::size_t N, typename... Types>
using n_indexes_enabler = std::enable_if_t<N == sizeof...(Types) && all_args(std::is_convertible<Types, uint>::value...)>;

// Request checks

template <typename T> constexpr auto index_or_slice()
{
  return std::is_convertible<T, uint>::value || std::is_convertible<T, base_slice<1>>::value;
}

template <typename Return, typename... Args>
using element_request = std::enable_if_t<all_args(std::is_convertible<Args, uint>::value...), Return>;

template <typename Return, typename... Args>
using slice_request =
  std::enable_if_t<all_args(index_or_slice<Args>()...) && !all_args(std::is_convertible<Args, uint>::value...), Return>;

template <typename Return, typename... Args>
using indirect_request = std::enable_if_t<!all_args(index_or_slice<Args>()...), Return>;

} // namespace detail

// Concept checking

template <typename T> struct Array : public std::false_type {
};

template <typename T, std::size_t N> struct Array<base_array<T, N>> : public std::true_type {
};

template <typename T, std::size_t N> struct Array<contiguous_array<T, N>> : public std::true_type {
};

template <typename T, std::size_t N> struct Array<ref_array<T, N>> : public std::true_type {
};

template <typename T, std::size_t N> struct Array<ind_array<T, N>> : public std::true_type {
};

template <typename It, typename F, std::size_t N> struct Array<unary_expr_array<It, F, N>> : public std::true_type {
};

template <typename LhsIt, typename RhsIt, typename F, std::size_t N>
struct Array<binary_expr_array<LhsIt, RhsIt, F, N>> : public std::true_type {
};

// Helpers for operators

template <std::size_t M, typename It, typename F>
auto make_expr_array(It first, It last, F&& f, typename base_slice<M>::extent_type extent)
  -> unary_expr_array<It, std::decay_t<F>, M>
{
  return {first, last, std::forward<F>(f), extent};
}

template <std::size_t M, typename LhsIt, typename RhsIt, typename F>
auto make_expr_array(LhsIt lhs_first, LhsIt lhs_last, RhsIt rhs_first, RhsIt rhs_last, F&& f,
                     typename base_slice<M>::extent_type extent) -> binary_expr_array<LhsIt, RhsIt, std::decay_t<F>, M>
{
  return {lhs_first, lhs_last, rhs_first, rhs_last, std::forward<F>(f), extent};
}

} // namespace jules

#endif // JULES_ARRAY_DETAIL_COMMON_H
