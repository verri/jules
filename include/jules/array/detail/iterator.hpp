// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DETAIL_ITERATOR_H
/// \exclude
#define JULES_ARRAY_DETAIL_ITERATOR_H

#include <jules/array/detail/common.hpp>
#include <jules/core/type.hpp>

#include <iterator>

/// \exclude
namespace jules::detail
{
template <typename T, typename It> class iterator_from_indexes
{
public:
  using iterator_category = typename std::iterator_traits<It>::iterator_category;
  using value_type = T;
  using difference_type = distance_t;
  using pointer = T*;
  using reference = T&;

  constexpr iterator_from_indexes() = default;
  constexpr iterator_from_indexes(T* data, It it) : data_{data}, it_{std::move(it)} {}

  constexpr iterator_from_indexes(const iterator_from_indexes& source) = default;
  constexpr iterator_from_indexes(iterator_from_indexes&& source) noexcept = default;

  constexpr auto operator=(const iterator_from_indexes& source) -> iterator_from_indexes& = default;
  constexpr auto operator=(iterator_from_indexes&& source) noexcept -> iterator_from_indexes& = default;

  constexpr auto operator++() -> iterator_from_indexes&
  {
    ++it_;
    return *this;
  }

  constexpr auto operator++(int) -> iterator_from_indexes
  {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  constexpr auto operator--() -> iterator_from_indexes&
  {
    --it_;
    return *this;
  }

  constexpr auto operator--(int) -> iterator_from_indexes
  {
    auto copy = *this;
    --(*this);
    return copy;
  }

  constexpr auto operator+(difference_type n) const -> iterator_from_indexes { return {data_, std::next(it_, n)}; }

  constexpr auto operator-(difference_type n) const -> iterator_from_indexes { return {data_, std::prev(it_, n)}; }

  constexpr auto operator==(const iterator_from_indexes& other) const { return it_ == other.it_; }

  constexpr auto operator!=(const iterator_from_indexes& other) const { return !(*this == other); }

  constexpr auto operator-(const iterator_from_indexes& other) const { return std::distance(other.it_, it_); }

  constexpr auto operator*() const -> reference { return data_[*it_]; }

  constexpr auto operator->() const -> pointer { return data_ + *it_; }

private:
  T* data_ = nullptr;
  It it_;
};

} // namespace jules::detail

#endif // JULES_ARRAY_DETAIL_ITERATOR_H
