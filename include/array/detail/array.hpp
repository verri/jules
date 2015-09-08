#ifndef JULES_ARRAY_DETAIL_ARRAY_H
#define JULES_ARRAY_DETAIL_ARRAY_H

#include "array/detail/array_decl.hpp"
#include "range/range.hpp"
#include "util/numeric.hpp"

namespace jules
{
namespace detail
{
template <typename T, std::size_t N>
base_ndarray<T, N>::base_ndarray(const std::array<std::size_t, N>& dim)
    : dim_(dim), data_(prod(dim))
{
}

template <typename T, std::size_t N>
base_ndarray<T, N>::base_ndarray(const T* data, const std::array<std::size_t, N>& dim)
    : dim_(dim), data_(data, prod(dim))
{
}

template <typename T, std::size_t N>
base_ndarray<T, N>::base_ndarray(const T& value, const std::array<std::size_t, N>& dim)
    : dim_(dim), data_(value, prod(dim))
{
}

template <typename T, std::size_t N>
template <typename Range>
base_ndarray<T, N>::base_ndarray(Range&& range, const std::array<std::size_t, N>& dim)
    : dim_(dim), data_(prod(dim))
{
    using R = typename range_value<Range>::type;
    static_assert(std::is_assignable<T, R>::value, "invalid elements type in the range");

    std::copy(std::begin(std::forward<Range>(range)),
              std::begin(std::forward<Range>(range)) + data_.size(),
              std::begin(data_));
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_H
