// Copyright (c) 2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_SEQUENCE_H
/// \exclude
#define JULES_BASE_SEQUENCE_H

#include <jules/base/const_vector.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

inline auto seq(const index_t start, const index_t stop, const distance_t step = 1) -> const_vector<index_t>
{
  DEBUG_ASSERT(step != 0, debug::default_module, debug::level::invalid_argument, "step cannot be zero");
  DEBUG_ASSERT(start <= stop || step < 0, debug::default_module, debug::level::invalid_argument, "stop value is unreachable");
  DEBUG_ASSERT(start >= stop || step > 0, debug::default_module, debug::level::invalid_argument, "stop value is unreachable");

  const auto size = static_cast<index_t>((static_cast<distance_t>(stop) - static_cast<distance_t>(start)) / step + 1);

  std::vector<index_t> indexes;
  indexes.reserve(size);

  for (auto i = start; indexes.size() < size; i += step)
    indexes.push_back(i);

  return {std::move(indexes)};
}

} // namespace jules

#endif // JULES_BASE_SEQUENCE_H
