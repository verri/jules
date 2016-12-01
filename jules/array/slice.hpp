#ifndef JULES_ARRAY_SLICE_H
#define JULES_ARRAY_SLICE_H

#include <jules/base/numeric.hpp>
#include <jules/core/type.hpp>

#include <array>
#include <iterator>
#include <numeric>
#include <utility>

namespace jules
{

/// Array slicing and dimensions representation.
/// `base_slice` is `TriviallyCopyable` and `TriviallyMovable`.
/// \module N-Dimensional Array
template <std::size_t N> class base_slice
{
  static_assert(N > 0, "Slice cannot have dimension 0.");

public:
  class iterator : public std::iterator<std::input_iterator_tag, uint, sint, void*, uint>
  {
    friend class base_slice<N>;

  public:
    iterator() = delete;

    constexpr iterator(const iterator& source) = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr iterator& operator=(const iterator& source) = default;
    constexpr iterator& operator=(iterator&& source) noexcept = default;

    constexpr auto operator++() -> iterator&;
    constexpr auto operator++(int) -> iterator;

    constexpr auto operator==(const iterator& other) const;
    constexpr auto operator!=(const iterator& other) const;

    constexpr auto operator*() const -> reference;
    constexpr auto operator-> () const -> pointer { return nullptr; }

  private:
    iterator(const base_slice* slice, std::array<uint, N> indexes) : slice_{slice}, indexes_{indexes} {}

    const base_slice* slice_;
    std::array<uint, N> indexes_;
  };

  /// One or more unsigned integers that represents all possibles extents when
  /// applied to an array.
  static constexpr auto all = repeat<N, uint>(0u);

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0u.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to `base_slice<N>::all`.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to consistent strides based on the `extents`.
  constexpr base_slice(uint start, std::array<uint, N> extents, std::array<uint, N> strides)
    : start{start}, extents{extents}, strides{strides}
  {
  }

  /// \group Constructor
  constexpr base_slice(uint start, std::array<uint, N> extents) : start{start}, extents{extents}
  {
    auto tmp = size();
    for (auto i = N; i > 0u; --i) {
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

  auto operator()(const std::array<std::size_t, N>& indexes) const -> uint
  {
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides), start);
  }

  auto begin() const -> iterator { return cbegin(); }
  auto end() const -> iterator { return cend(); }

  auto cbegin() const -> iterator { return {this, {}}; }
  auto cend() const -> iterator { return {this, end_index()}; }

  uint start = 0u;                                   //< Start position.
  std::array<uint, N> extents = all;                 //< Size in each dimension.
  std::array<uint, N> strides = repeat<N, uint>(1u); //< Skip in each dimension.

private:
  auto end_index() const { return end_index_impl(std::make_index_sequence<N>()); }

  template <std::size_t... I> auto end_index_impl(std::index_sequence<I...>) const -> std::array<uint, N>
  {
    return {(I == N - 1 ? extents[I] : 0ul)...};
  }
};

/// 1D-Array specialization for slicing and dimensions representation.
/// `base_slice` is `TriviallyCopyable` and `TriviallyMovable`.
/// \module N-Dimensional Array
template <> class base_slice<1>
{
public:
  class iterator : public std::iterator<std::input_iterator_tag, uint, sint, void*, uint>
  {
    friend class base_slice<1>;

  public:
    iterator() = delete;

    constexpr iterator(const iterator& source) = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr iterator& operator=(const iterator& source) = default;
    constexpr iterator& operator=(iterator&& source) noexcept = default;

    constexpr auto operator++() -> iterator&;
    constexpr auto operator++(int) -> iterator;

    constexpr auto operator==(const iterator& other) const;
    constexpr auto operator!=(const iterator& other) const;

    constexpr auto operator*() const -> reference;
    constexpr auto operator-> () const -> pointer { return nullptr; }

  private:
    iterator(const base_slice* slice, uint index) : slice_{slice}, indexes_{index} {}

    const base_slice* slice_;
    uint indexes_;
  };

  /// Unsigned integer that represents all possibles extents when applied to an array.
  static constexpr auto all = uint{0u};

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0u.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to `base_slice<N>::all`.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to consistent strides based on the `extents`.
  constexpr base_slice(uint start, uint extent, uint stride) : start{start}, extent{extent}, stride{stride} {}

  /// \group Constructor
  constexpr base_slice(uint start, uint extent) : start{start}, extent{extent} {}

  /// \group Constructor
  constexpr base_slice() = default;

  constexpr base_slice(const base_slice& source) = default;
  constexpr base_slice(base_slice&& source) noexcept = default;

  constexpr auto operator=(const base_slice& source) -> base_slice& = default;
  constexpr auto operator=(base_slice&& source) noexcept -> base_slice& = default;

  /// Effectively the product of the extents.
  constexpr auto size() const { return extent; }

  auto operator()(uint index) const -> uint { return start + index * stride; }

  auto begin() const -> iterator { return cbegin(); }
  auto end() const -> iterator { return cend(); }

  auto cbegin() const -> iterator { return {this, 0u}; }
  auto cend() const -> iterator { return {this, extent}; }

  uint start = 0u;   //< Start position.
  uint extent = all; //< Number of position.
  uint stride = 0u;  //< Skip positions.
};

} // namespace jules

#endif // JULES_ARRAY_SLICE_H
