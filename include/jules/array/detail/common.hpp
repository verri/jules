// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

// Whoever pursues righteousness and kindness will find life, righteousness, and honor.
// Proverbs 21:21 (ESV)

#ifndef JULES_ARRAY_DETAIL_COMMON_H
/// \exclude
#define JULES_ARRAY_DETAIL_COMMON_H

#include <jules/array/fwd.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/debug.hpp>

#include <algorithm>
#include <numeric>

/// \exclude
namespace jules::detail
{

template <typename Dest, typename Source>
static auto array_assign(common_array_base<Dest>& destination, const common_array_base<Source>& source)
{
  static_assert(Source::order == Dest::order, "array order mismatch");
  static_assert(std::is_assignable_v<typename Dest::value_type&, const typename Source::value_type&>, "incompatible assignment");

  DEBUG_ASSERT(source.dimensions() == destination.dimensions(), debug::default_module, debug::level::extents_check,
               "dimensions mismatch");

  auto it = source.begin();
  for (auto& elem : destination)
    elem = *it++;

  DEBUG_ASSERT(it == source.end(), debug::default_module, debug::level::unreachable, "should never happen");
}

template <typename T, std::size_t N, std::size_t... I>
static auto array_cat(const T& head, const std::array<T, N>& tail, std::index_sequence<I...>) -> std::array<T, N + 1>
{
  return {{head, tail[I]...}};
}

template <typename T, std::size_t N> static auto array_cat(const T& head, const std::array<T, N>& tail) -> std::array<T, N + 1>
{
  return array_cat(head, tail, std::make_index_sequence<N>());
}

// [start, stop)
constexpr auto seq_size(index_t start, index_t stop, index_t step) noexcept -> index_t
{
  auto size = (start < stop ? stop - start : start - stop);
  size += size % step == 0u ? 0u : 1u;
  size /= step;
  return size;
}

template <std::size_t D, std::size_t N>
constexpr auto drop_one_level_extents(const std::array<index_t, N>& extents) -> std::array<index_t, D>
{
  static_assert(D <= N);
  DEBUG_ASSERT(
    std::accumulate(begin(extents), end(extents), index_t{}, [](auto acc, auto d) { return acc + (d == 1 ? 1 : 0); }) >= N - D,
    debug::default_module, debug::level::invalid_argument, "array dimensions cannot be dropped");

  auto new_extents = repeat<D, index_t>(1u);
  std::copy_if(begin(extents), end(extents), begin(new_extents), [dropped_count = index_t{0u}](auto d) mutable -> bool {
    if (dropped_count == N - D || d != 1)
      return true;
    ++dropped_count;
    return false;
  });

  return new_extents;
}

} // namespace jules::detail

#endif // JULES_ARRAY_DETAIL_COMMON_H
