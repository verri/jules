// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_REF_ARRAY_H
/// \exclude
#define JULES_ARRAY_REF_ARRAY_H

#include <jules/array/descriptor.hpp>
#include <jules/array/slicing.hpp>
// #include <jules/array/detail/common.hpp>
// #include <jules/array/drop.hpp>
#include <jules/array/meta/common.hpp>
// #include <jules/array/reshape.hpp>
#include <jules/array/mapped_ref_array.hpp>
#include <jules/core/type.hpp>

namespace jules
{

/// Array with non-owned, continuous data.
///
/// This class represents a ref view of an concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T, std::size_t N> class ref_array
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class array;
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
  using iterator = value_type*;

  /// \group member_types Class Types and Constants
  using const_iterator = const value_type*;

  /// \group member_types Class Types and Constants
  using size_type = index_t;

  /// \group member_types Class Types and Constants
  using difference_type = distance_t;

  constexpr ref_array() noexcept = default;

  ref_array(value_type* data, descriptor<order> descriptor) : data_{data}, descriptor_{descriptor} {}
  ref_array(const ref_array& source) = default;
  ref_array(ref_array&& source) noexcept = default;

  ~ref_array() = default;

  /// \group Assignment
  auto operator=(const common_array auto& source) -> ref_array&
  {
    detail::array_assign(*this, source);
    return *this;
  }

  /// \group Assignment
  template <typename U>
  requires(!common_array<U>) auto operator=(const U& source) -> ref_array&
  {
    static_assert(std::is_assignable<value_type&, U>::value, "incompatible assignment");
    for (auto& elem : *this)
      elem = source;
    return *this;
  }

  /// \group Assignment
  auto operator=(const ref_array& source) -> ref_array&
  {
    if (this != &source)
      detail::array_assign(*this, source);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator ref_array<const value_type, order>() const { return {data(), descriptor_}; }

  // operator mapped_ref_array<value_type, identity_mapper<order>>() { return {data(), {{0u, dimensions()}}}; }

  // operator mapped_ref_array<const value_type, identity_mapper<order>>() const { return {data(), {{0u, dimensions()}}}; }

  /// \group Indexing
  decltype(auto) operator[](index_t index) { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](index_t index) const { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](absolute_slice index) { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](absolute_slice index) const { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](absolute_strided_slice index) { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](absolute_strided_slice index) const { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](bounded_slice index) { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](bounded_slice index) const { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](bounded_strided_slice index) { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](bounded_strided_slice index) const { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](valid_slice auto index) { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  template <typename F> decltype(auto) operator[](valid_slice auto index) const
  {
    return detail::forward_slicing<N>(data(), descriptor_, index);
  }

  /// \group Indexing
  decltype(auto) operator[](every_index index) { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](every_index index) const { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](index_span index) { return detail::forward_slicing<N>(data(), descriptor_, index); }

  /// \group Indexing
  decltype(auto) operator[](index_span index) const { return detail::forward_slicing<N>(data(), descriptor_, index); }

  auto begin() noexcept -> iterator { return data(); }
  auto end() noexcept -> iterator { return data() + size(); }

  auto begin() const noexcept -> const_iterator { return cbegin(); }
  auto end() const noexcept -> const_iterator { return cend(); }

  auto cbegin() const noexcept -> const_iterator { return data(); }
  auto cend() const noexcept -> const_iterator { return data() + size(); }

  auto size() const noexcept { return descriptor_.size(); }

  auto dimensions() const noexcept { return descriptor_.dimensions(); }

private:
  auto data() -> value_type* { return data_; }

  auto data() const -> const value_type* { return data_; }

  /// \exclude
  value_type* data_ = nullptr;

  /// \exclude
  descriptor<order> descriptor_ = {};
};

template <typename T, std::size_t N> ref_array(T*, descriptor<N>) -> ref_array<T, N>;

template <typename T, std::size_t N> auto eval(ref_array<T, N> source) -> ref_array<T, N> { return source; }

template <std::size_t D, typename T, std::size_t N> decltype(auto) drop_to(ref_array<T, N> source)
{
  if constexpr (D == 0) {
    DEBUG_ASSERT(source.size() == 1, debug::default_module, debug::level::invalid_argument, "array cannot be coerced to scalar");
    return *source.data();
  } else
    return ref_array<T, D>{source.begin(), {detail::template drop_one_level_extents<D>(source.dimensions())}};
}

template <typename T, std::size_t N> decltype(auto) drop(ref_array<T, N> source) { return drop_to<1>(source); }

template <typename T, std::size_t N> auto flatten(ref_array<T, N> source) noexcept
{
  return ref_array<T, 1>(source.begin(), {{source.size()}});
}

template <std::size_t D, typename T, std::size_t N>
auto reshape_to(ref_array<T, N> source, std::array<index_t, D> extents) noexcept -> ref_array<T, D>
{
  DEBUG_ASSERT(source.size() == prod(extents), debug::default_module, debug::level::invalid_argument, "invalid dimensions");
  return ref_array<T, D>(source.begin(), {{extents}});
}

} // namespace jules

#endif // JULES_ARRAY_REF_ARRAY_H
