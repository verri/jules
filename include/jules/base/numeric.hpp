// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_NUMERIC_H
#define JULES_BASE_NUMERIC_H

#include <jules/base/const_vector.hpp>
#include <jules/base/math.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/ranges.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

namespace jules
{

/// \exclude
namespace detail
{

template <typename T, std::size_t... I>
constexpr auto repeat_impl(const T& value, std::index_sequence<I...>) -> std::array<T, sizeof...(I)>
{
  return {{((void)I, value)...}};
}

template <typename T, std::size_t N, std::size_t... I>
constexpr auto prod_impl(const std::array<T, N>& arr,
                         std::index_sequence<I...>) noexcept(noexcept(std::declval<const T&>() * std::declval<const T&>())) -> T
{
  return (numeric_traits<T>::multiplicative_identity() * ... * arr[I]);
}

struct forward_arithmetic
{
  template <typename T> decltype(auto) operator+(T&& value) { return std::forward<T>(value); }
  template <typename T> decltype(auto) operator*(T&& value) { return std::forward<T>(value); }
  template <typename T> friend decltype(auto) operator+(T&& value, forward_arithmetic) { return std::forward<T>(value); }
  template <typename T> friend decltype(auto) operator*(T&& value, forward_arithmetic) { return std::forward<T>(value); }
};

struct identity_t
{
  template <typename T> constexpr decltype(auto) operator()(T&& x) const noexcept { return std::forward<T>(x); }
};

} // namespace detail

constexpr static auto identity = detail::identity_t{};

/// \group Repeat
///
/// Repeats `value` `N` times.
///
/// \module Arithmetic
/// \returns [std::array]() and [std::vector]() containing the repeated value if `N`
///   is a template argument or not, respectively.
template <std::size_t N, typename T> constexpr auto repeat(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
{
  return detail::repeat_impl(value, std::make_index_sequence<N>());
}

/// \group Repeat
template <typename T> auto repeat(index_t N, const T& value) { return std::vector<T>(N, value); }

/// \group Max
///
/// Returns the maximum element either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `unbounded_min`.
/// \notes If empty, returns [jules::numeric_traits<T>::unbounded_min]().
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>, typename = meta::requires_<>>
auto max(Iter first, Sent last, T start = numeric_traits<T>::unbounded_min())
{
  for (; first != last; ++first)
    if (start < *first)
      start = *first;
  return start;
}

/// \group Max
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto max(const Rng& rng, T start = numeric_traits<T>::unbounded_min())
{
  return ::jules::max(ranges::begin(rng), ranges::end(rng), std::move(start));
}

/// \group WhichMax
///
/// Returns the position of the maximum element either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `unbounded_min`.
/// \notes If empty, returns [jules::numeric_traits<T>::unbounded_min]().
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto which_max(Iter first, Sent last, T start = numeric_traits<T>::unbounded_min())
{
  std::size_t pos = 0u, best_pos = 0;

  for (; first != last; ++first, ++pos) {
    if (start < *first) {
      start = *first;
      best_pos = pos;
    }
  }

  return best_pos;
}

/// \group WhichMax
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto which_max(const Rng& rng, T start = numeric_traits<T>::unbounded_min())
{
  return ::jules::which_max(ranges::begin(rng), ranges::end(rng), std::move(start));
}

/// \group Min
///
/// Returns the minimum element either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `unbounded_max`.
/// \notes If empty, returns [jules::numeric_traits<T>::unbounded_max]().
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto min(Iter first, Sent last, T start = numeric_traits<T>::unbounded_max())
{
  for (; first != last; ++first)
    if (start > *first)
      start = *first;
  return start;
}

/// \group Min
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto min(const Rng& rng, T start = numeric_traits<T>::unbounded_max())
{
  return ::jules::min(ranges::begin(rng), ranges::end(rng), std::move(start));
}

/// \group Min
///
/// Returns the position of the minimum element either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `unbounded_max`.
/// \notes If empty, returns [jules::numeric_traits<T>::unbounded_max]().
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto which_min(Iter first, Sent last, T start = numeric_traits<T>::unbounded_max())
{
  std::size_t pos = 0u, best_pos = 0;

  for (; first != last; ++first, ++pos) {
    if (start > *first) {
      start = *first;
      best_pos = pos;
    }
  }

  return best_pos;
}

/// \group WhichMin
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto which_min(const Rng& rng, T start = numeric_traits<T>::unbounded_max())
{
  return ::jules::which_min(ranges::begin(rng), ranges::end(rng), std::move(start));
}

/// \group Prod
///
/// Returns the product of the elements either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `multiplicative_identity`.
/// \notes If empty, returns [jules::numeric_traits<T>::multiplicative_identity]().
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto prod(Iter first, Sent last, T start = numeric_traits<T>::multiplicative_identity())
{
  for (; first != last; ++first)
    start *= *first;
  return start;
}

/// \group Prod
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto prod(const Rng& rng, T start = numeric_traits<T>::multiplicative_identity())
{
  return ::jules::prod(ranges::begin(rng), ranges::end(rng), std::move(start));
}

template <typename T, std::size_t N>
constexpr auto prod(const std::array<T, N>& arr) noexcept(noexcept(detail::prod_impl(arr, std::make_index_sequence<N>()))) -> T
{
  return detail::prod_impl(arr, std::make_index_sequence<N>());
}

/// \group Sum
///
/// Returns the summation of the elements either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
/// \notes If empty, returns [jules::numeric_traits<T>::additive_identity]().
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto sum(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  for (; first != last; ++first)
    start += *first;
  return start;
}

/// \group Sum
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto sum(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::sum(ranges::begin(rng), ranges::end(rng), std::move(start));
}

/// \group Mean
///
/// Returns the mean of the elements either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto mean(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::sum(first, last, std::move(start)) / ranges::distance(first, last);
}

/// \group Mean
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto mean(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::sum(rng, std::move(start)) / ranges::size(rng);
}

/// \group Var
///
/// Returns the *corrected* sample variance of the elements either in a `range` or in the
/// sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto var(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  const auto N = ranges::distance(first, last);
  const auto mu = ::jules::mean(first, last, start);
  auto result = std::move(start);
  for (auto it = first; it != last; ++it)
    result += square(*it - mu);
  return result / (N - 1);
}

/// \group Var
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto var(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::var(ranges::begin(rng), ranges::end(rng), std::move(start));
}

/// \group Sd
///
/// Returns the *corrected* sample standard deviation of the elements either in a `range`
/// or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
/// \notes It might promote the result type since it calls sqrt.
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto sd(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  return sqrt(::jules::var(first, last, std::move(start)));
}

/// \group Sd
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto sd(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return sqrt(::jules::var(rng, std::move(start)));
}

/// \group MeanSd
///
/// Efficiently calculates both *corrected* mean and sample standard deviation of the
/// elements either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
/// \notes It might promote the result type since it calls sqrt.
template <typename Iter, typename Sent, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto meansd(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  const auto n = ranges::distance(first, last);
  const auto mu = ::jules::mean(first, last, start);
  auto result = std::move(start);

  for (auto it = first; it != last; ++it)
    result += square(*it - mu);

  return std::tuple(mu, sqrt(result / (n - 1)));
}

/// \group MeanSd
template <typename Rng, typename T = ranges::range_value_t<Rng>, typename = meta::requires_concept<ranges::range<Rng>>>
auto meansd(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::meansd(ranges::begin(rng), ranges::end(rng), std::move(start));
}

/// \group Nth
///
/// Returns the nth lesser of the elements either in a `range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes Memory complexity is O(size(rng)) or O(distance(first, last)).
template <typename Iter, typename Sent, typename Compare = std::less<>, typename T = ranges::iter_value_t<Iter>,
          typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto nth(Iter first, Sent last, index_t n, Compare cmp = {})
{
  auto values = std::vector<T>{};
  values.reserve(ranges::distance(first, last));
  ranges::copy(first, last, ranges::back_inserter(values));

  std::nth_element(values.begin(), values.begin() + n, values.end(), cmp);
  return T{std::move(values[n])};
}

/// \group Nth
template <typename Rng, typename Compare = std::less<>, typename T = ranges::range_value_t<Rng>,
          typename = meta::requires_concept<ranges::range<Rng>>>
auto nth(const Rng& rng, index_t n, Compare cmp = {})
{
  auto values = std::vector<T>{};
  values.reserve(ranges::size(rng));
  ranges::copy(rng, ranges::back_inserter(values));

  std::nth_element(values.begin(), values.begin() + n, values.end(), cmp);
  return T{std::move(values[n])};
}

/// \group Count
///
/// Returns the number of true elements in a `range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <typename Iter, typename Sent, typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto count(Iter first, Sent last)
{
  auto n = index_t{0u};
  for (; first != last; ++first)
    if (*first)
      ++n;
  return n;
}

/// \group Count
template <typename Rng, typename = meta::requires_concept<ranges::range<Rng>>> auto count(const Rng& rng)
{
  return ::jules::count(ranges::begin(rng), ranges::end(rng));
}

/// \group Freq
///
/// Returns the frequency of true elements in a `range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <typename Iter, typename Sent, typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto freq(Iter first, Sent last)
{
  return static_cast<numeric>(::jules::count(first, last)) / ranges::distance(first, last);
}

/// \group Freq
template <typename Rng, typename = meta::requires_concept<ranges::range<Rng>>> auto freq(const Rng& rng)
{
  return static_cast<numeric>(::jules::count(rng)) / ranges::size(rng);
}

/// \group Which
///
/// Returns a const_vector with the indexes of the true elements in a `range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <typename Iter, typename Sent, typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto which(Iter first, Sent last) -> const_vector<index_t>
{
  auto indexes = std::vector<index_t>();
  auto it = first;
  for (auto index = index_t{0u}; it != last; ++index)
    if (*it++)
      indexes.push_back(index);
  return {std::move(indexes)};
}

/// \group Which
template <typename Rng, typename = meta::requires_concept<ranges::range<Rng>>> auto which(const Rng& rng)
{
  return ::jules::which(ranges::begin(rng), ranges::end(rng));
}

/// \group All
///
/// Checks whether all elements in a `range` or in the sequence [`first`, `last`) are true.
///
/// \module Logical
/// \notes If empty, returns true.
template <typename Iter, typename Sent, typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto all(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (!static_cast<bool>(*first))
      return false;
  return true;
}

/// \group All
template <typename Rng, typename = meta::requires_concept<ranges::range<Rng>>> auto all(const Rng& rng)
{
  return all(ranges::begin(rng), ranges::end(rng));
}

/// \group None
///
/// Checks whether all elements in a `range` or in the sequence [`first`, `last`) are false.
///
/// \module Logical
/// \notes If empty, returns true.
template <typename Iter, typename Sent, typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto none(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (static_cast<bool>(*first))
      return false;
  return true;
}

/// \group None
template <typename Rng, typename = meta::requires_concept<ranges::range<Rng>>> auto none(const Rng& rng)
{
  return none(ranges::begin(rng), ranges::end(rng));
}

/// \group Any
///
/// Checks whether any of the elements in a `range` or in the sequence [`first`, `last`) are true.
///
/// \module Logical
/// \notes If empty, returns false.
template <typename Iter, typename Sent, typename = meta::requires_concept<ranges::sentinel_for<Sent, Iter>>>
auto any(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (static_cast<bool>(*first))
      return true;
  return false;
}

/// \group Any
template <typename Rng, typename = meta::requires_concept<ranges::range<Rng>>> auto any(const Rng& rng)
{
  return any(ranges::begin(rng), ranges::end(rng));
}

template <typename... Args> constexpr decltype(auto) prod_args(Args&&... args)
{
  return (std::forward<Args>(args) * ... * detail::forward_arithmetic{});
}

template <typename... Args> constexpr decltype(auto) sum_args(Args&&... args)
{
  return (std::forward<Args>(args) + ... + detail::forward_arithmetic{});
}

template <typename... Args> constexpr decltype(auto) all_args(Args&&... args) { return (std::forward<Args>(args) && ...); }

template <typename... Args> constexpr decltype(auto) any_args(Args&&... args) { return (std::forward<Args>(args) || ...); }

} // namespace jules

#endif // JULES_BASE_NUMERIC_H
