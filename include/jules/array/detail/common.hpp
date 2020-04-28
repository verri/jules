// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

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
#include <utility>

/// \exclude
namespace jules::detail
{

template <common_array Dest, common_array Source> static auto array_assign(Dest& destination, Source& source)
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

template <typename T, std::size_t N, std::size_t... I>
static auto array_cat(const std::array<T, N>& head, const T& tail, std::index_sequence<I...>) -> std::array<T, N + 1>
{
  return {{head[I]..., tail}};
}

template <typename T, std::size_t N> static auto array_cat(const std::array<T, N>& head, const T& tail) -> std::array<T, N + 1>
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

// TODO: where should a put it?
template <typename F, typename Tuple, std::size_t... I>
constexpr decltype(auto) apply_n(F&& f, Tuple&& tuple,
                                 std::index_sequence<I...>) noexcept(noexcept(std::forward<F>(f)(std::get<I>(tuple)...)))
{
  return std::forward<F>(f)(std::get<I>(tuple)...);
}

template <typename Tuple, std::size_t... I> constexpr auto reverse(Tuple& t, std::index_sequence<I...>) noexcept
{
  return std::make_tuple(std::move(std::get<sizeof...(I) - I - 1>(t))...);
}

template <typename... Ts> constexpr auto reverse(std::tuple<Ts...> t) noexcept
{
  return reverse(t, std::make_index_sequence<sizeof...(Ts)>());
}

template <typename T, std::size_t N, bool... B> constexpr auto drop_which(const std::array<T, N>& x)
{
  std::array<T, count_args(B...)> y;
  std::size_t i = 0, j = 0;
  for (const auto b : {B...}) {
    if (b)
      y[i++] = x[j];
    ++j;
  }
  return y;
}

} // namespace jules::detail

#endif // JULES_ARRAY_DETAIL_COMMON_H
