// Copyright (c) 2018 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_AXIS_H
#define JULES_ARRAY_AXIS_H

#include <jules/array/drop.hpp>
#include <jules/array/meta/reference.hpp>
#include <jules/array/slicing/bounded.hpp>
#include <jules/core/type.hpp>

namespace jules
{

namespace detail
{

template <size_t Order, size_t Axis, size_t I, typename ArrayLike>
constexpr decltype(auto) axis_at_impl(ArrayLike&& arr, [[maybe_unused]] index_t i)
{
  if constexpr (I == Order)
    return static_cast<ArrayLike>(arr);
  else if constexpr (I == Axis)
    return axis_at_impl<Order, Axis, I + 1>(std::forward<ArrayLike>(arr).operator[](i), i);
  else
    return axis_at_impl<Order, Axis, I + 1>(std::forward<ArrayLike>(arr).operator[](every), i);
}

} // namespace detail

template <size_t Axis, typename RefArray, typename = meta::requires<ReferenceArray<RefArray>>>
constexpr decltype(auto) axis_at(RefArray arr, index_t i)
{
  static_assert(Axis < RefArray::order);
  return drop_to<RefArray::order - 1>(detail::axis_at_impl<RefArray::order, Axis, 0>(arr, i));
}

template <size_t Axis, typename RefArray> class axis_iterator
{
  static_assert(ReferenceArray<RefArray>::value);
  static_assert(RefArray::order >= 2);

public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = decltype(axis_at<Axis>(std::declval<RefArray>(), 0u));
  using difference_type = distance_t;
  using pointer = value_type;
  using reference = value_type;

  constexpr axis_iterator(RefArray arr, index_t pos) noexcept : arr_{std::move(arr)}, pos_{pos} {}

  constexpr auto operator++() -> axis_iterator&
  {
    ++pos_;
    return *this;
  }

  constexpr auto operator++(int) -> axis_iterator
  {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  constexpr auto operator--() -> axis_iterator&
  {
    --pos_;
    return *this;
  }

  constexpr auto operator--(int) -> axis_iterator
  {
    auto copy = *this;
    --(*this);
    return copy;
  }

  constexpr auto operator+(difference_type n) const -> axis_iterator { return {arr_, pos_ + n}; }

  constexpr auto operator-(difference_type n) const -> axis_iterator { return {arr_, pos_ - n}; }

  constexpr auto operator==(const axis_iterator& other) const { return pos_ == other.pos_; }

  constexpr auto operator!=(const axis_iterator& other) const { return !(*this == other); }

  constexpr auto operator-(const axis_iterator& other) const
  {
    return static_cast<difference_type>(other.pos_) - static_cast<difference_type>(pos_);
  }

  constexpr auto operator*() -> reference { return axis_at<Axis>(arr_, pos_); }

private:
  RefArray arr_;
  index_t pos_;
};

template <size_t Axis, typename RefArray> class axis_array : public common_array_base<axis_array<Axis, RefArray>>
{
  static_assert(Axis < RefArray::order);

public:
  static constexpr size_t order = 1;

  using value_type = typename axis_iterator<Axis, RefArray>::value_type;
  using iterator = axis_iterator<Axis, RefArray>;
  using const_iterator = axis_iterator<Axis, RefArray>;
  using size_type = index_t;
  using difference_type = distance_t;

  constexpr axis_array(RefArray arr) : arr_{std::move(arr)} {}

  auto size() const noexcept { return arr_.dimensions()[Axis]; }

  auto dimensions() const noexcept -> std::array<size_t, 1u> { return {{this->size()}}; }

  auto length() const noexcept { return this->size(); }

  auto begin() noexcept -> iterator { return {arr_, 0u}; }

  auto end() noexcept -> iterator { return {arr_, this->size()}; }

  auto begin() const noexcept -> const_iterator { return this->begin(); }

  auto end() const noexcept -> const_iterator { return this->end(); }

  auto cbegin() const noexcept { return this->begin(); }

  auto cend() const noexcept { return this->end(); }

private:
  RefArray arr_;
};

template <size_t Axis, typename RefArray, typename meta::requires<ReferenceArray<RefArray>>>
auto axis(RefArray arr) -> axis_array<Axis, RefArray>
{
  return {std::move(arr)};
}

template <size_t Axis, typename T, size_t N> auto axis(array<T, N>& arr) -> axis_array<Axis, decltype(ref(arr))>
{
  return {ref(arr)};
}

template <size_t Axis, typename T, size_t N> auto axis(const array<T, N>& arr) -> axis_array<Axis, decltype(ref(arr))>
{
  return {ref(arr)};
}

template <typename RefArray, typename meta::requires<ReferenceArray<RefArray>>> auto rows(RefArray arr) -> axis_array<0, RefArray>
{
  return {std::move(arr)};
}

template <typename T, size_t N> auto rows(array<T, N>& arr) -> axis_array<0, decltype(ref(arr))> { return {ref(arr)}; }

template <typename T, size_t N> auto rows(const array<T, N>& arr) -> axis_array<0, decltype(ref(arr))> { return {ref(arr)}; }

template <typename RefArray, typename meta::requires<ReferenceArray<RefArray>>>
auto columns(RefArray arr) -> axis_array<1, RefArray>
{
  return {std::move(arr)};
}

template <typename T, size_t N> auto columns(array<T, N>& arr) -> axis_array<1, decltype(ref(arr))> { return {ref(arr)}; }

template <typename T, size_t N> auto columns(const array<T, N>& arr) -> axis_array<1, decltype(ref(arr))> { return {ref(arr)}; }

} // namespace jules

#endif // JULES_ARRAY_AXIS_H
