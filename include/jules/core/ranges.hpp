// Copyright (c) 2017-2025 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_RANGE_H
/// \exclude
#define JULES_CORE_RANGE_H

#include <concepts>
#include <cstddef>
#include <jules/core/type.hpp>
#include <ranges>

namespace jules
{
namespace ranges
{
using namespace std::ranges;
}

template <typename T> class index_iterator
{
public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = const T*;
  using reference = const T&;
  using iterator_category = std::input_iterator_tag;

  constexpr index_iterator() noexcept = default;
  constexpr index_iterator(T value) noexcept : value_{value} {}

  constexpr index_iterator(const index_iterator&) noexcept = default;
  constexpr index_iterator(index_iterator&&) noexcept = default;

  constexpr auto operator=(const index_iterator&) noexcept -> index_iterator& = default;
  constexpr auto operator=(index_iterator&&) noexcept -> index_iterator& = default;

  constexpr auto operator->() const noexcept { return &value_; }

  constexpr auto operator*() const noexcept { return value_; }
  constexpr auto operator++() noexcept -> index_iterator&
  {
    ++value_;
    return *this;
  }
  constexpr auto operator++(int) noexcept -> index_iterator { return index_iterator{value_++}; }

  constexpr auto operator==(const index_iterator& other) const noexcept { return value_ == other.value_; }
  constexpr auto operator!=(const index_iterator& other) const noexcept { return value_ != other.value_; }

  constexpr auto operator-(const index_iterator& other) const noexcept
  {
    return static_cast<std::ptrdiff_t>(value_) - other.value_;
  }

private:
  T value_;
};

template <std::integral T> class indices_t
{
public:
  using value_type = T;
  using iterator = index_iterator<T>;
  using const_iterator = index_iterator<T>;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;

  constexpr indices_t(T first, T last) noexcept : first_{first}, last_{last} {}
  constexpr indices_t(T last) noexcept : first_{0}, last_{last} {}

  constexpr auto begin() const noexcept -> index_iterator<T> { return first_; }
  constexpr auto end() const noexcept -> index_iterator<T> { return last_; }

  constexpr auto cbegin() const noexcept -> index_iterator<T> { return first_; }
  constexpr auto cend() const noexcept -> index_iterator<T> { return last_; }

  constexpr auto size() const noexcept -> std::size_t { return static_cast<std::size_t>(last_ - first_); }
  constexpr auto ssize() const noexcept -> std::ptrdiff_t { return static_cast<std::ptrdiff_t>(last_) - first_; }

  constexpr auto empty() const noexcept -> bool { return first_ == last_; }

private:
  T first_;
  T last_;
};

template <std::integral T> constexpr auto indices(T first, T last) noexcept -> indices_t<T> { return indices_t<T>{first, last}; }

template <std::integral T> constexpr auto indices(T last) noexcept -> indices_t<T> { return indices_t<T>{last}; }

template <std::integral T> constexpr auto closed_indices(T first, T last) noexcept -> indices_t<T>
{
  return indices_t<T>{first, last + 1};
}

static_assert(std::ranges::range<indices_t<int>>);

} // namespace jules

#endif // JULES_CORE_RANGE_H
