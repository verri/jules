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
using n_indexes_enabler = std::enable_if_t<N == sizeof...(Types) && all(std::is_convertible<Types, uint>::value...)>;

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

// // TODO: Helpers for operators
//
// template <typename It, typename F, std::size_t M>
// unary_expr_array<It, F, M> make_expr_array(const It&, const It&, const F&, const std::array<std::size_t, M>&);
//
// template <typename LhsIt, typename RhsIt, typename F, std::size_t M>
// binary_expr_array<LhsIt, RhsIt, F, M> make_expr_array(const LhsIt&, const LhsIt&, const RhsIt&, const RhsIt&, const F&,
//                                                           const std::array<std::size_t, M>&);

} // namespace detail

// Concept checking

template <typename T> struct is_array : public std::false_type {
};

template <typename T, std::size_t N> struct is_array<base_array<T, N>> : public std::true_type {
};

template <typename T, std::size_t N> struct is_array<ref_array<T, N>> : public std::true_type {
};

template <typename T, std::size_t N> struct is_array<ind_array<T, N>> : public std::true_type {
};

template <typename It, typename F, std::size_t N> struct is_array<unary_expr_array<It, F, N>> : public std::true_type {
};

template <typename LhsIt, typename RhsIt, typename F, std::size_t N>
struct is_array<binary_expr_array<LhsIt, RhsIt, F, N>> : public std::true_type {
};

template <typename T> constexpr auto is_array_v() { return is_array<std::decay_t<T>>::value; }

namespace detail
{

// Request checks for arrays

template <typename R, typename T> using array_request = std::enable_if_t<is_array_v<T>(), R>;
template <typename R, typename T> using not_array_request = std::enable_if_t<!is_array_v<T>(), R>;

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_COMMON_H
