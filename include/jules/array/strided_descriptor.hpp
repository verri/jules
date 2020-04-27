// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_STRIDED_DESCRIPTOR_H
/// \exclude
#define JULES_ARRAY_STRIDED_DESCRIPTOR_H

#include <jules/array/detail/common.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

/// Array strided descriptor.
///
/// This class is used internally by `jules` to represent the position of elements of an
/// array in memory.
///
/// \notes `strided_descriptor` is `TriviallyCopyable` and `TriviallyMovable`.
/// \module Array Types
template <std::size_t N> class strided_descriptor
{
  static_assert(N > 0, "Descriptor cannot have dimension 0.");

public:
  constexpr static auto order = N;

  /// `ForwardIterator` which gives the memory positions of a strided_descriptor.
  ///
  /// \notes It depends on the strided_descriptor that created it, and will be invalidated if have
  /// longer lifetime than the strided_descriptor.
  class iterator
  {
    friend class strided_descriptor<N>;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = index_t;
    using difference_type = distance_t;
    using pointer = void;
    using reference = index_t&&;

    constexpr iterator() noexcept = default;

    constexpr iterator(const iterator& source) noexcept = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr auto operator=(const iterator& source) noexcept -> iterator& = default;
    constexpr auto operator=(iterator&& source) noexcept -> iterator& = default;

    constexpr auto operator++() -> iterator&
    {
      DEBUG_ASSERT(indexes_[N - 1] < extents_[N - 1], debug::default_module, debug::level::boundary_check,
                   "iterating beyond end of strided descriptor.");

      if constexpr (N == 1) {
        ++indexes_[0];
      } else {
        auto i = index_t{0u};
        for (; i < N - 1; ++i) {
          indexes_[i] = (indexes_[i] + 1) % extents_[i];
          if (indexes_[i] != 0)
            break;
        }
        if (i == N - 1)
          ++indexes_[i];
      }

      return *this;
    }

    constexpr auto operator++(int) -> iterator
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr auto operator--() -> iterator&
    {
      DEBUG_ASSERT(sum(indexes_) > 0, debug::default_module, debug::level::boundary_check,
                   "iterating before the beginning strided descriptor.");

      if constexpr (N == 1) {
        --indexes_[0];
      } else {
        auto i = index_t{0u};
        for (; i < N - 1; ++i) {
          if (indexes_[i] == 0) {
            indexes_[i] = extents_[i] - 1;
            continue;
          }
          break;
        }
        --indexes_[i];
      }

      return *this;
    }

    constexpr auto operator--(int) -> iterator
    {
      auto copy = *this;
      --(*this);
      return copy;
    }

    /// \notes It does not check if the strided_descriptors are the same.
    constexpr auto operator==(const iterator& other) const noexcept { return indexes_ == other.indexes_; }

    constexpr auto operator!=(const iterator& other) const noexcept { return !(*this == other); }

    constexpr auto operator*() const noexcept -> value_type
    {
      if constexpr (N == 1) {
        return indexes_[0] * strides_[0];
      } else {
        return std::inner_product(std::begin(indexes_), std::end(indexes_), std::begin(strides_), index_t{0u});
      }
    }

  private:
    constexpr iterator(std::array<index_t, N> indexes, std::array<index_t, N> strides, std::array<index_t, N> extents) noexcept
      : indexes_{indexes}, strides_{strides}, extents_{extents}
    {}

    std::array<index_t, N> indexes_;
    std::array<index_t, N> strides_;
    std::array<index_t, N> extents_;
  };

  /// \group Constructor
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to 0u.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to consistent strides based on the `extents`.
  /// \notes If `strides` are inferred, `extents` cannot be zero.
  constexpr strided_descriptor(std::array<index_t, N> extents, std::array<index_t, N> strides) noexcept
    : extents_{extents}, strides_{strides}
  {}

  /// \group Constructor
  constexpr strided_descriptor(std::array<index_t, N> extents) noexcept : extents_{extents}
  {
    auto tmp = index_t{1u};
    for (const auto i : indices(N)) {
      DEBUG_ASSERT(extents[i] != 0u, debug::default_module, debug::level::invalid_argument,
                   "zero extents while inferring strides");
      strides_[i] = tmp;
      tmp *= extents[i];
    }
  }

  /// \group Constructor
  constexpr strided_descriptor() noexcept = default;

  constexpr strided_descriptor(const strided_descriptor& source) noexcept = default;
  constexpr strided_descriptor(strided_descriptor&& source) noexcept = default;

  constexpr auto operator=(const strided_descriptor& source) noexcept -> strided_descriptor& = default;
  constexpr auto operator=(strided_descriptor&& source) noexcept -> strided_descriptor& = default;

  /// Effectively the product of the extents.
  [[nodiscard]] constexpr auto size() const noexcept { return prod(extents_); }

  [[nodiscard]] constexpr auto dimensions() const noexcept { return extents_; }

  /// \group Index
  /// Returns the memory position of the index.
  /// \param indexes Index that can be either an array or more than one argument.
  constexpr auto operator()(const std::array<index_t, N>& indexes) const noexcept -> index_t
  {
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides_), index_t{0});
  }

  constexpr auto begin() const noexcept -> iterator { return cbegin(); }
  constexpr auto end() const noexcept -> iterator { return cend(); }

  constexpr auto cbegin() const noexcept -> iterator { return {repeat<N, index_t>(0u), strides_, extents_}; }
  constexpr auto cend() const noexcept -> iterator
  {
    return {detail::array_cat(repeat<N - 1, index_t>(0u), extents_[N - 1]), strides_, extents_};
  }

  constexpr auto discard_tail_dimension() const noexcept -> strided_descriptor<N - 1>
  {
    return discard_tail_dimension_impl(std::make_index_sequence<N - 1>{});
  }

  constexpr auto discard_head_dimension() const noexcept -> strided_descriptor<N - 1>
  {
    return discard_head_dimension_impl(std::make_index_sequence<N - 1>{});
  }

  template <std::size_t D> constexpr auto drop_one_level_dimensions() const -> strided_descriptor<D>
  {
    if constexpr (N == D) {
      return *this;
    } else {
      static_assert(D < N);
      DEBUG_ASSERT(std::accumulate(std::begin(extents_), std::end(extents_), index_t{},
                                   [](auto acc, auto d) { return acc + (d == 1 ? 1 : 0); }) >= N - D,
                   debug::default_module, debug::level::invalid_argument, "array dimensions cannot be dropped");

      auto dropped_count = index_t{0u};
      auto new_extents = repeat<D, index_t>(1u);
      auto new_strides = repeat<D, index_t>(1u);
      auto current = index_t{0u};

      for (const auto i : indices(N)) {
        if (dropped_count == N - D || extents_[i] != 1) {
          new_extents[current] = extents_[i];
          new_strides[current] = strides_[i];
          ++current;
        } else {
          ++dropped_count;
        }
      }

      DEBUG_ASSERT(dropped_count == N - D, debug::default_module, debug::level::unreachable, "");

      return {new_extents, new_strides};
    }
  }

  constexpr auto extent(index_t i) const noexcept { return extents_[i]; }
  constexpr auto stride(index_t i) const noexcept { return strides_[i]; }

  constexpr auto extents() const noexcept -> const std::array<index_t, N> { return extents_; }
  constexpr auto strides() const noexcept -> const std::array<index_t, N> { return strides_; }

  constexpr auto set_extent(index_t i, index_t value) noexcept { extents_[i] = value; }
  constexpr auto set_stride(index_t i, index_t value) noexcept { strides_[i] = value; }

private:
  template <std::size_t... I>
  constexpr auto discard_tail_dimension_impl(std::index_sequence<I...>) const noexcept -> strided_descriptor<N - 1>
  {
    static_assert(sizeof...(I) == N - 1);
    return {{{extents_[I + 1]...}}, {{strides_[I + 1]...}}};
  }

  template <std::size_t... I>
  constexpr auto discard_head_dimension_impl(std::index_sequence<I...>) const noexcept -> strided_descriptor<N - 1>
  {
    static_assert(sizeof...(I) == N - 1);
    return {{{extents_[I]...}}, {{strides_[I]...}}};
  }

  std::array<index_t, N> extents_ = repeat<N, index_t>(0u); //< Size in each dimension.
  std::array<index_t, N> strides_;                          //< Skip in each dimension.
};

} // namespace jules

#endif // JULES_ARRAY_STRIDED_DESCRIPTOR_H
