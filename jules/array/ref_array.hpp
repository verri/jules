// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_REF_ARRAY_H
/// \exclude
#define JULES_ARRAY_REF_ARRAY_H

#include <jules/array/descriptor.hpp>
#include <jules/array/detail/common.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/strided_ref_array.hpp>
#include <jules/core/type.hpp>

namespace jules
{

/// Array with non-owned, continuous data.
///
/// This class represents a ref view of an concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T, std::size_t N> class ref_array : public common_array_base<ref_array<T, N>>
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class ref_array;

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
  using iterator = T*;

  /// \group member_types Class Types and Constants
  using const_iterator = const T*;

  /// \group member_types Class Types and Constants
  using size_type = index_t;

  /// \group member_types Class Types and Constants
  using difference_type = distance_t;

  ref_array(value_type* data, descriptor<order> descriptor) : data_{data}, descriptor_{descriptor} {}

  ~ref_array(){};

  /// \group Assignment
  template <typename Array> auto operator=(const common_array_base<Array>& source) -> ref_array&
  {
    detail::array_assign(*this, source);
    return *this;
  }

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> ref_array&
  {
    static_assert(std::is_assignable<value_type&, U>::value, "incompatible assignment");
    for (auto& elem : *this)
      elem = source;
    return *this;
  }

  /// \group Assignment
  auto operator=(const ref_array& source) -> ref_array&
  {
    detail::array_assign(*this, source);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator ref_array<const value_type, order>() const { return {data(), descriptor_}; }

  operator strided_ref_array<value_type, identity_mapper<order>>() { return {data(), {{0u, dimensions()}}}; }

  operator strided_ref_array<const value_type, identity_mapper<order>>() const { return {data(), {{0u, dimensions()}}}; }

  /// \group Indexing
  decltype(auto) operator[](size_type i) { return at(data(), descriptor_, i); }

  /// \group Indexing
  decltype(auto) operator[](size_type i) const { return at(data(), descriptor_, i); }

  auto begin() noexcept -> iterator { return data(); }
  auto end() noexcept -> iterator { return data() + size(); }

  auto begin() const noexcept -> const_iterator { return cbegin(); }
  auto end() const noexcept -> const_iterator { return cend(); }

  auto cbegin() const noexcept -> const_iterator { return data(); }
  auto cend() const noexcept -> const_iterator { return data() + size(); }

  auto size() const noexcept { return descriptor_.size(); }

  auto length() const noexcept { return descriptor_.length(); }

  auto row_count() const noexcept { return descriptor_.row_count(); }

  auto column_count() const noexcept { return descriptor_.column_count(); }

  auto dimensions() const noexcept -> std::array<size_type, order> { return descriptor_.extents; }

protected:
  auto data() -> value_type* { return data_; }

  auto data() const -> const value_type* { return data_; }

  auto as_strided() -> strided_ref_array<value_type, identity_mapper<order>> { return *this; }

  auto as_strided() const -> strided_ref_array<const value_type, identity_mapper<order>> { return *this; }

  template <typename U, std::size_t M> static decltype(auto) at(U* data, const descriptor<M>& desc, size_type i)
  {
    detail::assert_in_bound(i, desc.extents[0]);
    // clang-format off
    if constexpr (M == 1) {
      return data[i];
    } else {
      const auto new_desc = strided_descriptor<M>{i, desc.extents};
      return strided_ref_array<U, identity_mapper<M - 1>>{data, {new_desc.drop_dimension()}};
    }
    // clang-format on
  }

  ref_array(const ref_array& source) = default;
  ref_array(ref_array&& source) noexcept = default;

  /// \exclude
  value_type* const data_;

  /// \exclude
  const descriptor<order> descriptor_;
};

template <typename T, std::size_t N> auto eval(const ref_array<T, N>& source) -> const ref_array<T, N>& { return source; }

} // namespace jules

#endif // JULES_ARRAY_REF_ARRAY_H
