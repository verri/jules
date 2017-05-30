// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

// Whoever pursues righteousness and kindness will find life, righteousness, and honor.
// Proverbs 21:21 (ESV)

#ifndef JULES_ARRAY_DETAIL_COMMON_H
/// \exclude
#define JULES_ARRAY_DETAIL_COMMON_H

#include <jules/array/meta/common.hpp>
#include <jules/core/debug.hpp>

// Forward declarations
namespace jules
{
template <typename T, std::size_t> class array;
template <typename T, typename Mapper> class strided_ref_array;
template <typename T, typename Mapper, typename... Indexes> class strided_ref_array_proxy;
template <typename T, std::size_t N> class ref_array;
}

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
static inline constexpr auto seq_size(index_t start, index_t stop, index_t step) -> index_t
{
  auto size = (start < stop ? stop - start : start - stop);
  size += size % step == 0u ? 0u : 1u;
  size /= step;
  return size;
}

} // namespace jules::detail

#endif // JULES_ARRAY_DETAIL_COMMON_H
