#ifndef JULES_ARRAY_DETAIL_SLICE_H
#define JULES_ARRAY_DETAIL_SLICE_H

#include "array/detail/slice_decl.hpp"

namespace jules
{
namespace detail
{
// Slice Iterator

template <std::size_t N>
template <typename... Dims, typename>
base_slice_iterator<N>::base_slice_iterator(const base_slice<N>& slice, Dims... indexes)
    : slice_{slice}, indexes_{{indexes...}}
{
}

template <std::size_t N>
base_slice_iterator<N>::base_slice_iterator(const base_slice<N>& slice,
                                            const std::array<std::size_t, N>& indexes)
    : slice_{slice}, indexes_(indexes)
{
}

template <std::size_t N> auto base_slice_iterator<N>::operator++() -> base_slice_iterator &
{
    auto i = N - 1;
    for (; i != 0; --i) {
        indexes_[i] = (indexes_[i] + 1) % slice_.extents(i);
        if (indexes_[i] != 0)
            break;
    }
    if (i == 0)
        ++indexes_[0];

    return *this;
}

template <std::size_t N> auto base_slice_iterator<N>::operator++(int) -> base_slice_iterator
{
    base_slice_iterator c = *this;
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
    : start_{start}, size_{prod(extents)}
{
    if (extents.size() != N || strides.size() != N)
        throw std::runtime_error{"invalid extents or strides"};

    std::copy(extents.begin(), extents.end(), std::begin(extents_));
    std::copy(strides.begin(), strides.end(), std::begin(strides_));
}

template <std::size_t N>
base_slice<N>::base_slice(std::size_t start, const std::array<std::size_t, N>& extents,
                          const std::array<std::size_t, N>& strides)
    : start_{start}, size_{prod(extents)}, extents_(extents), strides_(strides)
{
}

template <std::size_t N>
template <typename... Dims, typename>
std::size_t base_slice<N>::operator()(Dims... dims) const
{
    std::size_t indexes[N] = {dims...};
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides_), start_);
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_SLICE_H
