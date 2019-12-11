// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_STRIDED_DESCRIPTOR_H
/// \exclude
#define JULES_ARRAY_STRIDED_DESCRIPTOR_H

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
  /// `ForwardIterator` which gives the memory positions of a strided_descriptor.
  ///
  /// \notes It depends on the strided_descriptor that created it, and will be invalidated if have
  /// longer lifetime than the strided_descriptor.
  class iterator
  {
    friend class strided_descriptor<N>;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = index_t;
    using difference_type = distance_t;
    using pointer = void;
    using reference = index_t&&;

    constexpr iterator() noexcept = default;

    constexpr iterator(const iterator& source) noexcept = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr auto operator=(const iterator& source) noexcept -> iterator& = default;
    constexpr auto operator=(iterator&& source) noexcept -> iterator& = default;

    constexpr auto operator++() noexcept -> iterator&
    {
      auto i = index_t{0u};
      for (; i < N - 1; ++i) {
        indexes_[i] = (indexes_[i] + 1) % descriptor_->extents[i];
        if (indexes_[i] != 0)
          break;
      }
      if (i == N - 1)
        ++indexes_[i];

      return *this;
    }

    constexpr auto operator++(int) noexcept -> iterator
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    /// \notes It does not check if the strided_descriptors are the same.
    constexpr auto operator==(const iterator& other) const noexcept { return indexes_ == other.indexes_; }

    constexpr auto operator!=(const iterator& other) const noexcept { return !(*this == other); }

    constexpr auto operator*() const noexcept -> value_type { return (*descriptor_)(indexes_); }

  private:
    constexpr iterator(const strided_descriptor* strided_descriptor, std::array<index_t, N> indexes) noexcept
      : descriptor_{strided_descriptor}, indexes_{indexes}
    {}

    const strided_descriptor* descriptor_;
    std::array<index_t, N> indexes_;
  };

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0u.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to 0u.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to consistent strides based on the `extents`.
  /// \notes If `strides` are inferred, `extents` cannot be zero.
  constexpr strided_descriptor(index_t start, std::array<index_t, N> extents, std::array<index_t, N> strides) noexcept
    : start{start}, extents{extents}, strides{strides}
  {}

  /// \group Constructor
  constexpr strided_descriptor(index_t start, std::array<index_t, N> extents) noexcept : start{start}, extents{extents}
  {
    auto tmp = index_t{1u};
    for (auto i = index_t{0u}; i < N; ++i) {
      DEBUG_ASSERT(extents[i] != 0u, debug::default_module, debug::level::invalid_argument,
                   "zero extents while inferring strides");
      strides[i] = tmp;
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
  constexpr auto size() const noexcept { return prod(extents); }

  constexpr auto row_count() const noexcept { return extents[0u]; }

  constexpr auto column_count() const noexcept { return extents[1u]; }

  /// \group Index
  /// Returns the memory position of the index.
  /// \param indexes Index that can be either an array or more than one argument.
  constexpr auto operator()(const std::array<index_t, N>& indexes) const noexcept -> index_t
  {
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides), start);
  }

  constexpr auto begin() const noexcept -> iterator { return cbegin(); }
  constexpr auto end() const noexcept -> iterator { return cend(); }

  constexpr auto cbegin() const noexcept -> iterator { return {this, {}}; }
  constexpr auto cend() const noexcept -> iterator { return {this, end_index()}; }

  constexpr auto discard_dimension() const noexcept -> strided_descriptor<N - 1>
  {
    return discard_dimension_impl(std::make_index_sequence<N - 1>{});
  }

  template <std::size_t D> constexpr auto drop_one_level_dimensions() const -> strided_descriptor<D>
  {
    static_assert(D < N);
    DEBUG_ASSERT(std::accumulate(std::begin(extents), std::end(extents), index_t{},
                                 [](auto acc, auto d) { return acc + (d == 1 ? 1 : 0); }) >= N - D,
                 debug::default_module, debug::level::invalid_argument, "array dimensions cannot be dropped");

    auto dropped_count = index_t{0u};
    auto new_extents = repeat<D, index_t>(1u);
    auto new_strides = repeat<D, index_t>(1u);
    auto current = index_t{0u};

    for (const auto i : indices(N)) {
      if (dropped_count == N - D || extents[i] != 1) {
        new_extents[current] = extents[i];
        new_strides[current] = strides[i];
        ++current;
      } else {
        ++dropped_count;
      }
    }

    DEBUG_ASSERT(dropped_count == N - D, debug::default_module, debug::level::unreachable, "");

    return {start, new_extents, new_strides};
  }

  index_t start = 0u;                                      //< Start position.
  std::array<index_t, N> extents = repeat<N, index_t>(0u); //< Size in each dimension.
  std::array<index_t, N> strides;                          //< Skip in each dimension.

private:
  constexpr auto end_index() const noexcept { return end_index_impl(std::make_index_sequence<N>{}); }

  template <std::size_t... I> constexpr auto end_index_impl(std::index_sequence<I...>) const noexcept -> std::array<index_t, N>
  {
    return {{(I == N - 1 ? extents[I] : 0u)...}};
  }

  template <std::size_t... I>
  constexpr auto discard_dimension_impl(std::index_sequence<I...>) const noexcept -> strided_descriptor<N - 1>
  {
    return {start, {{extents[I + 1]...}}, {{strides[I + 1]...}}};
  }
};

/// Array strided descriptor specialized for one dimension.
///
/// This class is used internally by `jules` to represent the position of elements of an
/// array in memory.
///
/// \notes `strided_descriptor` is `TriviallyCopyable` and `TriviallyMovable`.
/// \module Array Types
template <> class strided_descriptor<1>
{
public:
  /// \notes For this specialization, the iterator does not depend on the parent strided_descriptor.
  class iterator
  {
    friend class strided_descriptor<1>;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = index_t;
    using difference_type = distance_t;
    using pointer = void;
    using reference = index_t&&;

    constexpr iterator() noexcept = default;

    constexpr iterator(const iterator& source) noexcept = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr auto operator=(const iterator& source) noexcept -> iterator& = default;
    constexpr auto operator=(iterator&& source) noexcept -> iterator& = default;

    constexpr auto operator++() noexcept -> iterator&
    {
      index_ = index_ + stride_;
      return *this;
    }

    constexpr auto operator++(int) noexcept -> iterator
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr auto operator==(const iterator& other) const noexcept { return index_ == other.index_; }

    constexpr auto operator!=(const iterator& other) const noexcept { return !(*this == other); }

    constexpr auto operator*() const noexcept -> value_type { return index_; }

  private:
    constexpr iterator(index_t index, index_t stride) noexcept : index_{index}, stride_{stride} {}

    index_t index_ = 0u;
    index_t stride_ = 0u;
  };

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0u.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to 0u.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to 1u.
  constexpr strided_descriptor(index_t start, const std::array<index_t, 1>& extents,
                               const std::array<index_t, 1>& strides) noexcept
    : start{start}, extents{extents}, strides{strides}
  {}

  /// \group Constructor
  constexpr strided_descriptor(index_t start, const std::array<index_t, 1>& extents) noexcept : start{start}, extents{extents} {}

  /// \group Constructor
  constexpr strided_descriptor() noexcept = default;

  constexpr strided_descriptor(const strided_descriptor& source) noexcept = default;
  constexpr strided_descriptor(strided_descriptor&& source) noexcept = default;

  constexpr auto operator=(const strided_descriptor& source) -> strided_descriptor& = default;
  constexpr auto operator=(strided_descriptor&& source) noexcept -> strided_descriptor& = default;

  /// Effectively the product of the extents.
  [[nodiscard]] constexpr auto size() const noexcept { return extents[0]; }

  [[nodiscard]] constexpr auto length() const noexcept { return extents[0]; }

  /// \group Index
  /// Returns the memory position of the `index`.
  constexpr auto operator()(const std::array<index_t, 1>& index) const noexcept -> index_t { return (*this)(index[0]); }

  /// \group Index
  constexpr auto operator()(index_t index) const noexcept -> index_t { return start + index * strides[0]; }

  [[nodiscard]] constexpr auto begin() const noexcept -> iterator { return cbegin(); }
  [[nodiscard]] constexpr auto end() const noexcept -> iterator { return cend(); }

  [[nodiscard]] constexpr auto cbegin() const noexcept -> iterator { return {start, strides[0]}; }
  [[nodiscard]] constexpr auto cend() const noexcept -> iterator { return {start + strides[0] * extents[0], strides[0]}; }

  template <std::size_t D> constexpr auto drop_one_level_dimensions() const noexcept -> strided_descriptor<D>
  {
    static_assert(D == 1);
    return *this;
  }

  index_t start = 0u;                      //< Start position.
  std::array<index_t, 1> extents = {{0u}}; //< Number of position.
  std::array<index_t, 1> strides = {{1u}}; //< Skip positions.
};

} // namespace jules

#endif // JULES_ARRAY_STRIDED_DESCRIPTOR_H
