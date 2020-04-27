// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DESCRIPTOR_H
/// \exclude
#define JULES_ARRAY_DESCRIPTOR_H

#include <jules/array/detail/common.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

/// Array descriptor.
///
/// This class is used internally by `jules` to represent the position of elements of an
/// array in memory.
///
/// \notes `descriptor` is `TriviallyCopyable` and `TriviallyMovable`.
/// \module Array Types
template <std::size_t N> class descriptor
{
  static_assert(N > 0, "Descriptor cannot have dimension 0.");

public:
  /// \group Constructor
  constexpr descriptor(const std::array<index_t, N>& extents) noexcept : extents{extents} {}

  /// \group Constructor
  constexpr descriptor() noexcept = default;

  constexpr descriptor(const descriptor& source) noexcept = default;
  constexpr descriptor(descriptor&& source) noexcept = default;

  constexpr auto operator=(const descriptor& source) noexcept -> descriptor& = default;
  constexpr auto operator=(descriptor&& source) noexcept -> descriptor& = default;

  constexpr auto size() const noexcept { return prod(extents); }

  constexpr auto dimensions() const noexcept { return extents; }

  /// \group Index
  /// Returns the memory position of the index.
  constexpr auto operator()(const std::array<index_t, N>& indexes) const noexcept -> index_t
  {
    if constexpr (N == 1) {
      return indexes[0];
    } else if constexpr (N == 2) {
      return indexes[0] + extents[0] * indexes[1];
    } else {
      auto stride = index_t{1u};
      auto pos = index_t{0u};
      for (auto i = index_t{0u}; i < N; ++i) {
        pos += stride * indexes[i];
        stride *= extents[i];
      }
      return pos;
    }
  }

  /// \group Index
  constexpr auto operator()(index_t index) const noexcept -> index_t
  {
    static_assert(N == 1u);
    return index;
  }

  constexpr auto discard_head_dimension() const noexcept -> descriptor<N - 1>
  {
    return discard_head_dimension_impl(std::make_index_sequence<N - 1>{});
  }

  constexpr auto discard_tail_dimension() const noexcept -> descriptor<N - 1>
  {
    return discard_tail_dimension_impl(std::make_index_sequence<N - 1>{});
  }

  template <std::size_t D> constexpr auto drop_one_level_dimensions() const -> descriptor<D>
  {
    if constexpr (D == N)
      return *this;
    else
      return {detail::template drop_one_level_extents<D>(extents)};
  }

  constexpr auto set_extent(index_t i, index_t value) noexcept { extents[i] = value; }

private:
  template <std::size_t... I>
  constexpr auto discard_head_dimension_impl(std::index_sequence<I...>) const noexcept -> descriptor<N - 1>
  {
    return {{{extents[I + 1]...}}};
  }

  template <std::size_t... I>
  constexpr auto discard_tail_dimension_impl(std::index_sequence<I...>) const noexcept -> descriptor<N - 1>
  {
    return {{{extents[I]...}}};
  }

  std::array<index_t, N> extents = repeat<N, index_t>(0u); //< Size in each dimension.
};

} // namespace jules

#endif // JULES_ARRAY_DESCRIPTOR_H
