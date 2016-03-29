#ifndef JULES_ARRAY_ARRAY_DECL_H
#define JULES_ARRAY_ARRAY_DECL_H

#include <jules/array/detail/all.hpp>

namespace jules
{
template <typename T, std::size_t N> using ndarray = detail::base_ndarray<T, N>;

template <typename T> using matrix = ndarray<T, 2>;
template <typename T> using vector = ndarray<T, 1>;
template <typename T> using scalar = ndarray<T, 0>;

template <typename T, typename Range, typename = detail::range_type_enabler<Range, T>> vector<T> to_vector(const Range& range);

template <typename Range, typename R = range_value_t<Range>> vector<R> as_vector(const Range& range);

static inline detail::base_slice<1> slice(std::size_t start = 0, std::size_t size = 0, std::size_t stride = 1);
static inline vector<std::size_t> seq(std::size_t start, std::size_t stop, std::size_t step = 1);

} // namespace jules

#endif // JULES_ARRAY_ARRAY_DECL_H
