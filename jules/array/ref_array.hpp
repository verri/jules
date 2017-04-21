// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_REF_ARRAY_H
/// \exclude
#define JULES_ARRAY_REF_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/detail/iterator.hpp>
#include <jules/array/detail/slicing.hpp>
#include <jules/array/slice.hpp>
#include <jules/base/const_vector.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>
#include <jules/core/type.hpp>

namespace jules
{

/// Array reference.
///
/// This class represents a view of an concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T, std::size_t N> class ref_array
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

  /// \group member_types
  using value_type = T;

  /// \group member_types
  using iterator = detail::iterator_from_indexes<T, typename base_slice<N>::iterator>;

  /// \group member_types
  using const_iterator = detail::iterator_from_indexes<const T, typename base_slice<N>::iterator>;

  /// \group member_types
  using size_type = index_t;

  /// \group member_types
  using difference_type = distance_t;

  /// *TODO*: Explain why the user should probably not call these functions.
  /// In C++17, we can provide a helper that generates a view with more security.
  ref_array() = default;
  ref_array(T* data, base_slice<N> descriptor) : data_{data}, descriptor_{descriptor} {}
  ref_array(const ref_array& source) = default;
  ref_array(ref_array&& source) noexcept = default;

  ~ref_array() = default;

  /// \group Assignment
  template <typename A> auto operator=(const A& source) -> meta::requires_t<ref_array&, Array<A>>
  {
    static_assert(A::order == N, "array order mismatch");
    static_assert(std::is_assignable<T&, typename A::value_type>::value, "incompatible assignment");

    DEBUG_ASSERT(this->extents() == source.extents(), debug::default_module, debug::level::extents_check, "extents mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::default_module, debug::level::unreachable, "should never happen");

    return *this;
  }

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> meta::fallback_t<ref_array&, Array<U>>
  {
    static_assert(std::is_assignable<T&, U>::value, "incompatible assignment");
    for (auto& elem : *this)
      elem = source;
    return *this;
  }

  /// \group Assignment
  auto operator=(base_array<T, N>&& source) noexcept -> ref_array&
  {
    auto it = source.begin();
    for (auto& elem : *this)
      elem = std::move(*it++);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator ref_array<const T, N>() const { return {data_, descriptor_}; }

  /// \group Indexing
  auto operator[](index_t i) -> ref_array<T, N - 1>
  {
    DEBUG_ASSERT(i <= row_count(), debug::default_module, debug::level::boundary_check, "out of range");
    auto descriptor = this->descriptor_.drop_dimension();
    descriptor.start = descriptor_.start + descriptor_.strides[0] * i;
    return {data_, descriptor};
  }

  /// \group Indexing
  auto operator[](index_t i) const -> ref_array<const T, N - 1> { return static_cast<ref_array<const T, N>>(*this)[i]; }

  auto operator()() -> ref_array<T, N>& { return *this; }
  auto operator()() const -> ref_array<const T, N> { return *this; }

  template <typename... Args> auto operator()(Args&&... args) -> detail::indirect_request<ind_array<T, N>, Args...>
  {
    static_assert(sizeof...(args) == N, "invalid number of arguments");
    auto slicing = detail::indirect_slicing(descriptor_, std::forward<Args>(args)...);
    return {data_, slicing.first, std::move(slicing.second)};
  }

  template <typename... Args> auto operator()(Args&&... args) -> detail::slice_request<ref_array<T, N>, Args...>
  {
    static_assert(sizeof...(args) == N, "invalid number of arguments");
    auto slice = detail::default_slicing(descriptor_, std::forward<Args>(args)...);
    return {data_, slice};
  }

  template <typename... Args> auto operator()(Args&&... args) -> detail::element_request<T&, Args...>
  {
    static_assert(sizeof...(args) == N, "invalid number of arguments");
    // TODO: check boundaries
    return data_[descriptor_(index_t{std::forward<Args>(args)}...)];
  }

  template <typename... Args> decltype(auto) operator()(Args&&... args) const
  {
    return static_cast<ref_array<const T, N>>(*this)(std::forward<Args>(args)...);
  }

  auto begin() -> iterator { return {data_, descriptor_.begin()}; }
  auto end() -> iterator { return {data_, descriptor_.end()}; }

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return {data_, descriptor_.begin()}; }
  auto cend() const -> const_iterator { return {data_, descriptor_.end()}; }

  auto descriptor() const -> base_slice<order> { return descriptor_; }
  auto data() const -> value_type* { return data_; }

  /// \exclude return
  auto extents() const { return descriptor_.extents; }

  auto size() const -> index_t { return descriptor_.size(); }

  auto row_count() const -> index_t { return extents()[0]; }
  auto column_count() const -> index_t { return extents()[1]; }

protected:
  /// \exclude
  value_type* data_;

  /// \exclude
  base_slice<order> descriptor_;
};

/// 1-D Array reference specialization.
///
/// This class represents a view of a concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T> class ref_array<T, 1>
{
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
  static constexpr auto order = std::size_t{1u};

  /// \group member_types Class Types and Constants
  using value_type = T;

  /// \group member_types Class Types and Constants
  using iterator = detail::iterator_from_indexes<T, typename base_slice<1>::iterator>;

  /// \group member_types Class Types and Constants
  using const_iterator = detail::iterator_from_indexes<const T, typename base_slice<1>::iterator>;

  /// \group member_types Class Types and Constants
  using size_type = index_t;

  /// \group member_types Class Types and Constants
  using difference_type = distance_t;

  /// *TODO*: Explain why the user should probably not call this function.
  /// In C++17, we can provide a helper that generates a view with more security.
  ref_array() = default;
  ref_array(T* data, base_slice<1> descriptor) : data_{data}, descriptor_{descriptor} {}
  ref_array(const ref_array& source) = default;
  ref_array(ref_array&& source) noexcept = default;

  ~ref_array() = default;

  /// \group Assignment
  template <typename A> auto operator=(const A& source) -> meta::requires_t<ref_array&, Array<A>>
  {
    static_assert(A::order == 1, "array order mismatch");
    static_assert(std::is_assignable<T&, typename A::value_type>::value, "incompatible assignment");

    DEBUG_ASSERT(this->extents() == source.extents(), debug::default_module, debug::level::extents_check, "extents mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::default_module, debug::level::unreachable, "should never happen");

    return *this;
  }

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> meta::fallback_t<ref_array&, Array<U>>
  {
    static_assert(std::is_assignable<T&, U>::value, "incompatible assignment");
    for (auto& elem : *this)
      elem = source;
    return *this;
  }

  /// \group Assignment
  auto operator=(const ref_array<T, 1>& source) -> ref_array&
  {
    DEBUG_ASSERT(this->extents() == source.extents(), debug::default_module, debug::level::extents_check, "extents mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::default_module, debug::level::unreachable, "should never happen");

    return *this;
  }

  /// \group Assignment
  auto operator=(base_array<T, 1>&& source) noexcept -> ref_array&
  {
    auto it = source.begin();
    for (auto& elem : *this)
      elem = std::move(*it++);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator ref_array<const T, 1>() const { return {data_, descriptor_}; }

  /// \group Indexing
  auto operator[](index_t i) -> T&
  {
    DEBUG_ASSERT(i <= length(), debug::default_module, debug::level::boundary_check, "out of range");
    return data_[descriptor_(i)];
  }

  /// \group Indexing
  auto operator[](index_t i) const -> const T&
  {
    DEBUG_ASSERT(i <= length(), debug::default_module, debug::level::boundary_check, "out of range");
    return data_[descriptor_(i)];
  }

  auto operator()() -> ref_array<T, 1>& { return *this; }
  auto operator()() const -> ref_array<const T, 1> { return *this; }

  template <typename Rng, typename U = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
  auto operator()(const Rng& rng) -> ind_array<T, 1>
  {
    static_assert(std::is_convertible<U, index_t>::value, "arbitrary ranges must contain indexes");
    auto slicing = detail::indirect_slicing(this->descriptor_, rng);
    return {data_, slicing.first[0], std::move(slicing.second)};
  }

  auto operator()(typename const_vector<index_t>::container_type indexes) -> ind_array<T, 1>
  {
    for (auto& index : indexes)
      index = this->descriptor()(index);
    return {this->data_, indexes.size(), std::move(indexes)};
  }

  auto operator()(const base_slice<1>& slice) -> ref_array<T, 1>
  {
    auto new_slice = detail::default_slicing(this->descriptor_, slice);
    return {data_, new_slice};
  }

  auto operator()(index_t i) -> T& { return (*this)[i]; }

  template <typename Rng, typename U = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
  auto operator()(const Rng& rng) const -> ind_array<const T, 1>
  {
    static_assert(std::is_convertible<U, index_t>::value, "arbitrary ranges must contain indexes");
    auto slicing = detail::indirect_slicing(this->descriptor_, rng);
    return {data_, slicing.first[0], std::move(slicing.second)};
  }

  auto operator()(typename const_vector<index_t>::container_type indexes) const -> ind_array<const T, 1>
  {
    for (auto& index : indexes)
      index = this->descriptor()(index);
    return {this->data_, indexes.size(), std::move(indexes)};
  }

  auto operator()(const base_slice<1>& slice) const -> ref_array<const T, 1>
  {
    auto new_slice = detail::default_slicing(this->descriptor_, slice);
    return {data_, new_slice};
  }

  auto operator()(index_t i) const -> const T& { return (*this)[i]; }

  auto begin() -> iterator { return {data_, descriptor_.begin()}; }
  auto end() -> iterator { return {data_, descriptor_.end()}; }

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return {data_, descriptor_.begin()}; }
  auto cend() const -> const_iterator { return {data_, descriptor_.end()}; }

  auto descriptor() const -> base_slice<1> { return descriptor_; }
  auto data() const -> value_type* { return data_; }

  auto extents() const -> index_t { return descriptor_.extent; }

  auto size() const -> index_t { return descriptor_.size(); }

  auto length() const -> index_t { return size(); }

protected:
  /// \exclude
  T* data_;

  /// \exclude
  base_slice<1> descriptor_;
};

} // namespace jules

#endif // JULES_ARRAY_REF_ARRAY_H
