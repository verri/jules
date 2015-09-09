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

template <typename R, typename S, std::size_t M>
base_ndarray<bool, M> operator==(const base_ndarray<R, M>& lhs, const base_ndarray<S, M>& rhs)
{
    if (!all(lhs.dim_, rhs.dim_))
        throw std::runtime_error{"array dimensions mismatch"};
    return {lhs.dim_, lhs.data_ == rhs.data_};
}

template <typename R, typename S, std::size_t M>
base_ndarray<bool, M> operator==(const base_ndarray<R, M>& lhs, const S& rhs)
{
    return {lhs.dim_, lhs.data_ == rhs};
}

template <typename R, typename S, std::size_t M>
base_ndarray<bool, M> operator==(const R& lhs, const base_ndarray<S, M>& rhs)
{
    return {rhs.dim_, lhs == rhs.data_};
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_H
