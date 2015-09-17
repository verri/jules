#ifndef JULES_ARRAY_DETAIL_UTILITY_DECL_H
#define JULES_ARRAY_DETAIL_UTILITY_DECL_H

#include "util/numeric.hpp"

namespace jules
{
namespace detail
{
// Enabler
template <std::size_t N, typename... Types>
using all_size_enabler =
    std::enable_if_t<N == sizeof...(Types) && all_args(std::is_convertible<Types, std::size_t>::value...)>;

template <typename Range, typename R, typename T = R>
using range_type_enabler =
    std::enable_if_t<std::is_assignable<R&, typename std::remove_reference<Range>::type::value_type>::value,
                     T>;

template <typename T, typename U, typename R = void>
using assignable_enabler = std::enable_if_t<std::is_assignable<T&, U>::value, R>;

// Request check

template <std::size_t N> class base_slice;
template <typename T, std::size_t N> constexpr bool size_or_slice()
{
    return std::is_convertible<T, std::size_t>::value || std::is_convertible<T, base_slice<N>>::value;
}

template <typename Return, typename... Args>
using element_request = std::enable_if_t<all_args(std::is_convertible<Args, std::size_t>::value...), Return>;

template <typename Return, typename... Args>
using slice_request = std::enable_if_t<all_args(size_or_slice<Args, sizeof...(Args)>()...) &&
                                           !all_args(std::is_convertible<Args, std::size_t>::value...),
                                       Return>;

template <typename Return, typename... Args>
using indirect_request = std::enable_if_t<!all_args(size_or_slice<Args, sizeof...(Args)>()...), Return>;

// Declarations

template <typename T, std::size_t N> class base_ndarray;
template <typename T, std::size_t N> class ref_ndarray;
template <typename T, std::size_t N> class indirect_ndarray;
template <typename T, std::size_t N> class ref_ndarray_iterator;
template <typename IndexesLeft, typename IndexesRight, typename TypeLeft, typename TypeRight, typename F> class binary_expr_ndarray;
template <typename Indexes, typename T, typename F> class unary_expr_ndarray;


} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_UTILITY_DECL_H
