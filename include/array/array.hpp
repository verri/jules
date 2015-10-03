#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include "array/array_decl.hpp"

namespace jules
{
template <typename T, typename Range, typename> vector<T> to_vector(const Range& rng)
{
    vector<T> vec(range::size(rng));
    range::copy(rng, vec.begin());

    return vec;
}

template <typename Range, typename R> vector<R> as_vector(const Range& rng) { return to_vector<R>(rng); }

static inline detail::base_slice<1> slice(std::size_t start, std::size_t size, std::size_t stride)
{
    return {start, size, stride};
}

static inline vector<std::size_t> seq(std::size_t start, std::size_t stop, std::size_t step)
{
    std::size_t size = (start < stop ? stop - start : start - stop) / step;
    vector<std::size_t> indexes(size);

    if (start < stop) {
        for (std::size_t i = 0; i < size; ++i) {
            indexes[i] = start;
            start += step;
        }
    } else {
        for (std::size_t i = 0; i < size; ++i) {
            indexes[i] = start;
            start -= step;
        }
    }

    return indexes;
}

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
