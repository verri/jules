// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_CONTIGUOUS_ARRAY_H
#define JULES_ARRAY_CONTIGUOUS_ARRAY_H

#include <jules/array/ref_array.hpp>

namespace jules
{

template <typename T, std::size_t N> class contiguous_array : public ref_array<T, N>
{
  template <typename, std::size_t> friend class contiguous_array;

public:
  using value_type = T;

  static constexpr auto order = N;

  using iterator = T*;

  using const_iterator = const T*;

  using size_type = index_t;

  using difference_type = distance_t;

  ~contiguous_array() = default;
  contiguous_array() = default;
  contiguous_array(T* data, typename base_slice<N>::extent_type extents) : ref_array<T, N>{data, {0u, extents}} {}

  contiguous_array(const contiguous_array& source) = default;
  contiguous_array(contiguous_array&& source) noexcept = default;

  auto operator=(const contiguous_array& source) -> contiguous_array& = delete;
  auto operator=(contiguous_array&& source) noexcept -> contiguous_array& = delete;

  operator contiguous_array<const T, N>() const { return {this->data(), this->extents()}; }

  /// \group Begin
  /// Returns an iterator to the first element of the array.
  auto begin() -> iterator { return this->data(); }

  /// \group End
  /// Returns an iterator to the element following the last element of the container.
  auto end() -> iterator { return this->data() + this->size(); }

  /// \group Begin
  auto begin() const -> const_iterator { return this->cbegin(); }

  /// \group End
  auto end() const -> const_iterator { return this->cend(); }

  /// \group Begin
  auto cbegin() const -> const_iterator { return this->data(); }

  /// \group End
  auto cend() const -> const_iterator { return this->data() + this->size(); }

  /// \group Data
  /// Returns pointer to the underlying array serving as element storage.
  auto data() -> value_type* { return this->data_; }

  /// \group Data
  auto data() const -> const value_type* { return this->data_; }
};

} // namespace jules

#endif // JULES_ARRAY_CONTIGUOUS_ARRAY_H
