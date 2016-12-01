#ifndef JULES_ARRAY_REF_ARRAY_H
#define JULES_ARRAY_REF_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/core/type.hpp>

namespace jules
{

/// Array reference.
///
/// This class is used internally by `jules` to represent a view of an concrete array.
///
/// \module N-Dimensional Array
template <typename T, std::size_t N> class ref_array
{
  static_assert(N > 1, "Invalid array dimension.");

public:
  using value_type = T;
  static constexpr auto order = N;

  using size_type = uint;
  using difference_type = sint;

  /// *TODO*: Explain why the user should probably not call this function.
  /// In C++17, we can provide a helper that generates a view with more security.
  ref_array(T* data, base_slice<N>& descriptor) : data_{data}, descriptor_{descriptor} {}

  ref_array(const ref_array& source) = delete;
  ref_array(ref_array&& source) noexcept = delete;

  ~ref_array() = default;

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> detail::not_array_request<ref_array&, U>;

  /// \group Assignment
  template <typename Array> auto operator=(const Array& source) -> detail::array_request<ref_array&, Array>;

  /// \group Assignment
  auto operator=(base_array<T, N>&& source) noexcept -> ref_array&;

private:
  T* data_;
  base_slice<N> descriptor_;
};

} // namespace jules

#endif // JULES_ARRAY_REF_ARRAY_H
