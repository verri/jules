// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_ABSOLUTE_H
#define JULES_ARRAY_SLICING_ABSOLUTE_H

#include <jules/core/type.hpp>

namespace jules
{
inline namespace slicing
{

struct absolute_strided_slice
{
  absolute_strided_slice() = delete;

  constexpr absolute_strided_slice(index_t start, index_t extent, index_t stride) noexcept
    : start{start}, extent{extent}, stride{stride}
  {}

  index_t start;
  index_t extent;
  index_t stride;
};

struct absolute_slice
{

  absolute_slice() = delete;

  constexpr absolute_slice(index_t start, index_t extent) noexcept : start{start}, extent{extent} {}

  constexpr operator absolute_strided_slice() const noexcept { return {start, extent, 1u}; }

  index_t start;
  index_t extent;
};

constexpr auto slice(index_t start, index_t extent) noexcept -> absolute_slice { return {start, extent}; }

constexpr auto slice(index_t start, index_t extent, index_t stride) noexcept -> absolute_strided_slice
{
  return {start, extent, stride};
}

constexpr auto eval(absolute_slice slice, index_t) noexcept -> absolute_slice { return slice; }

constexpr auto eval(absolute_strided_slice slice, index_t) noexcept -> absolute_strided_slice { return slice; }

} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_ABSOLUTE_H
