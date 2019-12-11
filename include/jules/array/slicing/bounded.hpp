// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_BOUNDED_H
#define JULES_ARRAY_SLICING_BOUNDED_H

#include <jules/array/detail/common.hpp>
#include <jules/array/slicing/absolute.hpp>
#include <jules/core/type.hpp>

#include <algorithm>

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

  constexpr auto operator()(index_t size) const noexcept { return std::min(maximum_, size); }

  constexpr bounded_index(every_index) noexcept : maximum_{numeric_traits<index_t>::max()} {}

  constexpr bounded_index(const bounded_index&) noexcept = default;
  constexpr bounded_index(bounded_index&&) noexcept = default;

  constexpr auto operator=(const bounded_index&) noexcept -> bounded_index& = default;
  constexpr auto operator=(bounded_index&&) noexcept -> bounded_index& = default;

private:
  constexpr bounded_index(tag<bounded_index>, index_t maximum) noexcept : maximum_{maximum} {}
  index_t maximum_;
};

constexpr auto at_most(index_t maximum) noexcept -> bounded_index { return {{}, maximum}; }

struct bounded_strided_slice
{

  bounded_strided_slice() = delete;

  constexpr bounded_strided_slice(index_t start, bounded_index extent, index_t stride) noexcept
    : start{start}, extent{extent}, stride{stride}
  {}

  index_t start;
  bounded_index extent;
  index_t stride;
};

struct bounded_slice
{

  bounded_slice() = delete;

  constexpr bounded_slice(index_t start, bounded_index extent) noexcept : start{start}, extent{extent} {}

  constexpr operator bounded_strided_slice() const noexcept { return {start, extent, 1u}; }

  index_t start;
  bounded_index extent;
};

constexpr auto slice(index_t start, bounded_index extent) noexcept -> bounded_slice { return {start, extent}; }

constexpr auto slice(index_t start, bounded_index extent, index_t stride) noexcept -> bounded_strided_slice
{
  return {start, extent, stride};
}

constexpr auto eval(bounded_slice slice, index_t dim) noexcept -> absolute_slice
{
  return {slice.start, slice.extent(slice.start < dim ? dim - slice.start : 0u)};
}

constexpr auto eval(bounded_strided_slice slice, index_t dim) noexcept -> absolute_strided_slice
{
  const auto size = slice.start < dim ? detail::seq_size(slice.start, dim, slice.stride) : 0u;
  return {slice.start, slice.extent(size), slice.stride};
}

} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_BOUNDED_H
