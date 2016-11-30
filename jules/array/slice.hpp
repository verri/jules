#ifndef JULES_ARRAY_SLICE_H
#define JULES_ARRAY_SLICE_H

#include <jules/base/numeric.hpp>
#include <jules/core/type.hpp>

#include <array>

namespace jules
{

/// \exclude
namespace detail
{

template <typename T, std::size_t N> struct array_or_plain {
  using type = std::array<T, N>;
};

template <typename T> struct array_or_plain<T, 1> {
  using type = T;
};

template <typename T, std::size_t N> using array_or_plain_t = typename array_or_plain<T, N>::type;

// TODO: C++17: use constexpr if
template <typename T, std::size_t N> struct filled_array_or_plain {
  static constexpr auto value(const T& value) { return repeat<N>(value); }
};

template <typename T> struct filled_array_or_plain<T, 1> {
  static constexpr auto value(const T& value) { return value; }
};

template <typename T, std::size_t N> constexpr auto filled_array_or_plain_v(const T& value)
{
  return filled_array_or_plain<T, N>::value(value);
}
} // namespace detail

/// Array slicing and dimensions representation.
/// `base_slice` is `TriviallyCopyable` and `TriviallyMovable`.
/// \module N-Dimensional Array
template <std::size_t N> class base_slice
{
  static_assert(N > 0, "Slice cannot have dimension 0.");

  using uints_t = detail::array_or_plain_t<uint, N>;

public:
  /// One or more unsigned integers that represents all possibles extents when
  /// applied to an array.
  static constexpr auto all = detail::filled_array_or_plain_v<uint, N>(0u);

  /// \group Constructor
  /// \param start Start position of the slicing. It defaults to 0u.
  /// \param extents Size of the slicing in each dimension.
  ///   It defaults to `base_slice<N>::all`.
  /// \param strides Number of skip positions in each dimension.
  ///   It defaults to consistent strides based on the `extents`.
  constexpr base_slice(uint start, uints_t extents, uints_t strides) : start{start}, extents{extents}, strides{strides} {}

  /// \group Constructor
  constexpr base_slice(uint start, uints_t extents) : start{start}, extents{extents}
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

  constexpr base_slice& operator=(const base_slice& source) = default;
  constexpr base_slice& operator=(base_slice&& source) noexcept = default;

  constexpr auto size() const { return prod(extents); }

  uint start = 0u;                                                //< Start position.
  uints_t extents = all;                                          //< Size in each dimension.
  uints_t strides = detail::filled_array_or_plain_v<uint, N>(1u); //< Skip in each dimension.
};

} // namespace jules

#endif // JULES_ARRAY_SLICE_H
