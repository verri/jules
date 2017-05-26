// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_STRIDED_REF_ARRAY_H
/// \exclude
#define JULES_ARRAY_STRIDED_REF_ARRAY_H

#include <jules/array/detail/common.hpp>
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
    detail::array_assign(source, *this);
    return *this;
  }

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> strided_ref_array&
  {
    static_assert(std::is_assignable<value_type&, U>::value, "incompatible assignment");
    range::fill(*this, source);
    return *this;
  }

  /// \group Assignment
  auto operator=(const strided_ref_array& source) -> strided_ref_array&
  {
    detail::array_assign(source, *this);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator strided_ref_array<const value_type, order, Mapper>() const { return {data(), descriptor_, mapper_}; }

  /// \group Indexing
  decltype(auto) operator[](size_type i) { return at(data(), descriptor_, mapper_, i); }

  /// \group Indexing
  decltype(auto) operator[](size_type i) const { return at(data(), descriptor_, mapper_, i); }

  auto begin() noexcept -> iterator { return {data(), descriptor_.begin()}; }
  auto end() noexcept -> iterator { return {data(), descriptor_.end()}; }

  auto begin() const noexcept -> const_iterator { return cbegin(); }
  auto end() const noexcept -> const_iterator { return cend(); }

  auto cbegin() const noexcept -> const_iterator { return {data(), descriptor_.begin()}; }
  auto cend() const noexcept -> const_iterator { return {data(), descriptor_.end()}; }

  auto size() const noexcept { return descriptor_.size(); }

  auto length() const noexcept { return descriptor_.length(); }

  auto row_count() const noexcept { return descriptor_.row_count(); }

  auto column_count() const noexcept { return descriptor_.column_count(); }

  auto dimensions() const noexcept -> std::array<size_type, order> { return descriptor_.extents; }

protected:
  auto data() -> value_type* { return data_; }

  auto data() const -> const value_type* { return data_; }

  template <typename U, std::size_t M>
  static decltype(auto) at(U* data, const strided_descriptor<M>& desc, const Mapper& mapper, size_type i)
  {
    detail::assert_in_bound(i, desc.extents[0]);
    // clang-format off
    if constexpr (M == 1) {
      const auto pos = mapper(desc(i));
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
  value_type* const data_;

  /// \exclude
  const strided_descriptor<order> descriptor_;

  /// \exclude
  const Mapper mapper_;
};

template <typename T, std::size_t N> auto eval(const strided_ref_array<T, N>& source) -> const strided_ref_array<T, N>&
{
  return source;
}

} // namespace jules

#endif // JULES_ARRAY_STRIDED_REF_ARRAY_H
