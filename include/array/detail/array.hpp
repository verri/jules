#ifndef JULES_ARRAY_DETAIL_ARRAY_H
#define JULES_ARRAY_DETAIL_ARRAY_H

#include "array/detail/array_decl.hpp"

namespace jules
{
namespace detail
{

// Reference Array

// Base Array

template <typename T, std::size_t N> base_ndarray<T, N>::base_ndarray() : ref_ndarray<T, N>{nullptr, {}} {}

template <typename T, std::size_t N> base_ndarray<T, N>::~base_ndarray()
{
    for (auto it = data(); it != data() + size(); ++it)
        it->~T();
    delete[] reinterpret_cast<storage_t*>(data());
}

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(Dims... dims)
    : ref_ndarray<T, N>{reinterpret_cast<T*>(new storage_t[prod_args(dims...)]), {0, {std::size_t(dims)...}}}
{
    for (auto it = data(); it != data() + size(); ++it)
        new (it) T;
}

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(const T& value, Dims... dims)
    : ref_ndarray<T, N>{reinterpret_cast<T*>(new storage_t[prod_args(dims...)]), {0, {std::size_t(dims)...}}}
{
    for (auto it = data(); it != data() + size(); ++it)
        new (it) T(value);
}

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(const T* data, Dims... dims)
    : ref_ndarray<T, N>{reinterpret_cast<T*>(new storage_t[prod_args(dims...)]), {0, {std::size_t(dims)...}}}
{
    std::copy(data, data + this->size(), this->data_);
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_H
