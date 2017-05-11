// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICED_ARRAY_H
/// \exclude
#define JULES_ARRAY_SLICED_ARRAY_H

#include <jules/core/type.hpp>
#include <jules/core/debug.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/detail/iterator.hpp>
#include <jules/array/descriptor.hpp>

namespace jules
{

/// Array with non-owned, non-continous data.
///
/// This class represents a sliced view of an concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T, std::size_t N> class sliced_array
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class sliced_array;

  /// TODO: maybe unify things here
};

/// 1-D Array with non-owned, non-continous data specialization.
///
/// This class represents a sliced view of a concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T> class sliced_array<T, 1>
{
  template <typename, std::size_t> friend class sliced_array;

public:
  /// \group member_types Class Types and Constants
  ///
  /// (1) The order of dimensionality.
  ///
  /// (2) The type of the elements.
  ///
  /// (3) `ForwardIterator` over the elements.
  ///
  /// (4) Constant `ForwardIterator` over the elements.
  ///
  /// (5) Unsigned integer type that can store the dimensions of the array.
  ///
  /// (6) Signed integer type that can store differences between sizes.
  static constexpr auto order = std::size_t{1u};

  /// \group member_types Class Types and Constants
  using value_type = T;

  /// \group member_types Class Types and Constants
  using iterator = detail::iterator_from_indexes<T, typename descriptor<1>::iterator>;

  /// \group member_types Class Types and Constants
  using const_iterator = detail::iterator_from_indexes<const T, typename descriptor<1>::iterator>;

  /// \group member_types Class Types and Constants
  using size_type = index_t;

  /// \group member_types Class Types and Constants
  using difference_type = distance_t;

  sliced_array(T* data, descriptor<1> descriptor) : data_{data}, descriptor_{descriptor} {}

  ~sliced_array() = default;

  /// \group Assignment
  template <typename Array> auto operator=(const Array& source) -> meta::requires_t<sliced_array&, CommonArray<Array>>
  {
    static_assert(Array::order == 1, "array order mismatch");
    static_assert(std::is_assignable<T&, typename Array::value_type>::value, "incompatible assignment");

    DEBUG_ASSERT(this->dimensions() == source.dimensions(), debug::default_module, debug::level::extents_check, "dimensions mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::default_module, debug::level::unreachable, "should never happen");

    return *this;
  }

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> meta::fallback_t<sliced_array&, Array<U>>
  {
    static_assert(std::is_assignable<T&, U>::value, "incompatible assignment");
    for (auto& elem : *this)
      elem = source;
    return *this;
  }

  /// \group Assignment
  auto operator=(const sliced_array<T, 1>& source) -> sliced_array&
  {
    DEBUG_ASSERT(this->dimensions() == source.dimensions(), debug::default_module, debug::level::extents_check, "dimensions mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::default_module, debug::level::unreachable, "should never happen");

    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator sliced_array<const T, 1>() const { return {data_, descriptor_}; }

  /// \group Indexing
  auto operator[](index_t i) -> T&
  {
    DEBUG_ASSERT(i <= length(), debug::default_module, debug::level::boundary_check, "out of range");
    return data_[descriptor_({{i}})];
  }

  /// \group Indexing
  auto operator[](index_t i) const -> const T&
  {
    DEBUG_ASSERT(i <= length(), debug::default_module, debug::level::boundary_check, "out of range");
    return data_[descriptor_({{i}})];
  }

  auto begin() -> iterator { return {data_, descriptor_.begin()}; }
  auto end() -> iterator { return {data_, descriptor_.end()}; }

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return {data_, descriptor_.begin()}; }
  auto cend() const -> const_iterator { return {data_, descriptor_.end()}; }

  auto size() const -> index_t { return descriptor_.size(); }

  auto length() const -> index_t { return size(); }

  auto dimensions() const -> std::array<index_t, 1ul> { return descriptor_.extents; }

protected:
  sliced_array() = default;
  sliced_array(const sliced_array& source) = default;
  sliced_array(sliced_array&& source) noexcept = default;

  /// \exclude
  T* data_;

  /// \exclude
  descriptor<1> descriptor_;
};

template <typename T, std::size_t N>
auto eval(const sliced_array<T, N>& source) -> const sliced_array<T, N>&
{
  return source;
}

} // namespace jules

#endif // JULES_ARRAY_SLICED_ARRAY_H
