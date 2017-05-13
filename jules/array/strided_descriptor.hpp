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

    iterator() = default;

    constexpr iterator(const iterator& source) = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr iterator& operator=(const iterator& source) = default;
    constexpr iterator& operator=(iterator&& source) noexcept = default;

    constexpr auto operator++() -> iterator&
    {
      auto i = index_t{0ul};
      for (; i < N - 1; ++i) {
        indexes_[i] = (indexes_[i] + 1) % descriptor_->extents[i];
        if (indexes_[i] != 0)
          break;
      }
      if (i == N - 1)
        ++indexes_[i];

      return *this;
    }

    constexpr auto operator++(int) -> iterator
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    /// \notes It does not check if the strided_descriptors are the same.
    constexpr auto operator==(const iterator& other) const { return indexes_ == other.indexes_; }

    /// \notes It only checks if the strided_descriptors are the same if `JULES_DEBUG_LEVEL` >=
    /// `incompatible_comparison`.
    constexpr auto operator!=(const iterator& other) const { return !(*this == other); }

    constexpr auto operator*() const -> value_type { return (*descriptor_)(indexes_); }

  private:
    constexpr iterator(const strided_descriptor* strided_descriptor, std::array<index_t, N> indexes)
      : descriptor_{strided_descriptor}, indexes_{indexes}
    {
    }

    const strided_descriptor* descriptor_;
    std::array<index_t, N> indexes_;
  };

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0ul.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to 0ul.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to consistent strides based on the `extents`.
  /// \notes If `strides` are inferred, `extents` cannot be zero.
  constexpr strided_descriptor(index_t start, std::array<index_t, N> extents, std::array<index_t, N> strides)
    : start{start}, extents{extents}, strides{strides}
  {
  }

  /// \group Constructor
  constexpr strided_descriptor(index_t start, std::array<index_t, N> extents) : start{start}, extents{extents}
  {
    auto tmp = 1ul;
    for (auto i = 0ul; i < N; ++i) {
      DEBUG_ASSERT(extents[i] != 0ul, debug::default_module, debug::level::invalid_argument,
                   "zero extents while inferring strides");
      strides[i] = tmp;
      tmp *= extents[i];
    }
  }

  /// \group Constructor
  constexpr strided_descriptor() = default;

  constexpr strided_descriptor(const strided_descriptor& source) = default;
  constexpr strided_descriptor(strided_descriptor&& source) noexcept = default;

  constexpr auto operator=(const strided_descriptor& source) -> strided_descriptor& = default;
  constexpr auto operator=(strided_descriptor&& source) noexcept -> strided_descriptor& = default;

  /// Effectively the product of the extents.
  constexpr auto size() const { return prod(extents); }

  constexpr auto row_count() const { return extents[0u]; }

  constexpr auto column_count() const { return extents[1u]; }

  /// \group Index
  /// Returns the memory position of the index.
  /// \param indexes Index that can be either an array or more than one argument.
  constexpr auto operator()(const std::array<index_t, N>& indexes) const -> index_t
  {
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides), start);
  }

  constexpr auto begin() const -> iterator { return cbegin(); }
  constexpr auto end() const -> iterator { return cend(); }

  constexpr auto cbegin() const -> iterator { return {this, {}}; }
  constexpr auto cend() const -> iterator { return {this, end_index()}; }

  constexpr auto drop_dimension() const -> strided_descriptor<N - 1>
  {
    return drop_dimension_impl(std::make_index_sequence<N - 1>{});
  }

  index_t start = 0ul;                                      //< Start position.
  std::array<index_t, N> extents = repeat<N, index_t>(0ul); //< Size in each dimension.
  std::array<index_t, N> strides;                           //< Skip in each dimension.

private:
  auto end_index() const { return end_index_impl(std::make_index_sequence<N>{}); }

  template <std::size_t... I> auto end_index_impl(std::index_sequence<I...>) const -> std::array<index_t, N>
  {
    return {{(I == N - 1 ? extents[I] : 0ul)...}};
  }

  template <std::size_t... I> auto drop_dimension_impl(std::index_sequence<I...>) const -> strided_descriptor<N - 1>
  {
    return {start, {extents[I + 1]...}, {strides[I + 1]...}};
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

    iterator() = default;

    constexpr iterator(const iterator& source) = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr iterator& operator=(const iterator& source) = default;
    constexpr iterator& operator=(iterator&& source) noexcept = default;

    constexpr auto operator++() -> iterator&
    {
      index_ = index_ + stride_;
      return *this;
    }

    constexpr auto operator++(int) -> iterator
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr auto operator==(const iterator& other) const { return index_ == other.index_; }

    constexpr auto operator!=(const iterator& other) const { return !(*this == other); }

    constexpr auto operator*() const -> value_type { return index_; }

  private:
    constexpr iterator(index_t index, index_t stride) : index_{index}, stride_{stride} {}

    index_t index_;
    index_t stride_;
  };

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0ul.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to 0ul.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to 1ul.
  constexpr strided_descriptor(index_t start, const std::array<index_t, 1>& extents, const std::array<index_t, 1>& strides)
    : start{start}, extents{extents}, strides{strides}
  {
  }

  /// \group Constructor
  constexpr strided_descriptor(index_t start, const std::array<index_t, 1>& extents) : start{start}, extents{extents} {}

  /// \group Constructor
  constexpr strided_descriptor() = default;

  constexpr strided_descriptor(const strided_descriptor& source) = default;
  constexpr strided_descriptor(strided_descriptor&& source) noexcept = default;

  constexpr auto operator=(const strided_descriptor& source) -> strided_descriptor& = default;
  constexpr auto operator=(strided_descriptor&& source) noexcept -> strided_descriptor& = default;

  /// Effectively the product of the extents.
  constexpr auto size() const { return extents[0]; }

  constexpr auto length() const { return extents[0]; }

  /// \group Index
  /// Returns the memory position of the `index`.
  constexpr auto operator()(const std::array<index_t, 1>& index) const -> index_t { return start + index[0] * strides[0]; }

  constexpr auto begin() const -> iterator { return cbegin(); }
  constexpr auto end() const -> iterator { return cend(); }

  constexpr auto cbegin() const -> iterator { return {start, strides[0]}; }
  constexpr auto cend() const -> iterator { return {start + strides[0] * extents[0], strides[0]}; }

  index_t start = 0ul;                      //< Start position.
  std::array<index_t, 1> extents = {{0ul}}; //< Number of position.
  std::array<index_t, 1> strides = {{1ul}}; //< Skip positions.
};

} // namespace jules

#endif // JULES_ARRAY_STRIDED_DESCRIPTOR_H
