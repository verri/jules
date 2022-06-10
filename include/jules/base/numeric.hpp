// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_NUMERIC_H
/// \exclude
#define JULES_BASE_NUMERIC_H

#include <jules/base/math.hpp>
#include <jules/core/concepts.hpp>
#include <jules/core/ranges.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <tuple>
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
  template <typename T> constexpr decltype(auto) operator+(T&& value) { return std::forward<T>(value); }
  template <typename T> constexpr decltype(auto) operator*(T&& value) { return std::forward<T>(value); }
  template <typename T> friend constexpr decltype(auto) operator+(T&& value, forward_arithmetic)
  {
    return std::forward<T>(value);
  }
  template <typename T> friend constexpr decltype(auto) operator*(T&& value, forward_arithmetic)
  {
    return std::forward<T>(value);
  }
};

} // namespace detail

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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
auto max(Iter first, Sent last, T start = numeric_traits<T>::unbounded_min())
{
  for (; first != last; ++first)
    if (start < *first)
      start = *first;
  return start;
}

/// \group Max
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
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
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
auto min(Iter first, Sent last, T start = numeric_traits<T>::unbounded_max())
{
  for (; first != last; ++first)
    if (start > *first)
      start = *first;
  return start;
}

/// \group Min
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
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
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
auto prod(Iter first, Sent last, T start = numeric_traits<T>::multiplicative_identity())
{
  for (; first != last; ++first)
    start *= *first;
  return start;
}

/// \group Prod
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
auto sum(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  for (; first != last; ++first)
    start += *first;
  return start;
}

/// \group Sum
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
auto mean(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::sum(first, last, std::move(start)) / ranges::distance(first, last);
}

/// \group Mean
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
auto var(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  const auto N = ranges::distance(first, last);
  const auto mu = ::jules::mean(first, last, start);
  auto result = std::move(start);
  for (auto it = first; it != last; ++it)
    result += vsq(*it - mu);
  return result / (N - 1);
}

/// \group Var
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
auto sd(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  return sqrt(::jules::var(first, last, std::move(start)));
}

/// \group Sd
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
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
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, common_numeric T = ranges::iter_value_t<Iter>>
auto meansd(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  const auto n = ranges::distance(first, last);
  const auto mu = ::jules::mean(first, last, start);
  auto result = std::move(start);

  for (auto it = first; it != last; ++it)
    result += vsq(*it - mu);

  return std::tuple(mu, sqrt(result / (n - 1)));
}

/// \group MeanSd
template <ranges::range Rng, common_numeric T = ranges::range_value_t<Rng>>
auto meansd(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::meansd(ranges::begin(rng), ranges::end(rng), std::move(start));
}

/// \group Count
///
/// Returns the number of true elements in a `range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, convertible_to<bool> T = ranges::iter_value_t<Iter>>
auto count(Iter first, Sent last)
{
  auto n = index_t{0u};
  for (; first != last; ++first)
    if (*first)
      ++n;
  return n;
}

/// \group Count
template <ranges::range Rng, convertible_to<bool> T = ranges::range_value_t<Rng>> auto count(const Rng& rng)
{
  return ::jules::count(ranges::begin(rng), ranges::end(rng));
}

/// \group Freq
///
/// Returns the frequency of true elements in a `range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, convertible_to<bool> T = ranges::iter_value_t<Iter>>
auto freq(Iter first, Sent last)
{
  return static_cast<numeric>(::jules::count(first, last)) / ranges::distance(first, last);
}

/// \group Freq
template <ranges::range Rng, convertible_to<bool> T = ranges::range_value_t<Rng>> auto freq(const Rng& rng)
{
  return static_cast<numeric>(::jules::count(rng)) / ranges::size(rng);
}

/// \group Which
///
/// Returns a const_vector with the indexes of the true elements in a `range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, convertible_to<bool> T = ranges::iter_value_t<Iter>,
          container_for<index_t> C = container<index_t>>
auto which(Iter first, Sent last, C indexes = container<index_t>{}) -> C
{
  auto it = first;
  for (auto index = index_t{0u}; it != last; ++index)
    if (*it++)
      indexes.push_back(index);
  return indexes;
}

/// \group Which
template <ranges::range Rng, convertible_to<bool> T = ranges::range_value_t<Rng>, container_for<index_t> C = container<index_t>>
auto which(const Rng& rng, C indexes = container<index_t>{}) -> C
{
  return ::jules::which(ranges::begin(rng), ranges::end(rng), std::move(indexes));
}

/// \group All
///
/// Checks whether all elements in a `range` or in the sequence [`first`, `last`) are true.
///
/// \module Logical
/// \notes If empty, returns true.
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, convertible_to<bool> T = ranges::iter_value_t<Iter>>
auto all(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (!static_cast<bool>(*first))
      return false;
  return true;
}

/// \group All
template <ranges::range Rng, convertible_to<bool> T = ranges::range_value_t<Rng>> auto all(const Rng& rng)
{
  return all(ranges::begin(rng), ranges::end(rng));
}

/// \group None
///
/// Checks whether all elements in a `range` or in the sequence [`first`, `last`) are false.
///
/// \module Logical
/// \notes If empty, returns true.
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, convertible_to<bool> T = ranges::iter_value_t<Iter>>
auto none(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (static_cast<bool>(*first))
      return false;
  return true;
}

/// \group None
template <ranges::range Rng, convertible_to<bool> T = ranges::range_value_t<Rng>> auto none(const Rng& rng)
{
  return none(ranges::begin(rng), ranges::end(rng));
}

/// \group Any
///
/// Checks whether any of the elements in a `range` or in the sequence [`first`, `last`) are true.
///
/// \module Logical
/// \notes If empty, returns false.
template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, convertible_to<bool> T = ranges::iter_value_t<Iter>>
auto any(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (static_cast<bool>(*first))
      return true;
  return false;
}

/// \group Any
template <ranges::range Rng, convertible_to<bool> T = ranges::range_value_t<Rng>> auto any(const Rng& rng)
{
  return any(ranges::begin(rng), ranges::end(rng));
}

template <typename... Args> constexpr auto prod_args(Args&&... args)
{
  return (std::forward<Args>(args) * ... * detail::forward_arithmetic{});
}

template <typename... Args> constexpr auto sum_args(Args&&... args)
{
  return (std::forward<Args>(args) + ... + detail::forward_arithmetic{});
}

template <typename... Args> constexpr auto count_args(Args&&... args)
{
  return (index_t{0} + ... + (std::forward<Args>(args) ? 1 : 0));
}

template <typename... Args> constexpr auto all_args(Args&&... args)
{
  return (static_cast<bool>(std::forward<Args>(args)) && ...);
}

template <typename... Args> constexpr auto any_args(Args&&... args)
{
  return (static_cast<bool>(std::forward<Args>(args)) || ...);
}

template <typename Arg, typename... Args> constexpr decltype(auto) first_arg(Arg&& arg, Args&&...)
{
  return std::forward<Arg>(arg);
}

template <typename Arg> constexpr decltype(auto) last_arg(Arg&& arg) { return std::forward<Arg>(arg); }

template <typename Arg, typename... Args>
requires(sizeof...(Args) > 0) constexpr decltype(auto) last_arg(Arg&&, Args&&... args)
{
  return last_arg(std::forward<Args>(args)...);
}

template <typename Head, typename...> using first_element = Head;
template <typename... Ts> using last_element = std::tuple_element_t<sizeof...(Ts) - 1, std::tuple<Ts...>>;

} // namespace jules

#endif // JULES_BASE_NUMERIC_H
