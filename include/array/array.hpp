#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include "array/array_decl.hpp"
#include "util/numeric.hpp"

namespace jules
{
template <typename T, std::size_t N>
array<T, N>::array(const std::array<std::size_t, N>& dim)
    : dim_(dim), data_(prod(dim))
{
}

template <typename T, std::size_t N>
array<T, N>::array(const T* data, const std::array<std::size_t, N>& dim)
    : dim_(dim), data_(data, prod(dim))
{
}

template <typename T, std::size_t N>
template <typename Range, typename R>
array<T, N>::array(Range&& range, const std::array<std::size_t, N>& dim)
    : dim_(dim), data_(prod(dim))
{
    static_assert(std::is_assignable<T, R>::value, "invalid elements type in the range");

    auto it = std::begin(range);
    for (std::size_t i = 0; i < data_.size(); ++i)
        data_[i] = *it++;
}

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
