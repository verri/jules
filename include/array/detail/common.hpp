#ifndef JULES_ARRAY_DETAIL_COMMON_DECL_H
#define JULES_ARRAY_DETAIL_COMMON_DECL_H

#include "range/range.hpp"
#include "util/numeric.hpp"
#include "util/async.hpp"

#include <array>
#include <vector>

namespace jules
{
namespace detail
{
// Enablers

template <std::size_t N, typename... Types>
using all_size_enabler =
    std::enable_if_t<N == sizeof...(Types) && all_args(std::is_convertible<Types, std::size_t>::value...)>;

template <typename Range, typename R, typename T = R>
using range_type_enabler =
    std::enable_if_t<std::is_assignable<R&, typename std::remove_reference<Range>::type::value_type>::value,
                     T>;

template <typename T, typename U, typename R = void>
using assignable_enabler = std::enable_if_t<std::is_assignable<T&, U>::value, R>;

// Request checks

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

// Array classes

template <std::size_t> class base_slice;
template <std::size_t> class base_slice_iterator;

template <typename, std::size_t> class base_ndarray;
template <typename, std::size_t> class ref_ndarray;
template <typename, std::size_t> class indirect_ndarray;

template <typename, std::size_t> class ref_ndarray_iterator;
template <typename, std::size_t> class ref_ndarray_data_iterator;
template <typename, std::size_t> class indirect_ndarray_iterator;
template <typename, std::size_t> class indirect_ndarray_data_iterator;

template <typename, typename, typename, std::size_t> class binary_expr_ndarray;
template <typename, typename, std::size_t> class unary_expr_ndarray;

} // namespace detail

// Friend classes

template <typename, typename> class base_column_view;
template <typename, typename, typename> class base_dataframe_colview;

} // namespace jules

#endif // JULES_ARRAY_DETAIL_COMMON_DECL_H
