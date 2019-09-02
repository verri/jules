// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_H
#define JULES_ARRAY_SLICING_H

#include <jules/array/descriptor.hpp>
#include <jules/array/detail/common.hpp>
#include <jules/array/mapper.hpp>
#include <jules/array/slicing/absolute.hpp>
#include <jules/array/slicing/bounded.hpp>
#include <jules/array/strided_descriptor.hpp>

#define CHECK_BOUNDS(I, MAX)                                                                                                     \
  DEBUG_ASSERT((I) < (MAX), debug::default_module, debug::level::boundary_check, "slicing out of array limits");

#define CHECK_STRIDE(X)                                                                                                          \
  DEBUG_ASSERT((X).stride > 0u, debug::default_module, debug::level::invalid_argument, "invalid slicing stride");

#define CHECK_EXTENT(X)                                                                                                          \
  DEBUG_ASSERT((X).extent > 0u, debug::default_module, debug::level::invalid_argument, "invalid slicing extent");

namespace jules::detail
{

// Slicing size

static inline constexpr auto slicing_size(index_t) noexcept -> index_t { return 1u; }

static inline constexpr auto slicing_size(const absolute_strided_slice& slice) noexcept -> index_t { return slice.extent; }

template <typename Rng, typename = meta::requires_concept<ranges::sized_range<Rng>>>
static auto slicing_size(const Rng& rng) -> index_t
{
  return ranges::size(rng);
}

template <typename... Args, typename = meta::requires_<std::bool_constant<(sizeof...(Args) >= 2u)>>>
static auto slicing_size(Args&&... args) -> index_t
{
  return (1u * ... * slicing_size(args));
}

// Default slicing Helpers

template <std::size_t D, std::size_t N> static auto do_slice(strided_descriptor<N>&) -> void;

template <std::size_t D, std::size_t N, typename... Args>
static auto do_slice(strided_descriptor<N>&, const absolute_strided_slice&, Args&&...) -> void;

template <std::size_t D, std::size_t N, typename... Args>
static auto do_slice(strided_descriptor<N>&, index_t, Args&&...) -> void;

template <std::size_t D, std::size_t N> static auto do_slice(strided_descriptor<N>&) -> void
{
  static_assert(N == D, "invalid number of arguments");
}

template <std::size_t D, std::size_t N, typename... Args>
static auto do_slice(strided_descriptor<N>& result, const absolute_strided_slice& slice, Args&&... args) -> void
{
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_STRIDE(slice);
  CHECK_EXTENT(slice);
  CHECK_BOUNDS(slice.start + (slice.extent - 1u) * slice.stride, result.extents[D]);

  result.start += result.strides[D] * slice.start;
  result.strides[D] = result.strides[D] * slice.stride;
  result.extents[D] = slice.extent;

  do_slice<D + 1>(result, args...);
}

template <std::size_t D, std::size_t N, typename... Args>
static auto do_slice(strided_descriptor<N>& result, index_t i, Args&&... args) -> void
{
  static_assert(N - D - 1 == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(i, result.extents[D]);

  result.start += result.strides[D] * i;
  result.extents[D] = 1u;
  result.strides[D] = 1u;

  do_slice<D + 1>(result, args...);
}

template <std::size_t N, typename... Args>
static auto default_slicing(const strided_descriptor<N>& source, Args&&... args) -> strided_descriptor<N>
{
  static_assert(sizeof...(args) == N, "invalid number of arguments");

  auto result = source;
  do_slice<0>(result, args...);

  return result;
}

// Indirect Slicing Helpers

template <std::size_t D, std::size_t N>
static auto do_slice(const std::array<index_t, N>&, std::vector<index_t>&, const strided_descriptor<N>&, std::array<index_t, D>)
  -> void;

template <std::size_t D, std::size_t N, typename... Args>
static auto do_slice(std::array<index_t, N>&, std::vector<index_t>&, const strided_descriptor<N>&, std::array<index_t, D>,
                     index_t, Args&&...) -> void;

template <std::size_t D, std::size_t N, typename... Args>
static auto do_slice(std::array<index_t, N>&, std::vector<index_t>&, const strided_descriptor<N>&, std::array<index_t, D>,
                     const absolute_strided_slice&, Args&&...) -> void;

template <std::size_t D, std::size_t N, typename Rng, typename... Args, typename T = ranges::range_value_t<Rng>,
          typename = meta::requires_concept<ranges::sized_range<Rng>>>
static auto do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const strided_descriptor<N>& descriptor,
                     std::array<index_t, D> ix, const Rng& rng, Args&&... args) -> void
{
  constexpr auto I = N - D - 1;

  static_assert(std::is_convertible<T, index_t>::value, "arbitrary ranges must contain indexes");
  static_assert(I == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(max(rng), descriptor.extents[I]);

  extents[I] = ranges::size(rng);

  for (index_t i : rng)
    do_slice(extents, indexes, descriptor, detail::array_cat(i, ix), args...);
}

template <std::size_t D, std::size_t N>
static auto do_slice(const std::array<index_t, N>&, std::vector<index_t>& indexes, const strided_descriptor<N>& descriptor,
                     std::array<index_t, D> ix) -> void
{
  static_assert(D == N, "invalid number of arguments");
  indexes.push_back(descriptor(ix));
}

template <std::size_t D, std::size_t N, typename... Args>
static auto do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const strided_descriptor<N>& descriptor,
                     std::array<index_t, D> ix, index_t i, Args&&... args) -> void
{
  constexpr auto I = N - D - 1;
  static_assert(I == sizeof...(args), "invalid number of arguments");

  CHECK_BOUNDS(i, descriptor.extents[I]);

  extents[I] = 1u;

  do_slice(extents, indexes, descriptor, detail::array_cat(i, ix), args...);
}

template <std::size_t D, std::size_t N, typename... Args>
static auto do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const strided_descriptor<N>& descriptor,
                     std::array<index_t, D> ix, const absolute_strided_slice& slice, Args&&... args) -> void
{
  constexpr auto I = N - D - 1;
  static_assert(I == sizeof...(args), "invalid number of arguments");

  CHECK_STRIDE(slice);
  CHECK_EXTENT(slice)
  CHECK_BOUNDS(slice.start + (slice.extent - 1u) * slice.stride, descriptor.extents[I]);

  extents[I] = slice.extent;

  for (index_t i = 0u; i < slice.extent; ++i)
    do_slice(extents, indexes, descriptor, detail::array_cat(slice.start + i * slice.stride, ix), args...);
}

template <std::size_t N, typename Tuple, std::size_t... I>
static auto indirect_slicing_impl(const strided_descriptor<N>& descriptor, const Tuple& args, std::index_sequence<I...>)
{
  auto extents = std::array<index_t, N>{};
  auto indexes = std::vector<index_t>();

  indexes.reserve(slicing_size(std::get<I>(args)...));

  do_slice(extents, indexes, descriptor, std::array<index_t, 0>{}, std::get<N - I - 1>(args)...);

  return std::make_pair(std::move(extents), std::move(indexes));
}

template <std::size_t N, typename... Args> static auto indirect_slicing(const strided_descriptor<N>& descriptor, Args&&... args)
{
  static_assert(sizeof...(args) == N, "invalid number of arguments");
  return indirect_slicing_impl(descriptor, std::forward_as_tuple(args...), std::make_index_sequence<N>());
}

// General slicing utilities

template <typename T, typename Mapper, typename... Args> static auto array_slice(T* data, Mapper mapper, Args&&... args)
{
  constexpr auto order = Mapper::order;
  static_assert(sizeof...(Args) > 0u);
  if constexpr (((std::is_convertible_v<Args, index_t> || std::is_convertible_v<Args, absolute_strided_slice>)&&...)) {
    auto new_descriptor = detail::default_slicing(mapper.descriptor(), std::forward<Args>(args)...);
    if constexpr (std::is_same_v<Mapper, identity_mapper<order>>) {
      return strided_ref_array<T, Mapper>{data, {std::move(new_descriptor)}};
    } else {
      auto indexes = mapper.map(new_descriptor);
      return strided_ref_array<T, Mapper>{data, {new_descriptor.extents, std::move(indexes)}};
    }
  } else {
    auto [extents, positions] = detail::indirect_slicing(mapper.descriptor(), std::forward<Args>(args)...);
    if constexpr (std::is_same_v<Mapper, identity_mapper<order>>) {
      return strided_ref_array<T, vector_mapper<order>>{data, {extents, {std::move(positions)}}};
    } else {
      auto indexes = mapper.map(std::move(positions));
      return strided_ref_array<T, vector_mapper<order>>{data, {extents, std::move(indexes)}};
    }
  }
}

template <typename T, typename Mapper> static decltype(auto) array_at(T* data, const Mapper& mapper, index_t i)
{
  const auto& descriptor = mapper.descriptor();
  DEBUG_ASSERT(i < descriptor.extents[0], debug::default_module, debug::level::boundary_check, "out of range");
  if constexpr (Mapper::order == 1) {
    const auto pos = mapper.map(descriptor(i));
    return data[pos];
  } else {
    auto new_mapper = mapper.drop_first_dimension(i);
    return strided_ref_array<T, decltype(new_mapper)>{data, std::move(new_mapper)};
  }
}

template <typename T, typename Mapper> static decltype(auto) array_at(T* data, const Mapper& mapper, every_index)
{
  const auto& descriptor = mapper.descriptor();
  if constexpr (Mapper::order == 1) {
    return strided_ref_array<T, Mapper>{data, mapper};
  } else {
    return strided_ref_array_proxy<T, Mapper, absolute_strided_slice>{data, mapper, slice(0u, descriptor.extents[0], 1u)};
  }
}

template <typename T, typename Mapper, typename Index>
static decltype(auto) array_at(T* data, const Mapper& mapper, Index&& index)
{
  if constexpr (Mapper::order == 1) {
    return detail::array_slice(data, mapper, std::forward<Index>(index));
  } else {
    using IndexType = std::conditional_t<std::is_rvalue_reference_v<Index>, std::decay_t<Index>, Index>;
    return strided_ref_array_proxy<T, Mapper, IndexType>{data, mapper, std::forward<Index>(index)};
  }
}

template <typename U, std::size_t M> static decltype(auto) array_at(U* data, const descriptor<M>& desc, index_t i)
{
  DEBUG_ASSERT(i < desc.extents[0], debug::default_module, debug::level::boundary_check, "out of range");
  if constexpr (M == 1) {
    return data[i];
  } else {
    const auto new_desc = strided_descriptor<M>{i, desc.extents};
    return strided_ref_array<U, identity_mapper<M - 1>>{data, {new_desc.discard_dimension()}};
  }
}

template <typename U, std::size_t M> static decltype(auto) array_at(U* data, const descriptor<M>& desc, every_index)
{
  if constexpr (M == 1)
    return ref_array<U, M>{data, desc};
  else
    return ref_array_every_proxy<U, M, 1u>{data, desc};
}

template <typename U, std::size_t M, typename Rng, typename = meta::requires_concept<ranges::sized_range<Rng>>>
static decltype(auto) array_at(U* data, const descriptor<M>& desc, const Rng& rng)
{
  if constexpr (M == 1 && std::is_constructible_v<index_view, const Rng&>) {
    DEBUG_ASSERT(max(rng) < desc.length(), debug::default_module, debug::level::boundary_check, "out of range");
    auto view = index_view(rng);
    const auto size = view.size();
    return strided_ref_array<U, vector_mapper<1u>>{data, {{{size}}, std::move(view)}};
  } else {
    return array_at(data, identity_mapper<M>{{0u, desc.extents}}, rng);
  }
}

template <typename U, std::size_t M> static decltype(auto) array_at(U* data, const descriptor<M>& desc, absolute_slice slice)
{
  if constexpr (M == 1) {
    DEBUG_ASSERT(slice.start + slice.extent - 1u < desc.extents[0], debug::default_module, debug::level::boundary_check,
                 "out of range");
    return ref_array<U, 1u>{data + slice.start, {{{slice.extent}}}};
  } else {
    return array_at(data, identity_mapper<M>{{0u, desc.extents}}, std::move(slice));
  }
}

template <typename U, std::size_t M, typename Index>
static decltype(auto) array_at(U* data, const descriptor<M>& desc, Index&& index)
{
  return array_at(data, identity_mapper<M>{{0u, desc.extents}}, std::forward<Index>(index));
}

} // namespace jules::detail

// Proxy types
namespace jules
{

/// \exclude
template <typename T, typename Mapper, typename... Indexes> class strided_ref_array_proxy
{
  static constexpr auto order = Mapper::order;
  static_assert(sizeof...(Indexes) < order);

public:
  strided_ref_array_proxy(T* data, Mapper mapper, Indexes... indexes)
    : data_{data}, mapper_{std::move(mapper)}, indexes_(std::forward<Indexes>(indexes)...)
  {}

  decltype(auto) operator[](index_t i) && { return at(i); }

  decltype(auto) operator[](absolute_slice slice) && { return at(slice); }

  decltype(auto) operator[](absolute_strided_slice slice) && { return at(slice); }

  decltype(auto) operator[](every_index) && { return at(slice(0u, mapper_.descriptor().extents[sizeof...(Indexes)], 1u)); }

  decltype(auto) operator[](bounded_slice slice) && { return at(eval(slice, mapper_.descriptor().extents[sizeof...(Indexes)])); }

  decltype(auto) operator[](bounded_strided_slice slice) &&
  {
    return at(eval(slice, mapper_.descriptor().extents[sizeof...(Indexes)]));
  }

  template <typename Rng, typename = meta::requires_concept<ranges::sized_range<Rng>>>
  decltype(auto) operator[](const Rng& rng) &&
  {
    return at(rng);
  }

private:
  template <typename Index> decltype(auto) at(Index&& index)
  {
    return at_impl(std::forward<Index>(index), std::make_index_sequence<sizeof...(Indexes)>());
  }

  template <typename Index, std::size_t... I> decltype(auto) at_impl(Index&& index, std::index_sequence<I...>)
  {
    if constexpr (sizeof...(Indexes) == order - 1) {
      return detail::array_slice(data_, std::move(mapper_), std::get<I>(indexes_)..., std::forward<Index>(index));
    } else {
      using IndexType = std::conditional_t<std::is_rvalue_reference_v<Index>, std::decay_t<Index>, Index>;
      return strided_ref_array_proxy<T, Mapper, Indexes..., IndexType>{data_, std::move(mapper_), std::get<I>(indexes_)...,
                                                                       std::forward<Index>(index)};
    }
  }

  T* data_;
  Mapper mapper_;
  std::tuple<Indexes...> indexes_;
};

/// \exclude
template <typename T, std::size_t N, std::size_t M> class ref_array_every_proxy
{
  static_assert(M >= 1u && M < N);

public:
  ref_array_every_proxy(T* data, const descriptor<N>& descriptor) : data_{data}, descriptor_{descriptor} {}

  decltype(auto) operator[](index_t i) &&
  {
    if constexpr (M == N - 1) {
      const auto last_dim = descriptor_.extents[N - 1];
      DEBUG_ASSERT(i < last_dim, debug::default_module, debug::level::boundary_check, "out of range");

      const auto stride = prod(descriptor_.extents) / last_dim;
      descriptor_.extents[N - 1] = 1u;

      return ref_array<T, N>{data_ + i * stride, descriptor_};
    } else {
      return at(slice);
    }
  }

  decltype(auto) operator[](absolute_slice slice) &&
  {
    if constexpr (M == N - 1) {
      const auto last_dim = descriptor_.extents[N - 1];
      DEBUG_ASSERT(slice.start + slice.extent - 1u < last_dim, debug::default_module, debug::level::boundary_check,
                   "out of range");

      const auto stride = prod(descriptor_.extents) / last_dim;
      descriptor_.extents[N - 1] = slice.extent;

      return ref_array<T, N>{data_ + slice.start * stride, descriptor_};
    } else {
      return at(slice);
    }
  }

  decltype(auto) operator[](absolute_strided_slice slice) && { return at(slice); }

  decltype(auto) operator[](every_index) &&
  {
    if constexpr (M == N - 1) {
      return ref_array<T, N>{data_, descriptor_};
    } else {
      return ref_array_every_proxy<T, N, M + 1>{data_, descriptor_};
    }
  }

  decltype(auto) operator[](bounded_slice slice) && { return at(eval(slice, descriptor_.extents[M])); }

  decltype(auto) operator[](bounded_strided_slice slice) && { return at(eval(slice, descriptor_.extents[M])); }

  template <typename Rng, typename = meta::requires_concept<ranges::sized_range<Rng>>>
  decltype(auto) operator[](const Rng& rng) &&
  {
    return at(rng);
  }

private:
  template <typename Index> decltype(auto) at(Index&& index)
  {
    return at(std::forward<Index>(index), std::make_index_sequence<M>());
  }

  template <std::size_t> using slice_type = absolute_strided_slice;

  template <typename Index, std::size_t... I> decltype(auto) at(Index&& index, std::index_sequence<I...>)
  {
    using proxy_type = strided_ref_array_proxy<T, identity_mapper<N>, slice_type<I>...>;
    return proxy_type{data_, {{0u, descriptor_.extents}}, slice(0u, descriptor_.extents[I], 1u)...}[std::forward<Index>(index)];
  }

  T* data_;
  descriptor<N> descriptor_;
};

} // namespace jules

#undef CHECK_BOUNDS
#undef CHECK_STRIDE
#undef CHECK_EXTENT

#endif // JULES_ARRAY_SLICING_H
