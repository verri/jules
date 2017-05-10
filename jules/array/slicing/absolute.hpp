// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_ABSOLUTE_H
#define JULES_ARRAY_SLICING_ABSOLUTE_H

#include <jules/core/type.hpp>

namespace jules
{
inline namespace slicing
{

struct absolute_slice {
  index_t start;
  index_t extent;
};

struct absolute_strided_slice {
  index_t start;
  index_t extent;
  index_t stride;
};

auto slice(index_t start, index_t extent) -> absolute_slice { return {start, extent}; }

auto slice(index_t start, index_t extent, index_t stride) -> absolute_strided_slice { return {start, extent, stride}; }
} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_ABSOLUTE_H
