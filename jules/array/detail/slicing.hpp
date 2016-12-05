// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DETAIL_SLICING_H
#define JULES_ARRAY_DETAIL_SLICING_H

#include <jules/array/slice.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/range.hpp>
#include <jules/core/type.hpp>

#include <array>

#define CHECK_BOUNDS(I, MAX)                                                                                                     \
  DEBUG_ASSERT((I) < (MAX), debug::module{}, debug::level::boundary_check, "slicing out of array limits");

#define CHECK_STRIDE(VALUE) DEBUG_ASSERT((VALUE) > 0u, debug::module{}, debug::level::invalid_argument, "invalid slicing stride");

namespace jules
{
namespace detail
{

// Accessor helpers

template <std::size_t N> struct slice_accessor {
  static auto extent(const base_slice<N>& slice, index_t i) { return slice.extents[i]; }
  static auto stride(const base_slice<N>& slice, index_t i) { return slice.strides[i]; }
  static auto extent(base_slice<N>& slice, index_t i) -> index_t& { return slice.extents[i]; }
  static auto stride(base_slice<N>& slice, index_t i) -> index_t& { return slice.strides[i]; }
};

template <> struct slice_accessor<1> {
  static auto extent(const base_slice<1>& slice, index_t) { return slice.extent; }
  static auto stride(const base_slice<1>& slice, index_t) { return slice.stride; }
  static auto extent(base_slice<1>& slice, index_t) -> index_t& { return slice.extent; }
  static auto stride(base_slice<1>& slice, index_t) -> index_t& { return slice.stride; }
};

template <std::size_t N> auto extent(const base_slice<N>& slice, index_t i = 0u) { return slice_accessor<N>::extent(slice, i); }

template <std::size_t N> auto stride(const base_slice<N>& slice, index_t i = 0u) { return slice_accessor<N>::stride(slice, i); }

template <std::size_t N> auto extent(base_slice<N>& slice, index_t i = 0u) -> index_t&
{
  return slice_accessor<N>::extent(slice, i);
}

template <std::size_t N> auto stride(base_slice<N>& slice, index_t i = 0u) -> index_t&
{
  return slice_accessor<N>::stride(slice, i);
}

static inline auto drop(const std::array<index_t, 1>& array) { return array[0]; }

template <std::size_t N> auto drop(const std::array<index_t, N>& array) { return array; }

static inline auto undrop(index_t value) -> std::array<index_t, 1> { return {{value}}; }

template <std::size_t N> auto undrop(const std::array<index_t, N>& array) { return array; }

// Concatening arrays

template <typename T, std::size_t N, std::size_t... I>
static inline auto cat_helper(const std::array<T, N>& head, const T& tail, std::index_sequence<I...>) -> std::array<T, N + 1>
{
  return {{head[I]..., tail}};
}

template <typename T, std::size_t N>
static inline auto cat(const std::array<T, N>& head, const T& tail) -> std::array<T, N + 1>
{
  return detail::cat_helper(head, tail, std::make_index_sequence<N>());
}

// Slicing size

static inline auto seq_size(index_t start, index_t stop, index_t step)
{
  auto size = (start < stop ? stop - start : start - stop);
  size += size % step == 0 ? 0 : 1;
  size /= step;

  return size;
}

template <std::size_t D, std::size_t N> index_t slicing_size(const std::array<index_t, N>&) { return 1; }

template <std::size_t D, std::size_t N, typename... Args>
index_t slicing_size(const std::array<index_t, N>& extents, index_t, Args&&... args)
{
  return slicing_size<D + 1>(extents, std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename... Args>
index_t slicing_size(const std::array<index_t, N>& extents, const base_slice<1>& slice, Args&&... args)
{
  return (slice.size() == 0 ? seq_size(slice.start, extents[D], slice.stride) : slice.size()) *
         slicing_size<D + 1>(extents, std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename Rng, typename... Args, typename T = range::range_value_t<Rng>,
          CONCEPT_REQUIRES_(range::Range<Rng>())>
index_t slicing_size(const std::array<index_t, N>& extents, const Rng& rng, Args&&... args)
{
  static_assert(std::is_convertible<T, index_t>::value, "arbitrary ranges must contain indexes");
  return range::size(rng) * slicing_size<D + 1>(extents, std::forward<Args>(args)...);
}

// Default slicing Helpers
// TODO: XXX: It may be wrong, since the data is stored column-wise now.

template <std::size_t D, std::size_t N> void do_slice(base_slice<N>&) { static_assert(N == D, "invalid number of arguments"); }

template <std::size_t D, std::size_t N, typename... Args> void do_slice(base_slice<N>& result, index_t i, Args&&... args)
{
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(i, extent(result, D));

  result.start += stride(result, D) * i;
  extent(result, D) = 1;
  stride(result, D) = 1;

  do_slice<D + 1>(result, std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(base_slice<N>& result, const base_slice<1>& slice, Args&&... args)
{
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_STRIDE(slice.stride);
  CHECK_BOUNDS(slice.start + slice.extent * slice.stride, extent(result, D));

  result.start += stride(result, D) * slice.start;
  stride(result, D) = stride(result, D) * slice.stride;
  extent(result, D) = slice.extent > 0 ? slice.extent : (extent(result, D) - slice.start) / slice.stride;

  do_slice<D + 1>(result, std::forward<Args>(args)...);
}

template <std::size_t N, typename... Args> base_slice<N> default_slicing(const base_slice<N>& source, Args&&... args)
{
  static_assert(sizeof...(args) == N, "invalid number of arguments");
  auto result = source;
  do_slice<0>(result, std::forward<Args>(args)...);
  return result;
}

// Indirect Slicing Helpers

template <std::size_t D, std::size_t N>
void do_slice(std::array<index_t, N>&, std::vector<index_t>& indexes, const base_slice<N>& slice, std::array<index_t, D> ix)
{
  static_assert(D == N, "invalid number of arguments");
  indexes.push_back(slice(drop(ix)));
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const base_slice<N>& slice,
              std::array<index_t, D> ix, index_t i, Args&&... args)
{
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(i, extent(slice, D));

  extents[D] = 1;
  do_slice(extents, indexes, slice, cat(ix, i), std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename... Args>
void do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const base_slice<N>& slice,
              std::array<index_t, D> ix, const base_slice<1>& rng_base, Args&&... args)
{
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_STRIDE(rng_base.stride);
  CHECK_BOUNDS(rng_base.start + rng_base.extent * rng_base.stride, extent(slice, D));

  auto rng = rng_base;
  if (rng.size() == 0)
    rng = base_slice<1>{rng_base.start, seq_size(rng_base.start, extent(slice, D), rng_base.stride), rng_base.stride};

  extents[D] = rng.size();

  for (index_t i : rng)
    do_slice(extents, indexes, slice, cat(ix, i), std::forward<Args>(args)...);
}

template <std::size_t D, std::size_t N, typename Rng, typename... Args, typename T = range::range_value_t<Rng>,
          CONCEPT_REQUIRES_(range::Range<Rng>())>
void do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const base_slice<N>& slice,
              std::array<index_t, D> ix, const Rng& rng, Args&&... args)
{
  static_assert(std::is_convertible<T, index_t>::value, "arbitrary ranges must contain indexes");
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(*max(rng), extent(slice, D));

  extents[D] = range::size(rng);

  for (index_t i : rng)
    do_slice(extents, indexes, slice, cat(ix, i), std::forward<Args>(args)...);
}

template <std::size_t N, typename... Args>
std::pair<std::array<index_t, N>, std::vector<index_t>> indirect_slicing(const base_slice<N>& slice, Args&&... args)
{
  static_assert(sizeof...(args) == N, "invalid number of arguments");

  std::pair<std::array<index_t, N>, std::vector<index_t>> result;
  result.second.reserve(slicing_size<0>(undrop(slice.dimensions()), std::forward<Args>(args)...));

  do_slice(result.first, result.second, slice, std::array<index_t, 0>{}, std::forward<Args>(args)...);

  return result;
}

} // namespace detail
} // namespace jules

#undef CHECK_BOUNDS
#undef CHECK_STRIDE

#endif // JULES_ARRAY_DETAIL_SLICING_H
