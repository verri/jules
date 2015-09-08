#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include "array/array_decl.hpp"
#include "range/range.hpp"

namespace jules
{
template <typename T>
ndarray<T, 1>::ndarray(std::size_t size) :
    detail::base_ndarray<T, 1>(std::array<T, 1>{size}) {}

template <typename T>
template <typename Range> ndarray<T, 1>::ndarray(Range&& range) :
    detail::base_ndarray<T, 1>(std::forward<Range>(range), {static_cast<std::size_t>(std::end(range) - std::begin(range))})
{
}
} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
