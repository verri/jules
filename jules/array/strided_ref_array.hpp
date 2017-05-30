// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_STRIDED_REF_ARRAY_H
/// \exclude
#define JULES_ARRAY_STRIDED_REF_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/detail/iterator.hpp>
#include <jules/array/mapper.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/slicing.hpp>
#include <jules/array/slicing/absolute.hpp>
#include <jules/array/slicing/bounded.hpp>
#include <jules/array/strided_descriptor.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

/// \exclude
template <typename T, typename Mapper, typename... Indexes> class strided_ref_array_proxy
{
  static constexpr auto order = Mapper::order;
  static_assert(sizeof...(Indexes) < order);

public:
  strided_ref_array_proxy(T* data, Mapper mapper, Indexes... indexes)
    : data_{data}, mapper_{std::move(mapper)}, indexes_(std::forward<Indexes>(indexes)...)
  {
  }

  decltype(auto) operator[](index_t i) && { return at(i); }

  decltype(auto) operator[](absolute_slice slice) && { return at(slice); }

  decltype(auto) operator[](absolute_strided_slice slice) && { return at(slice); }

  decltype(auto) operator[](every_index) && { return at(slice(0u, mapper_.descriptor().extents[sizeof...(Indexes)], 1u)); }

  decltype(auto) operator[](bounded_slice slice) && { return at(eval(slice, mapper_.descriptor().extents[sizeof...(Indexes)])); }

  decltype(auto) operator[](bounded_strided_slice slice) &&
  {
    return at(eval(slice, mapper_.descriptor().extents[sizeof...(Indexes)]));
  }

  template <typename Rng, typename = meta::requires<range::SizedRange<Rng>>> decltype(auto) operator[](const Rng& rng) &&
  {
    return at(rng);
  }

private:
  template <typename Index> decltype(auto) at(Index&& index)
  {
    return at_impl(std::forward<Index>(index), std::make_index_sequence<sizeof...(Indexes)>());
  }

  template <typename Index, std::size_t... I> decltype(auto) at_impl(Index&& index, std::index_sequence<I...>)
  {
    // clang-format off
    if constexpr (sizeof...(Indexes) == order - 1) {
      return detail::array_slice(
        data_, std::move(mapper_), std::get<I>(indexes_)..., std::forward<Index>(index));
    } else {
      using IndexType = std::conditional_t<std::is_rvalue_reference_v<Index>, std::decay_t<Index>, Index>;
      return strided_ref_array_proxy<T, Mapper, Indexes..., IndexType>{
        data_, std::move(mapper_), std::get<I>(indexes_)..., std::forward<Index>(index)};
    }
    // clang-format on
  }

  T* data_;
  Mapper mapper_;
  std::tuple<Indexes...> indexes_;
};

/// Array with non-owned, non-continuous data.
///
/// This class represents a strided_ref view of an concrete array.
///
/// \module Array Types
/// \notes This class is not meant to be used in user code.
template <typename T, typename Mapper>
class strided_ref_array : public common_array_base<strided_ref_array<T, Mapper>>, private Mapper
{
  static_assert(Mapper::order > 0u, "invalid array dimension");

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

  strided_ref_array(value_type* data, Mapper mapper) : Mapper{std::move(mapper)}, data_{data} {}

  ~strided_ref_array(){}; // not default to disable default copy, move, assignment, ...

  /// \group Assignment
  template <typename Array> auto operator=(const common_array_base<Array>& source) -> strided_ref_array&
  {
    detail::array_assign(*this, source);
    return *this;
  }

  /// \group Assignment
  template <typename U, typename = meta::fallback<CommonArray<U>>> auto operator=(const U& source) -> strided_ref_array&
  {
    static_assert(std::is_assignable<value_type&, U>::value, "incompatible assignment");
    range::fill(*this, source);
    return *this;
  }

  /// \group Assignment
  auto operator=(const strided_ref_array& source) -> strided_ref_array&
  {
    detail::array_assign(*this, source);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator strided_ref_array<const value_type, Mapper>() const { return {data(), mapper()}; }

  /// \group Indexing
  decltype(auto) operator[](size_type i) { return detail::array_at(data(), mapper(), i); }

  /// \group Indexing
  decltype(auto) operator[](size_type i) const { return detail::array_at(data(), mapper(), i); }

  /// \group Indexing
  decltype(auto) operator[](absolute_slice slice) { return detail::array_at(data(), mapper(), std::move(slice)); }

  /// \group Indexing
  decltype(auto) operator[](absolute_slice slice) const { return detail::array_at(data(), mapper(), std::move(slice)); }

  /// \group Indexing
  decltype(auto) operator[](absolute_strided_slice slice) { return detail::array_at(data(), mapper(), std::move(slice)); }

  /// \group Indexing
  decltype(auto) operator[](absolute_strided_slice slice) const { return detail::array_at(data(), mapper(), std::move(slice)); }

  /// \group Indexing
  decltype(auto) operator[](every_index index) { return detail::array_at(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](every_index index) const { return detail::array_at(data(), mapper(), index); }

  /// \group Indexing
  decltype(auto) operator[](bounded_slice slice)
  {
    return detail::array_at(data(), mapper(), eval(slice, this->descriptor().extents[0u]));
  }

  /// \group Indexing
  decltype(auto) operator[](bounded_slice slice) const
  {
    return detail::array_at(data(), mapper(), eval(slice, this->descriptor().extents[0u]));
  }

  /// \group Indexing
  decltype(auto) operator[](bounded_strided_slice slice)
  {
    return detail::array_at(data(), mapper(), eval(slice, this->descriptor().extents[0u]));
  }

  /// \group Indexing
  decltype(auto) operator[](bounded_strided_slice slice) const
  {
    return detail::array_at(data(), mapper(), eval(slice, this->descriptor().extents[0u]));
  }

  /// \group Indexing
  template <typename Rng, typename = meta::requires<range::SizedRange<Rng>>> decltype(auto) operator[](const Rng& rng)
  {
    return detail::array_at(data(), mapper(), rng);
  }

  /// \group Indexing
  template <typename Rng, typename = meta::requires<range::SizedRange<Rng>>> decltype(auto) operator[](const Rng& rng) const
  {
    return detail::array_at(data(), mapper(), rng);
  }

  auto begin() noexcept -> iterator { return {data(), this->index_begin()}; }
  auto end() noexcept -> iterator { return {data(), this->index_end()}; }

  auto begin() const noexcept -> const_iterator { return cbegin(); }
  auto end() const noexcept -> const_iterator { return cend(); }

  auto cbegin() const noexcept -> const_iterator { return {data(), this->index_begin()}; }
  auto cend() const noexcept -> const_iterator { return {data(), this->index_end()}; }

  auto size() const noexcept { return this->descriptor().size(); }

  auto length() const noexcept { return this->descriptor().length(); }

  auto row_count() const noexcept { return this->descriptor().row_count(); }

  auto column_count() const noexcept { return this->descriptor().column_count(); }

  auto dimensions() const noexcept -> std::array<size_type, order> { return this->descriptor().extents; }

protected:
  auto data() -> value_type* { return data_; }

  auto data() const -> const value_type* { return data_; }

  auto mapper() const -> const Mapper& { return *this; }

  strided_ref_array(const strided_ref_array& source) = default;
  strided_ref_array(strided_ref_array&& source) noexcept = default;

  /// \exclude
  value_type* const data_;
};

template <typename T, typename Mapper>
auto eval(const strided_ref_array<T, Mapper>& source) -> const strided_ref_array<T, Mapper>&
{
  return source;
}

} // namespace jules

#endif // JULES_ARRAY_STRIDED_REF_ARRAY_H
