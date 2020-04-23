// Copyright (c) 2019-2020 Filipe Verri <filipeverri@gmail.com>

// Oh how I love your law!
//    It is my meditation all the day.
// Psalm 119:97 (ESV)

#ifndef JULES_BASE_INDEX_SET_H
/// \exclude
#define JULES_BASE_INDEX_SET_H

#include <jules/base/index_span.hpp>
#include <jules/base/sequence.hpp>
#include <jules/core/concepts.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

namespace jules
{

template <ranges::range Rng> auto is_index_set(const Rng& ix) -> bool
{
  static_assert(std::is_convertible_v<ranges::range_value_t<Rng>, index_t>);

  if (ranges::size(ix) == 0)
    return true;

  auto it = ranges::begin(ix);
  index_t cur = *it++;
  while (it != ix.end()) {
    const auto prev = std::exchange(cur, *it++);
    if (cur <= prev)
      return false;
  }
  return true;
}

template <ranges::range RngX, ranges::range RngY, container_for<index_t> C = container<index_t>>
auto set_select(const RngX& x, const RngY& y, C c = container<index_t>{}) -> C
{
  static_assert(std::is_convertible_v<ranges::range_value_t<RngX>, index_t>);
  static_assert(std::is_convertible_v<ranges::range_value_t<RngY>, index_t>);

  DEBUG_ASSERT(is_index_set(x), debug::default_module, debug::level::invalid_argument, "argument is not a index set");
  DEBUG_ASSERT(is_index_set(y), debug::default_module, debug::level::invalid_argument, "argument is not a index set");

  const auto sx = ranges::size(x);
  const auto sb = ranges::size(y);

  if (sx == 0 || sb == 0)
    return c;

  DEBUG_ASSERT(*--ranges::end(y) < sx, debug::default_module, debug::level::boundary_check, "out of bounds");

  c.reserve(c.size() + sb);

  for (const auto i : y)
    c.push_back(x[i]);

  return c;
}

template <ranges::range RngX, ranges::range RngY, container_for<index_t> C = container<index_t>>
auto set_union(const RngX& x, const RngY& y, C c = container<index_t>{}) -> C
{
  c.reserve(c.size() + std::max(x.size(), y.size())); // underestimate (at most 1 reallocation)
  std::set_union(ranges::begin(x), ranges::end(x), ranges::begin(y), ranges::end(y), ranges::back_inserter(c));
  return c;
}

template <ranges::range RngX, ranges::range RngY, container_for<index_t> C = container<index_t>>
auto set_intersection(const RngX& x, const RngY& y, C c = container<index_t>{}) -> C
{
  c.reserve(c.size() + std::min(x.size(), y.size())); // overestimate (at most one reallocation to fit)
  std::set_intersection(ranges::begin(x), ranges::end(x), ranges::begin(y), ranges::end(y), ranges::back_inserter(c));
  c.shrink_to_fit();
  return c;
}

template <ranges::range RngX, ranges::range RngY, container_for<index_t> C = container<index_t>>
auto set_difference(const RngX& x, const RngY& y, C c = container<index_t>{}) -> C
{
  c.reserve(c.size() + x.size()); // overestimate (at most one reallocation to fit)
  std::set_difference(ranges::begin(x), ranges::end(x), ranges::begin(y), ranges::end(y), ranges::back_inserter(c));
  c.shrink_to_fit();
  return c;
}

} // namespace jules

#endif // JULES_BASE_INDEX_SET_H
