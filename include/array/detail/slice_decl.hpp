#ifndef JULES_ARRAY_DETAIL_SLICE_DECL_H
#define JULES_ARRAY_DETAIL_SLICE_DECL_H

#include "array/detail/common.hpp"

namespace jules
{
namespace detail
{
template <std::size_t N> class base_slice
{
    static_assert(N > 0, "Invalid slice dimension.");

  public:
    base_slice() = default;

    base_slice(std::size_t start, std::initializer_list<std::size_t> extents);
    base_slice(std::size_t start, std::initializer_list<std::size_t> extents,
               std::initializer_list<std::size_t> strides);

    base_slice(std::size_t start, const std::array<std::size_t, N>& extents);
    base_slice(std::size_t start, const std::array<std::size_t, N>& extents,
               const std::array<std::size_t, N>& strides);

    base_slice(const base_slice& source) = default;
    base_slice(base_slice&& source) = default;

    base_slice& operator=(const base_slice& source) = default;
    base_slice& operator=(base_slice&& source) = default;

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    std::size_t operator()(Dims... dims) const;

    auto start() const { return start_; }
    auto size() const { return prod(extents_); }
    const auto& extents() const { return extents_; }
    const auto& strides() const { return strides_; }

    auto extent(std::size_t i) const { return extents_[i]; }
    auto stride(std::size_t i) const { return strides_[i]; }

    auto& start() { return start_; }
    auto& extents() { return extents_; }
    auto& strides() { return strides_; }

    auto& extent(std::size_t i) { return extents_[i]; }
    auto& stride(std::size_t i) { return strides_[i]; }

    base_slice_iterator<N> begin() const { return {*this, begin_index()}; }
    base_slice_iterator<N> end() const { return {*this, end_index()}; }

    base_slice_iterator<N> cbegin() const { return {*this, begin_index()}; }
    base_slice_iterator<N> cend() const { return {*this, end_index()}; }

  private:
    std::array<std::size_t, N> begin_index() const { return {}; }
    std::array<std::size_t, N> end_index() const { return {{extents_[0]}}; }

    std::size_t start_ = 0;
    std::array<std::size_t, N> extents_ = {};
    std::array<std::size_t, N> strides_ = {};
};

template <std::size_t N> class base_slice_iterator
{
  public:
    using value_type = std::size_t;
    using reference = std::size_t&;
    using pointer = std::size_t*;
    using iterator_category = std::forward_iterator_tag;

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    base_slice_iterator(const base_slice<N>& slice, Dims... indexes);
    base_slice_iterator(const base_slice<N>& slice, const std::array<std::size_t, N>& indexes);

    std::size_t operator*() const { return index(std::make_index_sequence<N>{}); }

    base_slice_iterator& operator++();
    base_slice_iterator operator++(int);
    bool operator==(const base_slice_iterator& other) const { return indexes_ == other.indexes_; }
    bool operator!=(const base_slice_iterator& other) const { return !(*this == other); }

  private:
    template <std::size_t... I> auto index(std::index_sequence<I...>) const { return slice_(indexes_[I]...); }

    const base_slice<N>& slice_;
    std::array<std::size_t, N> indexes_;
};

template <> class base_slice_iterator<1>;
template <> class base_slice<1>
{
  public:
    base_slice() = default;

    inline base_slice(std::size_t start, std::size_t extent, std::size_t stride = 1);

    inline base_slice(std::size_t start, std::initializer_list<std::size_t> extents);
    inline base_slice(std::size_t start, std::initializer_list<std::size_t> extents,
                      std::initializer_list<std::size_t> strides);

    inline base_slice(std::size_t start, const std::array<std::size_t, 1>& extents);
    inline base_slice(std::size_t start, const std::array<std::size_t, 1>& extents,
                      const std::array<std::size_t, 1>& strides);

    base_slice(const base_slice& source) = default;
    base_slice(base_slice&& source) = default;

    base_slice& operator=(const base_slice& source) = default;
    base_slice& operator=(base_slice&& source) = default;

    std::size_t operator()(std::size_t i) const { return start() + i * stride(); }

    std::size_t start() const { return start_; }
    std::size_t size() const { return extent_[0]; }
    std::size_t extent(std::size_t = 0) const { return extent_[0]; }
    std::size_t stride(std::size_t = 0) const { return stride_[0]; }

    const auto& extents() const { return extent_; }
    const auto& strides() const { return stride_; }

    std::size_t& start() { return start_; }
    std::size_t& size() { return extent_[0]; }
    std::size_t& extent(std::size_t = 0) { return extent_[0]; }
    std::size_t& stride(std::size_t = 0) { return stride_[0]; }

    inline base_slice_iterator<1> begin() const;
    inline base_slice_iterator<1> end() const;

    inline base_slice_iterator<1> cbegin() const;
    inline base_slice_iterator<1> cend() const;

  private:
    std::size_t start_ = 0;
    std::array<std::size_t, 1> extent_ = {{0}};
    std::array<std::size_t, 1> stride_ = {{1}};
};

template <> class base_slice_iterator<1>
{
  public:
    using value_type = std::size_t;
    using reference = std::size_t&;
    using pointer = std::size_t*;
    using iterator_category = std::forward_iterator_tag;

    base_slice_iterator() = default;
    inline base_slice_iterator(const base_slice<1>& slice, std::size_t i);

    std::size_t operator*() const { return index_; }

    inline base_slice_iterator& operator++();
    inline base_slice_iterator operator++(int);
    bool operator==(const base_slice_iterator& other) const { return index_ == other.index_; }
    bool operator!=(const base_slice_iterator& other) const { return !(*this == other); }

  private:
    std::size_t index_ = 0;
    std::size_t step_ = 1;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_SLICE_DECL_H
