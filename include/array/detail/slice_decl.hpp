#ifndef JULES_ARRAY_DETAIL_SLICE_DECL_H
#define JULES_ARRAY_DETAIL_SLICE_DECL_H

#include "array/detail/utility.hpp"

#include <array>

namespace jules
{
namespace detail
{
template <std::size_t N> class base_slice;

template <std::size_t N> class base_slice_iterator
{
  public:
    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    base_slice_iterator(const base_slice<N>& slice, Dims... indexes);
    base_slice_iterator(const base_slice<N>& slice, const std::array<std::size_t, N>& indexes);

    std::size_t operator*() const { return index(std::make_index_sequence<N>{}); }

    base_slice_iterator& operator++();
    base_slice_iterator operator++(int) const;
    bool operator==(const base_slice_iterator& other) const { return indexes_ == other.indexes_; }
    bool operator!=(const base_slice_iterator& other) const { return !(*this == other); }

  private:
    template <std::size_t... I> auto index(std::index_sequence<I...>) const { return slice_(indexes_[I]...); }

    const base_slice<N>& slice_;
    std::array<std::size_t, N> indexes_;
};

template <std::size_t N> class base_slice
{
    static_assert(N > 0, "invalid slice dimension");

  public:
    base_slice() = default;

    base_slice(std::size_t start, std::initializer_list<std::size_t> extents);
    base_slice(std::size_t start, std::initializer_list<std::size_t> extents,
               std::initializer_list<std::size_t> strides);
    base_slice(std::size_t start, const std::array<std::size_t, N>& extents,
               const std::array<std::size_t, N>& strides);

    base_slice(const base_slice& source) = default;
    base_slice(base_slice&& source) = default;

    base_slice& operator=(const base_slice& source) = default;
    base_slice& operator=(base_slice&& source) = default;

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    std::size_t operator()(Dims... dims) const;

    auto start() const { return start_; }
    auto size() const { return size_; }
    const auto& extents() const { return extents_; }
    const auto& strides() const { return strides_; }

    const auto& extents(std::size_t i) const { return extents_[i]; }
    const auto& strides(std::size_t i) const { return strides_[i]; }

    base_slice_iterator<N> begin() const { return {*this, begin_index()}; }
    base_slice_iterator<N> end() const { return {*this, end_index()}; }

    base_slice_iterator<N> cbegin() const { return {*this, begin_index()}; }
    base_slice_iterator<N> cend() const { return {*this, end_index()}; }

  private:
    std::array<std::size_t, N> begin_index() const { return {}; }
    std::array<std::size_t, N> end_index() const { return {{extents_[0]}}; }

    std::size_t start_ = 0;
    std::size_t size_ = 0;
    std::array<std::size_t, N> extents_ = {};
    std::array<std::size_t, N> strides_ = {};
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_SLICE_DECL_H
