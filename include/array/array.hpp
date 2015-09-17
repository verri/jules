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

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
