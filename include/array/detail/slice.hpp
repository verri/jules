#ifndef JULES_ARRAY_DETAIL_SLICE_H
#define JULES_ARRAY_DETAIL_SLICE_H

#include "array/detail/slice_decl.hpp"

#ifndef NDEBUG
#define CHECK_BOUNDS(I, MAX) \
    do {\
        if (I >= MAX)\
            throw std::out_of_range{"out of array limits"};\
    } while (false)
#else
#define CHECK_BOUNDS(I, MAX)
#endif // NDEBUG

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
    if (extents.size() > N)
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
    if (extents.size() > N || strides.size() > N)
        throw std::runtime_error{"invalid extents or strides"};

    std::copy(extents.begin(), extents.end(), std::begin(extents_));
    std::copy(strides.begin(), strides.end(), std::begin(strides_));
}

template <std::size_t N>
base_slice<N>::base_slice(std::size_t start, const std::array<std::size_t, N>& extents)
    : start_{start}, size_{prod(extents)}, extents_(extents)
{
    auto tmp = size_;
    for (std::size_t i = 0; i < N; ++i) {
        tmp /= extents_[i];
        strides_[i] = tmp;
    }
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

// Slicing Helpers

template <std::size_t N, typename... Args>
void start_slicing(base_slice<N>& result, Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");
    do_slice<0>(result, std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(base_slice<N>& result, std::size_t i, Args&&... args)
{
    static_assert(N - D - 1 == sizeof...(args), "Invalid number of arguments.");
    CHECK_BOUNDS(i, result.extents(D));

    result.start(result.start() + result.strides(D) * i);
    result.extents(D, 1);

    do_slice<D + 1>(result, std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(base_slice<N>& result, const base_slice<1>& slice, Args&&... args)
{
    static_assert(N - D - 1 == sizeof...(args), "Invalid number of arguments.");
    CHECK_BOUNDS(slice.start() + slice.extents(0) * slice.strides(0), result.extents(D));

    result.start(result.start() + result.strides(D) * slice.start());
    result.extents(D, slice.extents(0));
    result.strides(D, result.strides() * slice.strides());

    do_slice<D + 1>(result, std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N>
void do_slice(base_slice<N>&) {}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_SLICE_H
