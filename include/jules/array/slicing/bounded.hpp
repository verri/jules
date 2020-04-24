// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_BOUNDED_H
#define JULES_ARRAY_SLICING_BOUNDED_H

#include <jules/array/detail/common.hpp>
#include <jules/array/slicing/absolute.hpp>
#include <jules/core/type.hpp>

namespace jules
{
inline namespace slicing
{

struct every_index
{};

constexpr auto every = every_index{};

constexpr auto slice() noexcept { return every; }

class bounded_index
{
public:
  friend constexpr auto at_most(index_t maximum) noexcept -> bounded_index;

  [[nodiscard]] constexpr auto bound(index_t size) const noexcept { return size < maximum_ ? size : maximum_; }

  constexpr bounded_index(every_index) noexcept : maximum_{numeric_traits<index_t>::max()} {}

  constexpr bounded_index(const bounded_index&) noexcept = default;
  constexpr bounded_index(bounded_index&&) noexcept = default;

  constexpr auto operator=(const bounded_index&) noexcept -> bounded_index& = default;
  constexpr auto operator=(bounded_index&&) noexcept -> bounded_index& = default;

private:
  constexpr bounded_index(tag<bounded_index>, index_t maximum) noexcept : maximum_{maximum} {}
  index_t maximum_;
};

constexpr auto at_most(index_t maximum) noexcept -> bounded_index { return {tag<bounded_index>{}, maximum}; }

class bounded_strided_slice
{
public:
  bounded_strided_slice() = delete;

  constexpr bounded_strided_slice(index_t start, bounded_index extent, index_t stride) noexcept
    : start_{start}, extent_{extent}, stride_{stride}
  {}

  [[nodiscard]] constexpr auto start() const noexcept { return start_; }
  [[nodiscard]] constexpr auto extent() const noexcept { return extent_; }
  [[nodiscard]] constexpr auto stride() const noexcept { return stride_; }

private:
  index_t start_;
  bounded_index extent_;
  index_t stride_;
};

struct bounded_slice
{
public:
  bounded_slice() = delete;

  constexpr bounded_slice(index_t start, bounded_index extent) noexcept : start_{start}, extent_{extent} {}

  constexpr operator bounded_strided_slice() const noexcept { return {start_, extent_, 1u}; }

  [[nodiscard]] constexpr auto start() const noexcept { return start_; }
  [[nodiscard]] constexpr auto extent() const noexcept { return extent_; }

private:
  index_t start_;
  bounded_index extent_;
};

constexpr auto slice(index_t start, bounded_index extent) noexcept -> bounded_slice { return {start, extent}; }

constexpr auto slice(index_t start, bounded_index extent, index_t stride) noexcept -> bounded_strided_slice
{
  return {start, extent, stride};
}

template <> struct slice_traits<bounded_slice>
{
  using absolute_type = absolute_slice;
  struct absolutize_type
  {
    constexpr auto operator()(bounded_slice slice, index_t dim) const noexcept -> absolute_type
    {
      return {slice.start(), slice.extent().bound(slice.start() < dim ? dim - slice.start() : 0u)};
    }
  };
};

template <> struct slice_traits<bounded_strided_slice>
{
  using absolute_type = absolute_strided_slice;
  struct absolutize_type
  {
    constexpr auto operator()(bounded_strided_slice slice, index_t dim) const noexcept -> absolute_type
    {
      const auto size = slice.start() < dim ? detail::seq_size(slice.start(), dim, slice.stride()) : 0u;
      return {slice.start(), slice.extent().bound(size), slice.stride()};
    }
  };
};

} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_BOUNDED_H
