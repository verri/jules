// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DETAIL_ITERATOR_H
#define JULES_ARRAY_DETAIL_ITERATOR_H

#include <jules/array/detail/common.hpp>
#include <jules/core/type.hpp>

#include <iterator>

namespace jules
{
namespace detail
{
template <typename T, typename It> class iterator_from_indexes
{
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = distance_t;
  using pointer = T*;
  using reference = T&;

  constexpr iterator_from_indexes() = default;
  constexpr iterator_from_indexes(T* data, It it) : data_{data}, it_{std::move(it)} {}

  constexpr iterator_from_indexes(const iterator_from_indexes& source) = default;
  constexpr iterator_from_indexes(iterator_from_indexes&& source) noexcept = default;

  constexpr iterator_from_indexes& operator=(const iterator_from_indexes& source) = default;
  constexpr iterator_from_indexes& operator=(iterator_from_indexes&& source) noexcept = default;

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

  constexpr auto operator==(const iterator_from_indexes& other) const { return it_ == other.it_; }

  constexpr auto operator!=(const iterator_from_indexes& other) const { return !(*this == other); }

  constexpr auto operator*() -> reference { return data_[*it_]; }

  constexpr auto operator-> () -> pointer { return data_ + *it_; }

private:
  T* data_ = nullptr;
  It it_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ITERATOR_H
