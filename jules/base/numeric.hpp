// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_NUMERIC_H
#define JULES_BASE_NUMERIC_H

#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>

#include <array>
#include <cmath>
#include <type_traits>
#include <utility>
#include <vector>

namespace jules
{

template <class T> constexpr auto pi = T(3.1415926535897932385);

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
  template <typename T> decltype(auto) operator-(T&& value) { return std::forward<T>(value); }
  template <typename T> decltype(auto) operator*(T&& value) { return std::forward<T>(value); }
  template <typename T> decltype(auto) operator/(T&& value) { return std::forward<T>(value); }
  template <typename T> friend decltype(auto) operator+(T&& value, forward_arithmetic) { return std::forward<T>(value); }
  template <typename T> friend decltype(auto) operator-(T&& value, forward_arithmetic) { return std::forward<T>(value); }
  template <typename T> friend decltype(auto) operator*(T&& value, forward_arithmetic) { return std::forward<T>(value); }
  template <typename T> friend decltype(auto) operator/(T&& value, forward_arithmetic) { return std::forward<T>(value); }
};

} // namespace detail

template <std::size_t N, typename T> constexpr auto repeat(const T& value)
{
  return detail::repeat_impl(value, std::make_index_sequence<N>());
}

template <typename T> auto repeat(index_t n, const T& value) { return std::vector<T>(n, value); }

template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>> auto length(Iter first, Sent last)
{
  return range::distance(first, last);
}

template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto length(const Rng& rng)
{
  return ::jules::length(range::begin(rng), range::end(rng));
}

template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto max(Iter first, Sent last, T start = numeric_traits<T>::unbounded_min())
{
  for (; first != last; ++first)
    if (start < *first)
      start = *first;
  return start;
}

template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto max(const Rng& rng, T start = numeric_traits<T>::unbounded_min())
{
  return ::jules::max(range::begin(rng), range::end(rng), std::move(start));
}

template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto min(Iter first, Sent last, T start = numeric_traits<T>::unbounded_max())
{
  for (; first != last; ++first)
    if (start > *first)
      start = *first;
  return start;
}

template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto min(const Rng& rng, T start = numeric_traits<T>::unbounded_max())
{
  return ::jules::min(range::begin(rng), range::end(rng), std::move(start));
}

template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto prod(Iter first, Sent last, T start = numeric_traits<T>::multiplicative_identity())
{
  for (; first != last; ++first)
    start *= *first;
  return start;
}

template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto prod(const Rng& rng, T start = numeric_traits<T>::multiplicative_identity())
{
  return ::jules::prod(range::begin(rng), range::end(rng), std::move(start));
}

constexpr auto prod_args() -> detail::forward_arithmetic { return {}; }

template <typename T, typename... Args> constexpr auto prod_args(const T& arg, Args&&... args)
{
  return arg * prod_args(std::forward<Args>(args)...);
}

template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          typename = meta::requires<range::Sentinel<Sent, Iter>>>
auto sum(Iter first, Sent last, T start = numeric_traits<T>::additive_identity())
{
  for (; first != last; ++first)
    start += *first;
  return start;
}

template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto sum(const Rng& rng, T start = numeric_traits<T>::additive_identity())
{
  return ::jules::sum(range::begin(rng), range::end(rng), std::move(start));
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

template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>> auto all(Iter first, Sent last)
{
  while (first != last)
    if (!static_cast<bool>(*first++))
      return false;
  return true;
}

template <typename Rng, typename = meta::requires<range::Range<Rng>>> auto all(const Rng& rng)
{
  return all(ranges::begin(rng), ranges::end(rng));
}

constexpr auto any_args() { return false; }

template <typename... Args> constexpr auto any_args(bool arg, Args&&... args)
{
  return arg || any_args(std::forward<Args>(args)...);
}

template <typename T> constexpr auto square(const T& value) { return value * value; }

template <typename T> auto normal_pdf(const T& x, const T& mu, const T& sigma)
{
  auto sigma2 = 2 * square(sigma);
  return std::exp(-square(x - mu)) / std::sqrt(sigma2 * pi<T>);
}

} // namespace jules

#endif // JULES_BASE_NUMERIC_H
