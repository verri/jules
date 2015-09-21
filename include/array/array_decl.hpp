#ifndef JULES_ARRAY_ARRAY_DECL_H
#define JULES_ARRAY_ARRAY_DECL_H

#include "array/detail/all.hpp"

namespace jules
{
template <typename T, std::size_t N> using ndarray = detail::base_ndarray<T, N>;

template <typename T> using matrix = ndarray<T, 2>;
template <typename T> using vector = ndarray<T, 1>;
template <typename T> using scalar = ndarray<T, 0>;

template <typename T, typename Range, typename = detail::range_type_enabler<Range, T>>
vector<T> to_vector(const Range& range);

template <typename Range, typename R = typename std::remove_reference<Range>::type::value_type>
vector<R> as_vector(const Range& range);

} // namespace jules

#endif // JULES_ARRAY_ARRAY_DECL_H
