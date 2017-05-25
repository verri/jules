// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_STRIDED_REF_ARRAY_H
/// \exclude
#define JULES_ARRAY_STRIDED_REF_ARRAY_H

#include <jules/array/detail/iterator.hpp>
#include <jules/array/mapper.hpp>
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
template <typename T, std::size_t N, typename Mapper = identity_mapper>
class strided_ref_array : public common_array_base<strided_ref_array<T, N, Mapper>>
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t, typename> friend class strided_ref_array;

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

  strided_ref_array(value_type* data, strided_descriptor<order> strided_descriptor, Mapper mapper = {})
    : data_{data}, descriptor_{strided_descriptor}, mapper_{std::move(mapper)}
  {
  }

  ~strided_ref_array() = default;

  /// \group Assignment
  template <typename Array> auto operator=(const common_array_base<Array>& source) -> strided_ref_array&
  {
    static_assert(Array::order == order, "array order mismatch");
    static_assert(std::is_assignable<value_type&, typename Array::value_type>::value, "incompatible assignment");
    assign_from_array(source);
    return *this;
  }

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> strided_ref_array&
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
  operator strided_ref_array<const value_type, order, Mapper>() const { return {data_, descriptor_, mapper_}; }

  /// \group Indexing
  decltype(auto) operator[](size_type i)
  {
    DEBUG_ASSERT(i < descriptor_.extents[0], debug::default_module, debug::level::boundary_check, "out of range");
    return at(data_, descriptor_, mapper_, i);
  }

  /// \group Indexing
  decltype(auto) operator[](size_type i) const
  {
    DEBUG_ASSERT(i < descriptor_.extents[0], debug::default_module, debug::level::boundary_check, "out of range");
    return at(data_, descriptor_, mapper_, i);
  }

  auto begin() noexcept -> iterator { return {data_, descriptor_.begin()}; }
  auto end() noexcept -> iterator { return {data_, descriptor_.end()}; }

  auto begin() const noexcept -> const_iterator { return cbegin(); }
  auto end() const noexcept -> const_iterator { return cend(); }

  auto cbegin() const noexcept -> const_iterator { return {data_, descriptor_.begin()}; }
  auto cend() const noexcept -> const_iterator { return {data_, descriptor_.end()}; }

  auto size() const noexcept { return descriptor_.size(); }

  auto length() const noexcept { return descriptor_.length(); }

  auto row_count() const noexcept { return descriptor_.row_count(); }

  auto column_count() const noexcept { return descriptor_.column_count(); }

  auto dimensions() const noexcept -> std::array<size_type, order> { return descriptor_.extents; }

protected:
  template <typename Array> auto assign_from_array(const common_array_base<Array>& source)
  {
    DEBUG_ASSERT(this->dimensions() == source.dimensions(), debug::default_module, debug::level::extents_check,
                 "dimensions mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::default_module, debug::level::unreachable, "should never happen");
  }

  // Static so I do not need to implement for const and non-const.
  template <typename U, std::size_t M>
  static decltype(auto) at(U* data, const strided_descriptor<M>& desc, const Mapper& mapper, size_type i)
  {
    // clang-format off
    if constexpr (M == 1) {
      const auto pos = mapper(desc({{i}}));
      return data[pos];
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

  /// \exclude
  Mapper mapper_;
};

template <typename T, std::size_t N> auto eval(const strided_ref_array<T, N>& source) -> const strided_ref_array<T, N>&
{
  return source;
}

} // namespace jules

#endif // JULES_ARRAY_STRIDED_REF_ARRAY_H
