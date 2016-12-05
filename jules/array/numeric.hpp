#ifndef JULES_ARRAY_NUMERIC_H
#define JULES_ARRAY_NUMERIC_H

#include <jules/array/array.hpp>
#include <jules/array/detail/slicing.hpp>
#include <jules/core/type.hpp>

namespace jules
{

static inline auto seq(index_t start, index_t stop, index_t step = 1u) -> vector<index_t>
{
  index_t size = detail::seq_size(start, stop, step);
  vector<index_t> indexes(size);

  if (start < stop) {
    for (index_t i = 0; i < size; ++i) {
      indexes[i] = start;
      start += step;
    }
  } else {
    for (index_t i = 0; i < size; ++i) {
      indexes[i] = start;
      start -= step;
    }
  }

  return indexes;
}
}

#endif // JULES_ARRAY_NUMERIC_H
