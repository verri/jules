// Copyright (c) 2019-2020 Filipe Verri <filipeverri@gmail.com>

// Oh how I love your law!
//    It is my meditation all the day.
// Psalm 119:97 (ESV)

#ifndef JULES_BASE_INDEX_SET_H
#define JULES_BASE_INDEX_SET_H

#include "jules/core/meta.hpp"
#include "range/v3/range/traits.hpp"
#include <algorithm>
#include <iterator>
#include <jules/base/const_vector.hpp>
#include <jules/base/sequence.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>
#include <type_traits>
#include <utility>

namespace jules
{

inline auto is_index_set(const const_vector<index_t>& ix) -> bool
{
  if (ix.size() == 0)
    return true;

  auto it = ix.begin();
  index_t cur = *it++;
  while (it != ix.end()) {
    const auto prev = std::exchange(cur, *it++);
    if (cur <= prev)
      return false;
  }
  return true;
}

class index_set : public const_vector<index_t>
{
public:
  index_set() = default;

  index_set(const_vector<index_t> data) : const_vector<index_t>{data}
  {
    DEBUG_ASSERT(is_index_set(data), debug::default_module, debug::level::invalid_argument, "indices are not a set.");
  }
};

constexpr auto is_index_set(const index_set&) -> bool { return true; }

template <ranges::range Rng> auto set_select(const index_set& set, const Rng& ix) -> index_set
{
  static_assert(std::is_convertible_v<ranges::range_value_t<Rng>, index_t>);

  DEBUG_ASSERT(is_index_set(ix), debug::default_module, debug::level::invalid_argument, "subset is not a index set");
  const auto size = ranges::size(ix);
  if (size == 0)
    return {};

  DEBUG_ASSERT(*--ranges::end(ix) < set.size(), debug::default_module, debug::level::boundary_check, "out of bounds");

  std::vector<index_t> result;
  result.reserve(size);

  for (const auto i : ix)
    result.push_back(set[i]);

  return {{std::move(result)}};
}

inline auto set_union(const index_set& x, const index_set& y) -> index_set
{
  std::vector<index_t> result;
  result.reserve(std::max(x.size(), y.size())); // underestimate (at most 1 reallocation)
  std::set_union(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result));
  return {{std::move(result)}};
}

inline auto set_intersection(const index_set& x, const index_set& y) -> index_set
{
  std::vector<index_t> result;
  result.reserve(std::min(x.size(), y.size())); // overestimate (at most one reallocation to fit)
  std::set_intersection(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result));
  result.shrink_to_fit();
  return {{std::move(result)}};
}

inline auto set_difference(const index_set& x, const index_set& y) -> index_set
{
  std::vector<index_t> result;
  result.reserve(x.size()); // overestimate (at most one reallocation to fit)
  std::set_difference(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result));
  result.shrink_to_fit();
  return {{std::move(result)}};
}

} // namespace jules

#endif // JULES_BASE_INDEX_SET_H
