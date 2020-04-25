// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_H
/// \exclude
#define JULES_ARRAY_SLICING_H

#include <jules/array/descriptor.hpp>
#include <jules/array/detail/common.hpp>
#include <jules/array/mapper.hpp>
#include <jules/array/slicing/absolute.hpp>
#include <jules/array/slicing/bounded.hpp>
#include <jules/array/strided_descriptor.hpp>

#include <stdexcept>
#include <type_traits>
#include <utility>

namespace jules::detail
{

struct absolute_every_index
{
  index_t dim;
};

constexpr auto slicing_size(index_t) noexcept { return 1u; }

constexpr auto slicing_size(absolute_every_index index) noexcept { return index.dim; }

constexpr auto slicing_size(absolute_strided_slice slice) noexcept { return slice.extent(); }

constexpr auto slicing_size(absolute_slice slice) noexcept { return slice.extent(); }

constexpr auto slicing_size(index_span span) noexcept { return span.size(); }

template <std::size_t N, typename Descriptor, typename... Tail>
auto slicing_fill(container<index_t>& map, const Descriptor& descriptor, std::array<index_t, N> args, index_t index, Tail... tail)
{
  if constexpr (sizeof...(Tail) == 0) {
    map.push_back(descriptor(detail::array_cat(args, index)));
  } else {
    slicing_fill(map, descriptor, detail::array_cat(args, index), tail...);
  }
}

template <std::size_t N, typename Descriptor, typename... Tail>
auto slicing_fill(container<index_t>& map, const Descriptor& descriptor, std::array<index_t, N> args, absolute_every_index index,
                  Tail... tail)
{
  for (const auto i : indices(index.dim)) {
    if constexpr (sizeof...(Tail) == 0)
      map.push_back(descriptor(detail::array_cat(args, i)));
    else
      slicing_fill(map, descriptor, detail::array_cat(args, i), tail...);
  }
}

template <std::size_t N, typename Descriptor, typename... Tail>
auto slicing_fill(container<index_t>& map, const Descriptor& descriptor, std::array<index_t, N> args, absolute_slice index,
                  Tail... tail)
{
  for (const auto i : indices(index.start(), index.extent())) {
    if constexpr (sizeof...(Tail) == 0)
      map.push_back(descriptor(detail::array_cat(args, i)));
    else
      slicing_fill(map, descriptor, detail::array_cat(args, i), tail...);
  }
}

template <std::size_t N, typename Descriptor, typename... Tail>
auto slicing_fill(container<index_t>& map, const Descriptor& descriptor, std::array<index_t, N> args,
                  absolute_strided_slice index, Tail... tail)
{
  auto indexes = ranges::views::ints(index.start(), ranges::unreachable) | ranges::views::stride(index.stride()) |
                 ranges::views::take(index.extent());
  for (const auto i : indexes) {
    if constexpr (sizeof...(Tail) == 0)
      map.push_back(descriptor(detail::array_cat(args, i)));
    else
      slicing_fill(map, descriptor, detail::array_cat(args, i), tail...);
  }
}

template <std::size_t N, typename Descriptor, typename... Tail>
auto slicing_fill(container<index_t>& map, const Descriptor& descriptor, std::array<index_t, N> args, index_span indexes,
                  Tail... tail)
{
  for (const auto i : indexes) {
    if constexpr (sizeof...(Tail) == 0)
      map.push_back(descriptor(detail::array_cat(args, i)));
    else
      slicing_fill(map, descriptor, detail::array_cat(args, i), tail...);
  }
}

template <std::size_t N, typename Tuple, std::size_t... I>
constexpr auto slicing_start(std::array<index_t, N> strides, Tuple slices, std::index_sequence<I...>)
{
  return (index_t{0} + ... + (strides[I] * std::get<I>(slices).start()));
}

template <std::size_t N, typename Tuple, std::size_t... I>
constexpr auto slicing_strides(std::array<index_t, N> strides, Tuple slices, std::index_sequence<I...>) -> std::array<index_t, N>
{
  return {{(strides[I] * std::get<I>(slices).stride())...}};
}

template <typename T, std::size_t Order, typename... Indexes>
decltype(auto) do_slice(T* data, descriptor<Order> desc, Indexes... indexes)
{
  static_assert(sizeof...(Indexes) == Order);

  if constexpr (all_args(std::is_same_v<absolute_every_index, Indexes>...)) //
  {
    return ref_array<T, Order>(data, desc);
  }                                                                 //
  else if constexpr (all_args(std::is_same_v<index_t, Indexes>...)) //
  {
    return *(data + descriptor({{indexes...}}));
  }                                                                     //
  else if constexpr (std::is_same_v<last_element<Indexes...>, index_t>) //
  {
    // every time the last index is an index_t, we can reduce the problem.
    const auto ix = detail::array_cat(repeat<Order - 1, index_t>(0u), last_arg(indexes...));
    data += descriptor(ix);

    return apply_n(
      [&](auto... newindexes) constexpr->decltype(auto) { return do_slice(data, desc.discard_tail_dimension(), newindexes...); },
      std::make_tuple(indexes...), std::make_index_sequence<Order - 1>());
  }                                                                            //
  else if constexpr (std::is_same_v<last_element<Indexes...>, absolute_slice>) //
  {
    // in a similiar manner, if the last element is an absolute_slice (not strided), we
    // also can reduce the problem (that is adjust the start and extent and forwarding as
    // a every_index).

    const absolute_slice slice = last_arg(indexes...);

    // adjust the beginning
    const auto ix = detail::array_cat(repeat<Order - 1, index_t>(0u), slice.start());
    data += descriptor(ix);

    // adjust the extent
    desc.set_extent(Order - 1, slice.extent());

    return apply_n(
      [&](auto... newindexes) constexpr->decltype(auto) {
        return do_slice(data, desc, newindexes..., absolute_every_index{slice.extent()});
      },
      std::make_tuple(indexes...), std::make_index_sequence<Order - 1>());
  }                                                                                               //
  else if constexpr (sizeof...(Indexes) == 1 && all_args(std::is_same_v<Indexes, index_span>...)) //
  {
    const index_span index = first_arg(indexes...);
    return ref_strided_array<Order, span_mapper>(data, span_mapper(index));
  }                                                                    //
  else if constexpr (any_args(std::is_same_v<Indexes, index_span>...)) //
  {
    constexpr auto D = count_args(std::is_same_v<Indexes, index_t>...);

    const auto newdescriptor = descriptor<Order>{{slicing_size(indexes)...}}.template drop_one_level_dimensions<Order - D>();
    const auto size = newdescriptor.size();

    container<index_t> map;
    map.reserve(size);

    slicing_fill(map, desc, std::array<index_t, 0u>{}, indexes...);

    return ref_strided_array<Order - D, container_mapper>(data, container_mapper(newdescriptor, std::move(map)));
  }    //
  else //
  {
    constexpr auto D = count_args(std::is_same_v<Indexes, index_t>...);

    constexpr auto to_strided_slice = overloaded{
      [](index_t i) -> absolute_strided_slice {
        return {i, 1, 1};
      },
      [](absolute_every_index index) -> absolute_strided_slice {
        return {0, index.dim, 1};
      },
      [](auto index) -> absolute_strided_slice { return index; },
    };

    const strided_descriptor descriptor = desc;
    const auto slices = std::make_tuple(to_strided_slice(indexes)...);

    data += slicing_start(desc.strides(), slices, std::make_index_sequence<Order>());

    const auto strides = slicing_strides(desc.strides(), slices, std::make_index_sequence<Order>());
    const auto extents = std::array<index_t, N>{{slicing_size(indexes)...}};

    const auto newdescriptor = strided_descriptor<Order>(extents, strides).template drop_one_level_dimensions<Order - D>();
    return ref_strided_array<Order - D, identity_mapper>(data, identity_mapper(newdescriptor));
  }
}

// Checks boundaries and extents.  If index is a slice, also makes it absolute.
template <typename Index> auto check_slicing(Index index, index_t dim)
{
  if constexpr (valid_slice<Index>) {
    using absolute_type = typename slice_traits<Index>::absolute_type;
    static_assert(std::is_same_v<absolute_type, absolute_slice> || std::is_same_v<absolute_slice, absolute_strided_slice>);

    const auto absolute = absolutize(index, dim);

    DEBUG_ASSERT(absolute.start() < dim, debug::default_module, debug::level::boundary_check, "slicing out of array limits");
    DEBUG_ASSERT(absolute.extent() > 0u, debug::default_module, debug::level::invalid_argument, "invalid slicing extent");

    if constexpr (std::is_same_v<absolute_type, absolute_strided_slice>) {
      DEBUG_ASSERT(absolute.start() + (absolute.extent() - 1u) * absolute.stride() < dim, debug::default_module,
                   debug::level::boundary_check, "slicing out of array limits");
      DEBUG_ASSERT(absolute.stride() > 0u, debug::default_module, debug::level::invalid_argument, "invalid slicing stride");
    } else {
      DEBUG_ASSERT(absolute.start() + absolute.extent() <= dim, debug::default_module, debug::level::boundary_check,
                   "slicing out of array limits");
    }

    return absolute;
  }

  if constexpr (std::is_same_v<Index, index_t>) {
    DEBUG_ASSERT(index < dim, debug::default_module, debug::level::boundary_check, "slicing out of array limits");
    return index;
  }

  if constexpr (std::is_same_v<Index, index_span>) {
    DEBUG_ASSERT(index.size() > 0u, debug::default_module, debug::level::invalid_argument, "invalid slicing extent");
    DEBUG_ASSERT(max(index) < dim, debug::default_module, debug::level::boundary_check, "slicing out of array limits");
    return index;
  }

  if constexpr (std::is_same_v<Index, every_index>) {
    return absolute_every_index{dim};
  }
}

template <std::size_t, typename, typename, typename...> class array_slicing_proxy;

// Calls do_slice when the number of array indexes (that is array[index1]...[indexN]) is
// the same as array order, otherwise keeps proxing.
template <std::size_t Order, typename T, typename Descriptor, typename Tuple, std::size_t... I>
decltype(auto) forward_slicing(T* data, const Descriptor& descriptor, Tuple indexes, std::index_sequence<I...>)
{
  if constexpr (Order == sizeof...(I)) {
    const auto dims = descriptor.dimensions();
    return do_slice(data, descriptor, check_slicing(std::get<I>(indexes), dims[I])...);
  } else {
    return array_slicing_proxy<Order, T, Descriptor, std::tuple_element_t<I, Tuple>...>(data, descriptor,
                                                                                        std::get<I>(indexes)...);
  }
}

template <std::size_t Order, typename T, typename Descriptor, typename Index>
decltype(auto) forward_slicing(T* data, const Descriptor& descriptor, Index index)
{
  if constexpr (Order == 1) {
    const auto dims = descriptor.dimensions();
    return do_slice(data, descriptor, check_slicing(index, dims[0]));
  } else {
    return array_slicing_proxy<Order, T, Descriptor, Index>(data, descriptor, index);
  }
}

// Array proxy to represent partial slicing.
template <std::size_t Order, typename T, typename Descriptor, typename... Indexes> class array_slicing_proxy
{
  static_assert(sizeof...(Indexes) < Order);

public:
  array_slicing_proxy(T* data, const Descriptor& descriptor, Indexes... indexes)
    : data_(data), descriptor_(descriptor), indexes_(indexes...)
  {}

  decltype(auto) operator[](index_t index) && { return std::move(*this).forward(index); }
  decltype(auto) operator[](valid_slice auto index) && { return std::move(*this).forward(index); }
  decltype(auto) operator[](every_index index) && { return std::move(*this).forward(index); }
  decltype(auto) operator[](index_span index) && { return std::move(*this).forward(index); }

private:
  template <typename Index, std::size_t... I> decltype(auto) forward(Index index, std::index_sequence<I...>) &&
  {
    return forward_slicing<Order>(data_, descriptor_, std::make_tuple(std::get<I>(indexes_)..., index),
                                  std::make_index_sequence<sizeof...(Indexes) + 1>());
  }

  T* data_;
  const Descriptor& descriptor_;
  std::tuple<Indexes...> indexes_;
};

} // namespace jules::detail

#endif // JULES_ARRAY_SLICING_H
