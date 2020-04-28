// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_STRIDED_REF_ARRAY_H
/// \exclude
#define JULES_ARRAY_STRIDED_REF_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/detail/iterator.hpp>
#include <jules/array/drop.hpp>
#include <jules/array/mapper.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/meta/reference.hpp>
#include <jules/array/slicing.hpp>
#include <jules/array/slicing/absolute.hpp>
#include <jules/array/slicing/bounded.hpp>
#include <jules/array/strided_descriptor.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

// TODO: change name to mapped_ref_array (should ref_array be a special case of
// mapped_ref_array?)

/// Array with non-owned, non-continuous data.
///
/// This class represents a strided_ref view of an concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T, typename Mapper> class strided_ref_array
{
  static_assert(Mapper::order > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class array;
  template <typename, typename> friend class strided_ref_array;

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
  static constexpr auto order = Mapper::order;

  /// \group member_types Class Types and Constants
  using value_type = T;

  /// \group member_types Class Types and Constants
  using iterator = detail::iterator_from_indexes<T, typename Mapper::iterator>;

  /// \group member_types Class Types and Constants
  using const_iterator = detail::iterator_from_indexes<const T, typename Mapper::iterator>;

  /// \group member_types Class Types and Constants
  using size_type = index_t;

  /// \group member_types Class Types and Constants
  using difference_type = distance_t;

  constexpr strided_ref_array() noexcept = default;

  strided_ref_array(value_type* data, Mapper mapper) : mapper_{std::move(mapper)}, data_{data} {}
  strided_ref_array(const strided_ref_array& source) = default;
  strided_ref_array(strided_ref_array&& source) noexcept = default;

  ~strided_ref_array() = default;

  /// \group Assignment
  auto operator=(const common_array auto& source) -> strided_ref_array&
  {
    detail::array_assign(*this, source);
    return *this;
  }

  /// \group Assignment
  template <typename U> requires(!common_array<U>) auto operator=(const U& source) -> strided_ref_array&
  {
    static_assert(std::is_assignable<value_type&, U>::value, "incompatible assignment");
    ranges::fill(*this, source);
    return *this;
  }

  /// \group Assignment
  auto operator=(const strided_ref_array& source) -> strided_ref_array&
  {
    if (this != &source)
      detail::array_assign(*this, source);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator strided_ref_array<const value_type, Mapper>() const { return {data(), mapper()}; }

  /// \group Indexing
  decltype(auto) operator[](index_t index) { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](index_t index) const { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](absolute_slice index) { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](absolute_slice index) const { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](absolute_strided_slice index) { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](absolute_strided_slice index) const
  {
    return detail::forward_slicing<order>(data(), mapper(), index);
  }

  /// \group Indexing
  decltype(auto) operator[](bounded_slice index) { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](bounded_slice index) const { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](bounded_strided_slice index) { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](bounded_strided_slice index) const { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](valid_slice auto index) { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  template <typename F> decltype(auto) operator[](valid_slice auto index) const
  {
    return detail::forward_slicing<order>(data(), mapper(), index);
  }

  /// \group Indexing
  decltype(auto) operator[](every_index index) { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](every_index index) const { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](index_span index) { return detail::forward_slicing<order>(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](index_span index) const { return detail::forward_slicing<order>(data(), mapper(), index); }

  auto begin() noexcept -> iterator { return {data(), mapper().index_begin()}; }
  auto end() noexcept -> iterator { return {data(), mapper().index_end()}; }

  auto begin() const noexcept -> const_iterator { return cbegin(); }
  auto end() const noexcept -> const_iterator { return cend(); }

  auto cbegin() const noexcept -> const_iterator { return {data(), mapper().index_begin()}; }
  auto cend() const noexcept -> const_iterator { return {data(), mapper().index_end()}; }

  auto size() const noexcept { return mapper().size(); }

  auto dimensions() const noexcept -> std::array<size_type, order> { return mapper().dimensions(); }

  template <std::size_t D> decltype(auto) drop_to()
  {
    if constexpr (D == 0) {
      DEBUG_ASSERT(this->size() == 1, debug::default_module, debug::level::invalid_argument, "array cannot be coerced to scalar");
      return *this->data();
    } else {
      auto mapper = this->mapper().template drop_one_level_dimensions<D>();
      return strided_ref_array<T, decltype(mapper)>{this->data(), std::move(mapper)};
    }
  }

private:
  auto mapper() const -> const Mapper& { return mapper_; }

  auto data() -> value_type* { return data_; }

  auto data() const -> const value_type* { return data_; }

  /// \exclude
  Mapper mapper_;
  value_type* const data_ = nullptr;
};

template <typename T, typename Mapper> strided_ref_array(T*, Mapper) -> strided_ref_array<T, Mapper>;

template <typename T, typename Mapper> auto eval(strided_ref_array<T, Mapper> source) -> strided_ref_array<T, Mapper>
{
  return source;
}

template <std::size_t D, typename U, typename Mapper> decltype(auto) drop_to(strided_ref_array<U, Mapper> source)
{
  return source.template drop_to<D>();
}

template <typename U, typename Mapper> decltype(auto) drop(strided_ref_array<U, Mapper> source) { return drop_to<1>(source); }

} // namespace jules

#endif // JULES_ARRAY_STRIDED_REF_ARRAY_H
