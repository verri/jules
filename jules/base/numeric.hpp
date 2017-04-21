// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_NUMERIC_H
#define JULES_BASE_NUMERIC_H

#include <jules/base/const_vector.hpp>
#include <jules/base/math.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>

#include <array>
#include <cmath>
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

struct forward_arithmetic {
  template <typename T> decltype(auto) operator+(T&& value) { return std::forward<T>(value); }
  template <typename T> decltype(auto) operator*(T&& value) { return std::forward<T>(value); }
  template <typename T> friend decltype(auto) operator+(T&& value, forward_arithmetic) { return std::forward<T>(value); }
  template <typename T> friend decltype(auto) operator*(T&& value, forward_arithmetic) { return std::forward<T>(value); }
};

} // namespace detail

/// \group Repeat
///
/// Repeats `value` `N` times.
///
/// \module Arithmetic
/// \returns [std::array]() and [std::vector]() containing the repeated value if `N`
///   is a template argument or not, respectively.
template <std::size_t N, typename T> constexpr auto repeat(const T& value)
{
  return detail::repeat_impl(value, std::make_index_sequence<N>());
}

/// \group Repeat
template <typename T> auto repeat(index_t N, const T& value) { return std::vector<T>(N, value); }

/// \group Length
///
/// Returns either the length of a `Range` or the distance of a pair of iterators.
///
/// \module Arithmetic
/// \notes The value may be negative if random-access iterators are used and `first`
///   is reachable from `last`.
template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>> auto length(Iter first, Sent last)
{
  return range::distance(first, last);
}

/// \group Length
template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto length(const Rng& rng)
{
  return ::jules::length(range::begin(rng), range::end(rng));
}

/// \group Max
///
/// Returns the maximum element either in a `Range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `unbounded_min`.
/// \notes If empty, returns [jules::numeric_traits<T>::unbounded_min]().
template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto max(Iter first, Sent last, T start = numeric_traits<T>::unbounded_min())
{
  for (; first != last; ++first)
    if (start < *first)
      start = *first;
  return start;
}

/// \group Max
template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto max(const Rng& rng, T start = numeric_traits<T>::unbounded_min())
{
  return ::jules::max(range::begin(rng), range::end(rng), std::move(start));
}

/// \group Min
///
/// Returns the minimum element either in a `Range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `unbounded_max`.
/// \notes If empty, returns [jules::numeric_traits<T>::unbounded_max]().
template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto min(Iter first, Sent last, T start = numeric_traits<T>::unbounded_max())
{
  for (; first != last; ++first)
    if (start > *first)
      start = *first;
  return start;
}

/// \group Min
template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto min(const Rng& rng, T start = numeric_traits<T>::unbounded_max())
{
  return ::jules::min(range::begin(rng), range::end(rng), std::move(start));
}

/// \group Prod
///
/// Returns the product of the elements either in a `Range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `multiplicative_identity`.
/// \notes If empty, returns [jules::numeric_traits<T>::multiplicative_identity]().
template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto prod(Iter first, Sent last, T start = numeric_traits<T>::multiplicative_identity())
{
  for (; first != last; ++first)
    start *= *first;
  return start;
}

/// \group Prod
template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto prod(const Rng& rng, T start = numeric_traits<T>::multiplicative_identity())
{
  return ::jules::prod(range::begin(rng), range::end(rng), std::move(start));
}

/// \group Sum
///
/// Returns the summation of the elements either in a `Range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
/// \notes If empty, returns [jules::numeric_traits<T>::additive_identity]().
template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto sum(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  for (; first != last; ++first)
    start += *first;
  return start;
}

/// \group Sum
template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto sum(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::sum(range::begin(rng), range::end(rng), std::move(start));
}

/// \group Mean
///
/// Returns the mean of the elements either in a `Range` or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto mean(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::sum(first, last, std::move(start)) / ::jules::length(first, last);
}

/// \group Mean
template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto mean(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::sum(rng, std::move(start)) / ::jules::length(rng);
}

/// \group Var
///
/// Returns the *corrected* sample variance of the elements either in a `Range` or in the
/// sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto var(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  const auto N = ::jules::length(first, last);
  const auto mu = ::jules::mean(first, last, start);
  auto result = std::move(start);
  for (auto it = first; it != last; ++it)
    result += square(*it - mu);
  return result / (N - 1);
}

/// \group Var
template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto var(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::var(range::begin(rng), range::end(rng), std::move(start));
}

/// \group Sd
///
/// Returns the *corrected* sample standard deviation of the elements either in a `Range`
/// or in the sequence [`first`, `last`).
///
/// \module Arithmetic
/// \notes [jules::numeric_traits<T>]() must implement `additive_identity`.
template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto sd(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  return std::sqrt(::jules::var(first, last, std::move(start)));
}

/// \group Sd
template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto sd(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return std::sqrt(::jules::var(rng, std::move(start)));
}

/// \group Count
///
/// Returns the number of true elements in a `Range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>> auto count(Iter first, Sent last)
{
  auto n = index_t{0u};
  for (; first != last; ++first)
    if (*first)
      ++n;
  return n;
}

/// \group Count
template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto count(const Rng& rng)
{
  return ::jules::count(range::begin(rng), range::end(rng));
}

/// \group Freq
///
/// Returns the frequency of true elements in a `Range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>> auto freq(Iter first, Sent last)
{
  return static_cast<numeric>(::jules::count(first, last)) / ::jules::length(first, last);
}

/// \group Freq
template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto freq(const Rng& rng)
{
  return static_cast<numeric>(::jules::count(rng)) / ::jules::length(rng);
}

/// \group Which
///
/// Returns a const_vector with the indexes of the true elements in a `Range` or in the sequence [`first`, `last`).
///
/// \module Logical
template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto which(Iter first, Sent last) -> const_vector<index_t>
{
  auto indexes = std::vector<index_t>();
  auto it = first;
  for (auto index = 0ul; it != last; ++index)
    if (*it++)
      indexes.push_back(index);
  return {std::move(indexes)};
}

/// \group Which
template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto which(const Rng& rng)
{
  return ::jules::which(range::begin(rng), range::end(rng));
}

/// \group All
///
/// Checks whether all elements in a `Range` or in the sequence [`first`, `last`) are true.
///
/// \module Logical
/// \notes If empty, returns true.
template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>> auto all(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (!static_cast<bool>(*first))
      return false;
  return true;
}

/// \group All
template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto all(const Rng& rng)
{
  return all(ranges::begin(rng), ranges::end(rng));
}

/// \group None
///
/// Checks whether all elements in a `Range` or in the sequence [`first`, `last`) are false.
///
/// \module Logical
/// \notes If empty, returns true.
template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>> auto none(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (static_cast<bool>(*first))
      return false;
  return true;
}

/// \group None
template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto none(const Rng& rng)
{
  return none(ranges::begin(rng), ranges::end(rng));
}

/// \group Any
///
/// Checks whether any of the elements in a `Range` or in the sequence [`first`, `last`) are true.
///
/// \module Logical
/// \notes If empty, returns false.
template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>> auto any(Iter first, Sent last)
{
  for (; first != last; ++first)
    if (static_cast<bool>(*first))
      return true;
  return false;
}

/// \group Any
template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto any(const Rng& rng)
{
  return any(ranges::begin(rng), ranges::end(rng));
}

constexpr auto prod_args() -> detail::forward_arithmetic { return {}; }

template <typename T, typename... Args> constexpr auto prod_args(const T& arg, Args&&... args)
{
  return arg * prod_args(std::forward<Args>(args)...);
}

constexpr auto sum_args() -> detail::forward_arithmetic { return {}; }

template <typename T, typename... Args> constexpr auto sum_args(const T& arg, Args&&... args)
{
  return arg * sum_args(std::forward<Args>(args)...);
}

constexpr auto all_args() { return true; }

template <typename... Args> constexpr auto all_args(bool arg, Args&&... args)
{
  return arg && all_args(std::forward<Args>(args)...);
}

constexpr auto any_args() { return false; }

template <typename... Args> constexpr auto any_args(bool arg, Args&&... args)
{
  return arg || any_args(std::forward<Args>(args)...);
}

} // namespace jules

#endif // JULES_BASE_NUMERIC_H
