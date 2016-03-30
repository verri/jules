#ifndef JULES_ARRAY_DETAIL_COMMON_DECL_H
#define JULES_ARRAY_DETAIL_COMMON_DECL_H

#include <jules/range/range.hpp>
#include <jules/util/async.hpp>
#include <jules/util/numeric.hpp>
#include <jules/util/type.hpp>

#include <array>
#include <type_traits>
#include <vector>

namespace jules
{
namespace detail
{
// Enablers

template <std::size_t N, typename... Types>
using all_size_enabler = std::enable_if_t<N == sizeof...(Types) && all_args(std::is_convertible<Types, std::size_t>::value...)>;

template <typename Range, typename R, typename T = R>
using range_type_enabler = std::enable_if_t<std::is_assignable<R&, range_value_t<Range>>::value, T>;

template <typename T, typename U, typename R = void>
using assignable_enabler = std::enable_if_t<std::is_assignable<T&, U>::value, R>;

// Request checks

template <std::size_t N> class base_slice;
template <typename T> constexpr bool size_or_slice()
{
    return std::is_convertible<T, std::size_t>::value || std::is_convertible<T, base_slice<1>>::value;
}

template <typename Return, typename... Args>
using element_request = std::enable_if_t<all_args(std::is_convertible<Args, std::size_t>::value...), Return>;

template <typename Return, typename... Args>
using slice_request =
    std::enable_if_t<all_args(size_or_slice<Args>()...) && !all_args(std::is_convertible<Args, std::size_t>::value...), Return>;

template <typename Return, typename... Args>
using indirect_request = std::enable_if_t<!all_args(size_or_slice<Args>()...), Return>;

// Array classes

template <std::size_t> class base_slice;
template <std::size_t> class base_slice_iterator;

template <typename, std::size_t> class base_ndarray;
template <typename, std::size_t> class ref_ndarray;
template <typename, std::size_t> class indirect_ndarray;

template <typename T> class ref_ndarray<T, 1>;
template <typename T> class indirect_ndarray<T, 1>;

template <typename, std::size_t> class ref_ndarray_iterator;
template <typename, std::size_t> class ref_ndarray_data_iterator;
template <typename, std::size_t> class indirect_ndarray_iterator;
template <typename> class indirect_ndarray_data_iterator;

template <typename T> class ref_ndarray_iterator<T, 1>;
template <typename T> class indirect_ndarray_iterator<T, 1>;

template <typename, typename, typename, std::size_t> class binary_expr_ndarray;
template <typename, typename, std::size_t> class unary_expr_ndarray;

template <typename T> struct is_array_helper {
    static constexpr bool value = false;
};

template <typename T, std::size_t N> struct is_array_helper<base_ndarray<T, N>> {
    static constexpr bool value = true;
};

template <typename T, std::size_t N> struct is_array_helper<ref_ndarray<T, N>> {
    static constexpr bool value = true;
};

template <typename T, std::size_t N> struct is_array_helper<indirect_ndarray<T, N>> {
    static constexpr bool value = true;
};

template <typename It, typename F, std::size_t N> struct is_array_helper<unary_expr_ndarray<It, F, N>> {
    static constexpr bool value = true;
};

template <typename LhsIt, typename RhsIt, typename F, std::size_t N>
struct is_array_helper<binary_expr_ndarray<LhsIt, RhsIt, F, N>> {
    static constexpr bool value = true;
};

template <typename T> constexpr bool is_array() { return is_array_helper<T>::value; }

// Helpers for operators

template <typename It, typename F, std::size_t M>
unary_expr_ndarray<It, F, M> make_expr_ndarray(const It&, const It&, const F&, const std::array<std::size_t, M>&);

template <typename LhsIt, typename RhsIt, typename F, std::size_t M>
binary_expr_ndarray<LhsIt, RhsIt, F, M> make_expr_ndarray(const LhsIt&, const LhsIt&, const RhsIt&, const RhsIt&, const F&,
                                                          const std::array<std::size_t, M>&);

} // namespace detail

// Friend classes

template <typename> class base_column_view;
template <typename, typename, typename> class base_dataframe_colview;

} // namespace jules

#endif // JULES_ARRAY_DETAIL_COMMON_DECL_H
