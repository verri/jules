// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_H
#define JULES_ARRAY_SLICING_H

#include <jules/array/detail/common.hpp>
#include <jules/array/strided_descriptor.hpp>

#define CHECK_BOUNDS(I, MAX)                                                                                                     \
  DEBUG_ASSERT((I) < (MAX), debug::default_module, debug::level::boundary_check, "slicing out of array limits");

#define CHECK_STRIDE(X)                                                                                                          \
  DEBUG_ASSERT((X).strides[0u] > 0u, debug::default_module, debug::level::invalid_argument, "invalid slicing stride");

#define CHECK_EXTENT(X)                                                                                                          \
  DEBUG_ASSERT((X).extents[0u] > 0u, debug::default_module, debug::level::invalid_argument, "invalid slicing extent");

namespace jules::detail
{

// Slicing size

static constexpr auto slicing_size(index_t) -> index_t
{
  (void)slicing_size;
  return 1u;
}

auto slicing_size(const strided_descriptor<1>& slice) -> index_t { return slice.size(); }

template <typename Rng, typename = meta::requires<range::SizedRange<Rng>>> auto slicing_size(const Rng& rng) -> index_t
{
  return range::size(rng);
}

template <typename... Args, typename = meta::requires<std::bool_constant<(sizeof...(Args) >= 2u)>>>
auto slicing_size(Args&&... args) -> index_t
{
  return (1u * ... * slicing_size(args));
}

// Default slicing Helpers

template <std::size_t D, std::size_t N> auto do_slice(strided_descriptor<N>&) -> void;

template <std::size_t D, std::size_t N, typename... Args>
auto do_slice(strided_descriptor<N>&, const strided_descriptor<1>&, Args&&...) -> void;

template <std::size_t D, std::size_t N, typename... Args> auto do_slice(strided_descriptor<N>&, index_t, Args&&...) -> void;

template <std::size_t D, std::size_t N> auto do_slice(strided_descriptor<N>&)
{
  static_assert(N == D, "invalid number of arguments");
}

template <std::size_t D, std::size_t N, typename... Args>
auto do_slice(strided_descriptor<N>& result, const strided_descriptor<1>& slice, Args&&... args) -> void
{
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_STRIDE(slice);
  CHECK_EXTENT(slice);
  CHECK_BOUNDS(slice.start + slice.extents[0u] * slice.strides[0u], result.extents[D] + 1u);

  result.start += result.strides[D] * slice.start;
  result.strides[D] = result.strides[D] * slice.strides[0u];
  result.extents[D] = slice.extents[0u];

  do_slice<D + 1>(result, args...);
}

template <std::size_t D, std::size_t N, typename... Args>
auto do_slice(strided_descriptor<N>& result, index_t i, Args&&... args) -> void
{
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(i, result.extents[D]);

  result.start += result.strides[D] * i;
  result.extents[D] = 1u;
  result.strides[D] = 1u;

  do_slice<D + 1>(result, args...);
}

template <std::size_t N, typename... Args>
auto default_slicing(const strided_descriptor<N>& source, Args&&... args) -> strided_descriptor<N>
{
  static_assert(sizeof...(args) == N, "invalid number of arguments");

  auto result = source;
  do_slice<0>(result, args...);

  return result;
}

// Indirect Slicing Helpers

template <std::size_t D, std::size_t N>
auto do_slice(const std::array<index_t, N>&, std::vector<index_t>&, const strided_descriptor<N>&, std::array<index_t, D>) -> void;

template <std::size_t D, std::size_t N, typename... Args>
auto do_slice(std::array<index_t, N>&, std::vector<index_t>&, const strided_descriptor<N>&, std::array<index_t, D>, index_t,
              Args&&...) -> void;

template <std::size_t D, std::size_t N, typename... Args>
auto do_slice(std::array<index_t, N>&, std::vector<index_t>&, const strided_descriptor<N>&, std::array<index_t, D>,
              const strided_descriptor<1>&, Args&&...) -> void;

template <std::size_t D, std::size_t N, typename Rng, typename... Args, typename T = range::range_value_t<Rng>,
          typename = meta::requires<range::SizedRange<Rng>>>
auto do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const strided_descriptor<N>& descriptor,
              std::array<index_t, D> ix, const Rng& rng, Args&&... args) -> void
{
  constexpr auto I = N - D - 1;

  static_assert(std::is_convertible<T, index_t>::value, "arbitrary ranges must contain indexes");
  static_assert(I == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(max(rng), descriptor.extents[I]);

  extents[I] = range::size(rng);

  for (index_t i : rng)
    do_slice(extents, indexes, descriptor, detail::array_cat(i, ix), args...);
}

template <std::size_t D, std::size_t N>
auto do_slice(const std::array<index_t, N>&, std::vector<index_t>& indexes, const strided_descriptor<N>& descriptor,
              std::array<index_t, D> ix)
{
  static_assert(D == N, "invalid number of arguments");
  indexes.push_back(descriptor(ix));
}

template <std::size_t D, std::size_t N, typename... Args>
auto do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const strided_descriptor<N>& descriptor,
              std::array<index_t, D> ix, index_t i, Args&&... args)
{
  constexpr auto I = N - D - 1;
  static_assert(I == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(i, descriptor.extents[D]);

  extents[I] = 1u;

  do_slice(extents, indexes, descriptor, detail::array_cat(i, ix), args...);
}

template <std::size_t D, std::size_t N, typename... Args>
auto do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const strided_descriptor<N>& descriptor,
              std::array<index_t, D> ix, const strided_descriptor<1>& slice, Args&&... args)
{
  constexpr auto I = N - D - 1;
  static_assert(I == sizeof...(args), "invalid number of arguments");

  CHECK_STRIDE(slice);
  CHECK_EXTENT(slice)
  CHECK_BOUNDS(slice.start + slice.extents[0u] * slice.strides[0u], descriptor.extents[I] + 1u);

  extents[I] = slice.size();

  for (index_t i : slice)
    do_slice(extents, indexes, descriptor, detail::array_cat(i, ix), args...);
}

template <std::size_t N, typename Tuple, std::size_t... I>
auto indirect_slicing_impl(const strided_descriptor<N>& descriptor, const Tuple& args, std::index_sequence<I...>)
{
  auto extents = std::array<index_t, N>{};
  auto indexes = std::vector<index_t>();

  indexes.reserve(slicing_size(std::get<I>(args)...));

  do_slice(extents, indexes, descriptor, std::array<index_t, 0>{}, std::get<N - I - 1>(args)...);

  return std::make_pair(std::move(extents), std::move(indexes));
}

template <std::size_t N, typename... Args> auto indirect_slicing(const strided_descriptor<N>& descriptor, Args&&... args)
{
  static_assert(sizeof...(args) == N, "invalid number of arguments");
  return indirect_slicing_impl(descriptor, std::forward_as_tuple(args...), std::make_index_sequence<N>());
}

} // namespace jules::detail

#undef CHECK_BOUNDS
#undef CHECK_STRIDE

#endif // JULES_ARRAY_SLICING_H
