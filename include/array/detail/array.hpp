#ifndef JULES_ARRAY_DETAIL_ARRAY_H
#define JULES_ARRAY_DETAIL_ARRAY_H

#include "array/detail/array_decl.hpp"

namespace jules
{
namespace detail
{
// Reference Array

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const U& source)
{
    for (std::size_t i : descriptor_)
        data_[i] = source;
    return *this;
}

template <typename T, std::size_t N> ref_ndarray<T, N - 1> ref_ndarray<T, N>::operator[](std::size_t i)
{
    auto start = descriptor_.start() + descriptor_.extents(0) * i;
    std::array<std::size_t, N - 1> extents, strides;

    const auto& e = descriptor_.extents();
    const auto& s = descriptor_.strides();

    std::copy(e.begin() + 1, e.end(), extents.begin());
    std::copy(s.begin() + 1, s.end(), strides.begin());

    return {data_, {start, extents, strides}};
}

template <typename T, std::size_t N>
ref_ndarray<const T, N - 1> ref_ndarray<T, N>::operator[](std::size_t i) const
{
    auto start = descriptor_.start() + descriptor_.extents(0) * i;
    std::array<std::size_t, N - 1> extents, strides;

    const auto& e = descriptor_.extents();
    const auto& s = descriptor_.strides();

    std::copy(e.begin() + 1, e.end(), extents.begin());
    std::copy(s.begin() + 1, s.end(), strides.begin());

    return {data_, {start, extents, strides}};
}

// Iterator


template <typename T, std::size_t N>
auto ref_ndarray_iterator<T, N>::operator++() -> ref_ndarray_iterator&
{
    ++index_;
    return *this;
}

template <typename T, std::size_t N>
auto ref_ndarray_iterator<T, N>::operator++(int) -> ref_ndarray_iterator
{
    ref_ndarray_iterator copy = *this;
    ++index_;
    return copy;
}

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
