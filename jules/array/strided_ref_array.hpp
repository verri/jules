// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_STRIDED_REF_ARRAY_H
/// \exclude
#define JULES_ARRAY_STRIDED_REF_ARRAY_H

#include <jules/array/detail/iterator.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/strided_descriptor.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

/// Array with non-owned, non-continuous data.
///
/// This class represents a strided_ref view of an concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T, std::size_t N> class strided_ref_array
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class strided_ref_array;

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
  static constexpr auto order = N;

  /// \group member_types Class Types and Constants
  using value_type = T;

  /// \group member_types Class Types and Constants
  using iterator = detail::iterator_from_indexes<T, typename strided_descriptor<order>::iterator>;

  /// \group member_types Class Types and Constants
  using const_iterator = detail::iterator_from_indexes<const T, typename strided_descriptor<order>::iterator>;

  /// \group member_types Class Types and Constants
  using size_type = index_t;

  /// \group member_types Class Types and Constants
  using difference_type = distance_t;

  strided_ref_array(value_type* data, strided_descriptor<order> strided_descriptor) : data_{data}, descriptor_{strided_descriptor}
  {
  }

  ~strided_ref_array() = default;

  /// \group Assignment
  template <typename Array> auto operator=(const Array& source) -> meta::requires_t<strided_ref_array&, CommonArray<Array>>
  {
    static_assert(Array::order == order, "array order mismatch");
    static_assert(std::is_assignable<value_type&, typename Array::value_type>::value, "incompatible assignment");
    assign_from_array(source);
    return *this;
  }

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> meta::fallback_t<strided_ref_array&, Array<U>>
  {
    static_assert(std::is_assignable<value_type&, U>::value, "incompatible assignment");
    for (auto& elem : *this)
      elem = source;
    return *this;
  }

  /// \group Assignment
  auto operator=(const strided_ref_array& source) -> strided_ref_array&
  {
    assign_from_array(source);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator strided_ref_array<const value_type, order>() const { return {data_, descriptor_}; }

  /// \group Indexing
  decltype(auto) operator[](index_t i)
  {
    DEBUG_ASSERT(i < descriptor_.extents[0], debug::default_module, debug::level::boundary_check, "out of range");
    return at(data_, descriptor_, i);
  }

  /// \group Indexing
  decltype(auto) operator[](index_t i) const
  {
    DEBUG_ASSERT(i < descriptor_.extents[0], debug::default_module, debug::level::boundary_check, "out of range");
    return at(data_, descriptor_, i);
  }

  auto begin() -> iterator { return {data_, descriptor_.begin()}; }
  auto end() -> iterator { return {data_, descriptor_.end()}; }

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return {data_, descriptor_.begin()}; }
  auto cend() const -> const_iterator { return {data_, descriptor_.end()}; }

  auto size() const -> size_type { return descriptor_.size(); }

  template <typename _ = void>
  auto length() const -> meta::requires_t<size_type, meta::always_true<_>, std::bool_constant<(order == 1ul)>>
  {
    return descriptor_.extents[0u];
  }

  auto dimensions() const -> std::array<index_t, order> { return descriptor_.extents; }

protected:
  template <typename Array> auto assign_from_array(const Array& source)
  {
    DEBUG_ASSERT(this->dimensions() == source.dimensions(), debug::default_module, debug::level::extents_check,
                 "dimensions mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::default_module, debug::level::unreachable, "should never happen");
  }

  template <typename U, std::size_t M> static decltype(auto) at(U* data, const strided_descriptor<M>& desc, index_t i)
  {
    // clang-format off
    if constexpr (M == 1) {
      return data[desc({{i}})];
    } else {
      return; // TODO...
    }
    // clang-format on
  }

  strided_ref_array() = default;
  strided_ref_array(const strided_ref_array& source) = default;
  strided_ref_array(strided_ref_array&& source) noexcept = default;

  /// \exclude
  value_type* data_;

  /// \exclude
  strided_descriptor<order> descriptor_;
};

template <typename T, std::size_t N> auto eval(const strided_ref_array<T, N>& source) -> const strided_ref_array<T, N>&
{
  return source;
}

} // namespace jules

#endif // JULES_ARRAY_STRIDED_REF_ARRAY_H
