#ifndef JULES_ARRAY_SLICE_H
#define JULES_ARRAY_SLICE_H

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
}

/// Array slicing and dimensions representation.
/// \module N-Dimensional Array
template <std::size_t N> class base_slice
{
  static_assert(N > 0, "Slice cannot have dimension 0.");

  using uints = detail::array_or_plain_t<uint, N>;

public:
  base_slice() = default;

  base_slice(uint start, uints extents);
  base_slice(uint start, uints extents, uints strides);

  base_slice(const base_slice& source) = default;
  base_slice(base_slice&& source) noexcept = default;

  base_slice& operator=(const base_slice& source) = default;
  base_slice& operator=(base_slice&& source) noexcept = default;
};

} // namespace jules

#endif // JULES_ARRAY_SLICE_H
