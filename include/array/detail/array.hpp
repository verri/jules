#ifndef JULES_ARRAY_DETAIL_ARRAY_H
#define JULES_ARRAY_DETAIL_ARRAY_H

#include "array/detail/array_decl.hpp"

namespace jules
{
namespace detail
{
// Slice Iterator

template <std::size_t N> auto base_slice<N>::iterator::operator++() -> iterator &
{
    for (auto i = N; i != 0; --i) {
        indexes[i - 1] = (indexes[i - 1] + 1) % slice->extents()[i - 1];
        if (indexes[i - 1] != 0)
            break;
    }

    return *this;
}

template <std::size_t N> auto base_slice<N>::iterator::operator++(int) const -> iterator
{
    iterator c = *this;
    ++(*this);
    return c;
}

// Slice

template <std::size_t N>
base_slice<N>::base_slice(std::size_t start, std::initializer_list<std::size_t> extents)
    : start_{start}
{
    if (extents.size() != N)
        throw std::runtime_error{"invalid extents"};

    std::copy(extents.begin(), extents.end(), std::begin(extents_));

    std::size_t tmp = prod(extents_);
    size_ = tmp;

    for (std::size_t i = 0; i < N; ++i) {
        tmp /= extents_[i];
        strides_[i] = tmp;
    }
}

template <std::size_t N>
base_slice<N>::base_slice(std::size_t start, std::initializer_list<std::size_t> extents,
                          std::initializer_list<std::size_t> strides)
    : start_{start}, size_{prod(extents_)}
{
    if (extents.size() != N || strides.size() != N)
        throw std::runtime_error{"invalid extents or strides"};

    std::copy(extents.begin(), extents.end(), std::begin(extents_));
    std::copy(strides.begin(), strides.end(), std::begin(strides_));
}

template <std::size_t N>
template <typename... Dims, typename>
std::size_t base_slice<N>::operator()(Dims... dims) const
{
    std::size_t indexes[] = {dims...};
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides_), start_);
}

template <typename T, std::size_t N> base_ndarray<T, N>::base_ndarray() : ref_ndarray<T, N>{nullptr, {}} {}

template <typename T, std::size_t N> base_ndarray<T, N>::~base_ndarray()
{
    for (auto it = data(); it != data() + size(); ++it)
        it->~T();
    delete[] reinterpret_cast<storage_t*>(data());
}

// Reference Array

// Base Array

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
