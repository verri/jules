// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_ABSOLUTE_H
#define JULES_ARRAY_SLICING_ABSOLUTE_H

#include <jules/core/type.hpp>

namespace jules
{
inline namespace slicing
{

struct absolute_slice {

  absolute_slice() = delete;

  constexpr absolute_slice(index_t start, index_t extent) noexcept : start{start}, extent{extent} {}

  index_t start;
  index_t extent;
};

struct absolute_strided_slice {
  absolute_strided_slice() = delete;

  constexpr absolute_strided_slice(index_t start, index_t extent, index_t stride) noexcept
    : start{start}, extent{extent}, stride{stride}
  {
  }

  index_t start;
  index_t extent;
  index_t stride;
};

constexpr auto slice(index_t start, index_t extent) noexcept -> absolute_slice { return {start, extent}; }

constexpr auto slice(index_t start, index_t extent, index_t stride) noexcept -> absolute_strided_slice
{
  return {start, extent, stride};
}
} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_ABSOLUTE_H
