// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_BOUNDED_H
#define JULES_ARRAY_SLICING_BOUNDED_H

#include <jules/core/type.hpp>

#include <algorithm>

namespace jules
{
inline namespace slicing
{

struct all_index {
};

constexpr auto all = all_index{};

class bounded_index
{
public:
  friend constexpr auto at_most(index_t maximum) noexcept -> bounded_index { return {{}, maximum}; }

  constexpr auto operator()(index_t size) const noexcept { return std::min(maximum_, size); }

  constexpr bounded_index(all_index) noexcept : maximum_{numeric_traits<index_t>::max()} {}

  constexpr bounded_index(const bounded_index&) noexcept = default;
  constexpr bounded_index(bounded_index&&) noexcept = default;

  constexpr auto operator=(const bounded_index&) noexcept -> bounded_index& = default;
  constexpr auto operator=(bounded_index&&) noexcept -> bounded_index& = default;

private:
  constexpr bounded_index(tag<bounded_index>, index_t maximum) noexcept : maximum_{maximum} {}
  index_t maximum_;
};

struct bounded_slice {
  index_t start;
  bounded_index extent;
};

struct bounded_strided_slice {
  index_t start;
  bounded_index extent;
  index_t stride;
};

auto slice(index_t start, bounded_index extent) -> bounded_slice { return {start, extent}; }

auto slice(index_t start, bounded_index extent, index_t stride) -> bounded_strided_slice { return {start, extent, stride}; }
} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_BOUNDED_H
