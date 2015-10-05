#ifndef JULES_ARRAY_DETAIL_SLICE_H
#define JULES_ARRAY_DETAIL_SLICE_H

#include "array/detail/slice_decl.hpp"

#ifndef NDEBUG
#define CHECK_BOUNDS(I, MAX)                                                                                 \
    do {                                                                                                     \
        if (I >= MAX)                                                                                        \
            throw std::out_of_range{"out of array limits"};                                                  \
    } while (false)
#define CHECK_STRIDE(VALUE)                                                                                  \
    do {                                                                                                     \
        if (VALUE == 0)                                                                                      \
            throw std::out_of_range{"invalid stride"};                                                       \
    } while (false)
#else
#define CHECK_BOUNDS(I, MAX)
#define CHECK_STRIDE(VALUE)
#endif // NDEBUG

namespace jules
{
namespace detail
{
// Slice

template <std::size_t N>
base_slice<N>::base_slice(std::size_t start, std::initializer_list<std::size_t> extents)
    : start_{start}
{
#ifndef NDEBUG
    if (extents.size() > N)
        throw std::runtime_error{"invalid extents"};
#endif // NDEBUG

    std::copy(extents.begin(), extents.end(), std::begin(extents_));

    std::size_t tmp = this->size();
    for (std::size_t i = 0; i < N; ++i) {
        tmp /= extents_[i];
        strides_[i] = tmp;
    }
}

template <std::size_t N>
base_slice<N>::base_slice(std::size_t start, std::initializer_list<std::size_t> extents,
                          std::initializer_list<std::size_t> strides)
    : start_{start}
{
#ifndef NDEBUG
    if (extents.size() > N || strides.size() > N)
        throw std::runtime_error{"invalid extents or strides"};
#endif // NDEBUG

    std::copy(extents.begin(), extents.end(), std::begin(extents_));
    std::copy(strides.begin(), strides.end(), std::begin(strides_));
}

template <std::size_t N>
base_slice<N>::base_slice(std::size_t start, const std::array<std::size_t, N>& extents)
    : start_{start}, extents_(extents)
{
    auto tmp = this->size();
    for (std::size_t i = 0; i < N; ++i) {
        tmp /= extents_[i];
        strides_[i] = tmp;
    }
}

template <std::size_t N>
base_slice<N>::base_slice(std::size_t start, const std::array<std::size_t, N>& extents,
                          const std::array<std::size_t, N>& strides)
    : start_{start}, extents_(extents), strides_(strides)
{
}

template <std::size_t N>
template <typename... Dims, typename>
std::size_t base_slice<N>::operator()(Dims... dims) const
{
    std::size_t indexes[N] = {dims...};
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides_), start_);
}

template <std::size_t N>
std::size_t base_slice<N>::operator()(const std::array<std::size_t, N>& indexes) const
{
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides_), start_);
}

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
        indexes_[i] = (indexes_[i] + 1) % slice_.extent(i);
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

// Slice Specialization

inline base_slice<1>::base_slice(std::size_t start, std::size_t extent, std::size_t stride)
    : start_{start}, extent_{{extent}}, stride_{{stride}}
{
    CHECK_STRIDE(stride);
}

inline base_slice<1>::base_slice(std::size_t start, std::initializer_list<std::size_t> extents)
    : start_{start}, stride_{{1}}
{
#ifndef NDEBUG
    if (extents.size() > 1)
        throw std::runtime_error{"invalid extents"};
#endif // NDEBUG
    if (extents.size() > 0)
        extent_[0] = *extents.begin();
}

inline base_slice<1>::base_slice(std::size_t start, std::initializer_list<std::size_t> extents,
                                 std::initializer_list<std::size_t> strides)
    : start_{start}
{
#ifndef NDEBUG
    if (extents.size() > 1 || strides.size() > 1)
        throw std::runtime_error{"invalid extents or strides"};
#endif // NDEBUG
    if (extents.size() > 0)
        extent_[0] = *extents.begin();
    if (strides.size() > 0)
        stride_[0] = *strides.begin();
}

inline base_slice<1>::base_slice(std::size_t start, const std::array<std::size_t, 1>& extents)
    : start_{start}, extent_(extents), stride_{{1}}
{
}

inline base_slice<1>::base_slice(std::size_t start, const std::array<std::size_t, 1>& extents,
                                 const std::array<std::size_t, 1>& strides)
    : start_{start}, extent_(extents), stride_(strides)
{
}

inline base_slice_iterator<1> base_slice<1>::begin() const { return {*this, 0}; }
inline base_slice_iterator<1> base_slice<1>::end() const { return {*this, extent()}; }

inline base_slice_iterator<1> base_slice<1>::cbegin() const { return {*this, 0}; }
inline base_slice_iterator<1> base_slice<1>::cend() const { return {*this, extent()}; }

// Slice Iterator Specialization

inline base_slice_iterator<1>::base_slice_iterator(const base_slice<1>& slice, std::size_t i)
    : index_{slice.start() + i * slice.stride()}, step_{slice.stride()}
{
}

inline base_slice_iterator<1>& base_slice_iterator<1>::operator++()
{
    index_ += step_;
    return *this;
}

inline base_slice_iterator<1> base_slice_iterator<1>::operator++(int)
{
    auto c = *this;
    index_ += step_;
    return c;
}

// Slicing Helpers

template <std::size_t N, typename... Args>
base_slice<N> default_slicing(const base_slice<N>& source, Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");
    auto result = source;
    do_slice<0>(result, std::forward<Args>(args)...);
    return result;
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(base_slice<N>& result, std::size_t i, Args&&... args)
{
    static_assert(N - D - 1 == sizeof...(args), "Invalid number of arguments.");

    CHECK_BOUNDS(i, result.extent(D));

    result.start() += result.stride(D) * i;
    result.extent(D) = 1;
    result.stride(D) = 1;

    do_slice<D + 1>(result, std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(base_slice<N>& result, const base_slice<1>& slice, Args&&... args)
{
    static_assert(N - D - 1 == sizeof...(args), "Invalid number of arguments.");

    CHECK_STRIDE(slice.stride());
    CHECK_BOUNDS(slice.start() + slice.extent() * slice.stride(), result.extent(D));

    result.start() += result.stride(D) * slice.start();
    result.stride(D) = result.stride(D) * slice.stride();
    result.extent(D) =
        slice.extent() > 0 ? slice.extent() : (result.extent(D) - slice.start()) / slice.stride();

    do_slice<D + 1>(result, std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N> void do_slice(base_slice<N>&)
{
    static_assert(N == D, "Invalid number of arguments.");
}

// Indirect Slicing Helpers

template <std::size_t N, typename... Args>
std::pair<std::array<std::size_t, N>, std::vector<std::size_t>> indirect_slicing(const base_slice<N>& slice,
                                                                                 Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");

    std::pair<std::array<std::size_t, N>, std::vector<std::size_t>> result;
    do_slice(result.first, result.second, slice, std::array<std::size_t, 0>{}, std::forward<Args>(args)...);

    return result;
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(std::array<std::size_t, N>& extents, std::vector<std::size_t>& indexes,
              const base_slice<N>& slice, std::array<std::size_t, D> ix, std::size_t i, Args&&... args)
{
    static_assert(N - D - 1 == sizeof...(args), "Invalid number of arguments.");

    CHECK_BOUNDS(i, slice.extent(D));

    extents[D] = 1;
    do_slice(extents, indexes, slice, cat(ix, i), std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(std::array<std::size_t, N>& extents, std::vector<std::size_t>& indexes,
              const base_slice<N>& slice, std::array<std::size_t, D> ix, const base_slice<1>& rng_base,
              Args&&... args)
{
    static_assert(N - D - 1 == sizeof...(args), "Invalid number of arguments.");

    CHECK_STRIDE(rng_base.stride());
    CHECK_BOUNDS(rng_base.start() + rng_base.extent() * rng_base.stride(), slice.extent(D));

    auto rng = rng_base;
    if (rng.size() == 0)
        rng = base_slice<1>{rng_base.start(), slice.extent(D), rng_base.stride()};

    extents[D] = rng.size();

    for (std::size_t i : rng)
        do_slice(extents, indexes, slice, cat(ix, i), std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename Range, typename... Args, typename>
void do_slice(std::array<std::size_t, N>& extents, std::vector<std::size_t>& indexes,
              const base_slice<N>& slice, std::array<std::size_t, D> ix, const Range& rng, Args&&... args)
{
    static_assert(N - D - 1 == sizeof...(args), "Invalid number of arguments.");

    // CHECK_BOUNDS(max(rng), slice.extent(D)); TODO fix max for ref_ndarray

    extents[D] = range::size(rng);

    for (std::size_t i : rng)
        do_slice(extents, indexes, slice, cat(ix, i), std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(std::array<std::size_t, N>&, std::vector<std::size_t>& indexes, const base_slice<N>& slice,
              std::array<std::size_t, D> ix)
{
    static_assert(D == N, "Invalid number of arguments.");
    indexes.push_back(slice(ix));
}

} // namespace detail
} // namespace jules

#undef CHECK_BOUNDS
#undef CHECK_STRIDE

#endif // JULES_ARRAY_DETAIL_SLICE_H
