#ifndef JULES_ARRAY_DETAIL_ARRAY_H
#define JULES_ARRAY_DETAIL_ARRAY_H

#include "array/detail/array_decl.hpp"
#include "util/numeric.hpp"

namespace jules
{
namespace detail
{
template <typename T, std::size_t N>
template <typename... Args>
base_ndarray<T, N>::base_ndarray(const std::array<std::size_t, N>& dim, Args&&... args)
    : dim_(dim), data_(std::forward<Args>(args)...)
{
}

template <typename T, std::size_t N>
base_ndarray<T, N>::base_ndarray(const std::array<std::size_t, N>& dim)
    : dim_(dim), data_(prod(dim))
{
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_H
