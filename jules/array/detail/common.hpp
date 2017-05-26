// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

// Whoever pursues righteousness and kindness will find life, righteousness, and honor.
// Proverbs 21:21 (ESV)

#ifndef JULES_ARRAY_DETAIL_COMMON_H
/// \exclude
#define JULES_ARRAY_DETAIL_COMMON_H

#include <jules/array/meta/common.hpp>
#include <jules/core/debug.hpp>

/// \exclude
namespace jules::detail
{

template <typename Source, typename Dest>
static auto array_assign(common_array_base<Source>& destination, const common_array_base<Dest>& source)
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

static inline auto assert_in_bound(index_t index, index_t extent) -> void
{
  DEBUG_ASSERT(index < extent, debug::default_module, debug::level::boundary_check, "out of range");
}

template <std::size_t N> static auto assert_in_bound(const std::array<index_t, N>& indexes, const std::array<index_t, N>& extents)
{
  return assert_in_bound(indexes, extents, std::make_index_sequence<N>());
}

template <std::size_t N, std::size_t... I>
static auto assert_in_bound(const std::array<index_t, N>& indexes, const std::array<index_t, N>& extents,
                            std::index_sequence<I...>)
{
  DEBUG_ASSERT(((indexes[I] < extents[I]) && ...), debug::default_module, debug::level::boundary_check, "out of range");
}

template <typename T, std::size_t N> static auto array_cat(const T& head, const std::array<T, N>& tail) -> std::array<T, N + 1>
{
  return array_cat(head, tail, std::make_index_sequence<N>());
}

template <typename T, std::size_t N, std::size_t... I>
static auto array_cat(const T& head, const std::array<T, N>& tail, std::index_sequence<I...>) -> std::array<T, N + 1>
{
  return {{head, tail[I]...}};
}

static inline auto seq_size(index_t start, index_t stop, index_t step) -> index_t
{
  auto size = (start < stop ? stop - start : start - stop);
  size += size % step == 0u ? 0u : 1u;
  size /= step;
  return size;
}

} // namespace jules::detail

#endif // JULES_ARRAY_DETAIL_COMMON_H
