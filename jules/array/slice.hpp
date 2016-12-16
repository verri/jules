// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

// Even a fool who keeps silent is considered wise; when he closes his lips, he is deemed intelligent.
// Proverbs 17:28 (ESV)

#ifndef JULES_ARRAY_SLICE_H
#define JULES_ARRAY_SLICE_H

#include <jules/array/detail/common.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

#include <array>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>

namespace jules
{

/// Array slicing and dimensions representation.
///
/// This class is used internally by `jules` to represent the position of elements of an
/// array in memory.
///
/// \notes `base_slice` is `TriviallyCopyable` and `TriviallyMovable`.
/// \module N-Dimensional Array
template <std::size_t N> class base_slice
{
  static_assert(N > 0, "Slice cannot have dimension 0.");

public:
  using extent_type = std::array<index_t, N>;

  /// `InputIterator` which gives the memory positions of a slice.
  ///
  /// \notes It depends on the slice that created it, and will be invalidated if have
  /// longer lifetime than the slice.
  class iterator
  {
    friend class base_slice<N>;

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = index_t;
    using difference_type = distance_t;
    using pointer = void*;
    using reference = index_t;

    iterator() = default;

    constexpr iterator(const iterator& source) = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr iterator& operator=(const iterator& source) = default;
    constexpr iterator& operator=(iterator&& source) noexcept = default;

    constexpr auto operator++() -> iterator&
    {
      auto i = index_t{0ul};
      for (; i < N - 1; ++i) {
        indexes_[i] = (indexes_[i] + 1) % slice_->extents[i];
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

    /// \notes It does not check if the slices are the same.
    constexpr auto operator==(const iterator& other) const { return indexes_ == other.indexes_; }

    /// \notes It only checks if the slices are the same if `JULES_DEBUG_LEVEL` >=
    /// `incompatible_comparison`.
    constexpr auto operator!=(const iterator& other) const { return !(*this == other); }

    constexpr auto operator*() const -> reference { return (*slice_)(indexes_); }

    /// \notes You should not call this function.
    auto operator-> () const -> pointer
    {
      DEBUG_ASSERT(false, debug::module{}, debug::level::invalid_state, "you should not call this function");
      return nullptr;
    }

  private:
    constexpr iterator(const base_slice* slice, extent_type indexes) : slice_{slice}, indexes_{indexes} {}

    const base_slice* slice_;
    extent_type indexes_;
  };

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0ul.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to `base_slice<N>::all`.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to consistent strides based on the `extents`.
  /// \notes If `strides` are inferred, `extents` cannot be zero.
  constexpr base_slice(index_t start, extent_type extents, extent_type strides) : start{start}, extents{extents}, strides{strides}
  {
  }

  /// \group Constructor
  constexpr base_slice(index_t start, extent_type extents) : start{start}, extents{extents}
  {
    auto tmp = size();
    for (auto i = N; i > 0ul; --i) {
      DEBUG_ASSERT(extents[i - 1] != 0ul, debug::module{}, debug::level::invalid_argument,
                   "zero extents while inferring strides");
      tmp /= extents[i - 1];
      strides[i - 1] = tmp;
    }
  }

  /// \group Constructor
  constexpr base_slice() = default;

  constexpr base_slice(const base_slice& source) = default;
  constexpr base_slice(base_slice&& source) noexcept = default;

  constexpr auto operator=(const base_slice& source) -> base_slice& = default;
  constexpr auto operator=(base_slice&& source) noexcept -> base_slice& = default;

  /// Effectively the product of the extents.
  constexpr auto size() const { return prod(extents); }

  [[deprecated("to be removed in v0.2 series")]] constexpr auto dimensions() const { return extents; }

  /// \group Index
  /// Returns the memory position of the index.
  /// \param indexes Index that can be either an array or more than one argument.
  constexpr auto operator()(const extent_type& indexes) const -> index_t
  {
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides), start);
  }

  /// \group Index
  template <typename... Args, typename = detail::n_indexes_enabler<N, Args...>>
  constexpr auto operator()(Args&&... indexes) const -> index_t
  {
    static_assert(sizeof...(Args) == N, "invalid number of arguments");
    // static_assert(all_args(std::is_convertible<Args, index_t>::value...), "indexes must be convertible to index_t");
    auto arg = extent_type{{std::forward<Args>(indexes)...}};
    return (*this)(arg);
  }

  constexpr auto operator==(const base_slice& other) const
  {
    return start == other.start && extents == other.extents && strides == other.strides;
  }

  constexpr auto operator!=(const base_slice& other) const { return !(*this == other); }

  constexpr auto begin() const -> iterator { return cbegin(); }
  constexpr auto end() const -> iterator { return cend(); }

  constexpr auto cbegin() const -> iterator { return {this, {}}; }
  constexpr auto cend() const -> iterator { return {this, end_index()}; }

  constexpr auto drop_dimension() const -> base_slice<N - 1> { return drop_dimension_impl(std::make_index_sequence<N - 1>{}); }

  index_t start = 0ul;                           //< Start position.
  extent_type extents = repeat<N, index_t>(0ul); //< Size in each dimension.
  extent_type strides = repeat<N, index_t>(1ul); //< Skip in each dimension.

private:
  auto end_index() const { return end_index_impl(std::make_index_sequence<N>{}); }

  template <std::size_t... I> auto end_index_impl(std::index_sequence<I...>) const -> extent_type
  {
    return {{(I == N - 1 ? extents[I] : 0ul)...}};
  }

  template <std::size_t... I> auto drop_dimension_impl(std::index_sequence<I...>) const -> base_slice<N - 1>
  {
    return {start, {extents[I + 1]...}, {strides[I + 1]...}};
  }
};

/// 1D-Array specialization for slicing and dimensions representation.
///
/// This specialization is useful for slicing multidimensional arrays.
///
/// *TODO*: See the alias `jules::slice` for usage more information.
///
/// \notes `base_slice` is `TriviallyCopyable` and `TriviallyMovable`.
/// \module N-Dimensional Array
template <> class base_slice<1>
{
public:
  using extent_type = index_t;

  /// **TODO**: No documentation, consult `jules::base_slice<N>::iterator`.
  /// \notes For this specialization, the iterator does not depend on the parent slice.
  class iterator
  {
    friend class base_slice<1>;

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = index_t;
    using difference_type = distance_t;
    using pointer = void*;
    using reference = index_t;

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

    constexpr auto operator*() const -> reference { return index_; }

    /// \notes You should not call this function.
    auto operator-> () const -> pointer
    {
      DEBUG_ASSERT(false, debug::module{}, debug::level::invalid_state, "you should not call this function");
      return nullptr;
    }

  private:
    constexpr iterator(index_t index, index_t stride) : index_{index}, stride_{stride} {}

    index_t index_;
    index_t stride_;
  };

  /// Unsigned integer that represents all possibles extents when applied to an array.
  static constexpr auto all = index_t{0ul};

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0ul.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to `base_slice<N>::all`.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to consistent strides based on the `extents`.
  constexpr base_slice(index_t start, index_t extent, index_t stride) : start{start}, extent{extent}, stride{stride} {}

  /// \group Constructor
  constexpr base_slice(index_t start, index_t extent) : start{start}, extent{extent} {}

  /// \group Constructor
  constexpr base_slice() = default;

  constexpr base_slice(const base_slice& source) = default;
  constexpr base_slice(base_slice&& source) noexcept = default;

  constexpr auto operator=(const base_slice& source) -> base_slice& = default;
  constexpr auto operator=(base_slice&& source) noexcept -> base_slice& = default;

  /// Effectively the product of the extents.
  constexpr auto size() const { return extent; }

  [[deprecated("to be removed in v0.2 series")]] constexpr auto dimensions() const { return extent; }

  /// \group Index
  /// Returns the memory position of the `index`.
  constexpr auto operator()(index_t index) const -> index_t { return start + index * stride; }

  constexpr auto begin() const -> iterator { return cbegin(); }
  constexpr auto end() const -> iterator { return cend(); }

  constexpr auto cbegin() const -> iterator { return {start, stride}; }
  constexpr auto cend() const -> iterator { return {start + stride * extent, stride}; }

  index_t start = 0ul;  //< Start position.
  index_t extent = all; //< Number of position.
  index_t stride = 1ul; //< Skip positions.
};

using slice = base_slice<1>;

} // namespace jules

#endif // JULES_ARRAY_SLICE_H
