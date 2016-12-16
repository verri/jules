// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_NUMERIC_H
#define JULES_BASE_NUMERIC_H

#include <jules/core/range.hpp>

#include <array>
#include <functional>
#include <numeric>
#include <type_traits>
#include <utility>

namespace jules
{

template <class T> constexpr auto pi = T(3.1415926535897932385);

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

template <typename Iter, typename Sent, CONCEPT_REQUIRES_(range::Sentinel<Sent, Iter>())> auto max(Iter first, Sent last)
{
  return std::max_element(first, last);
}

template <typename Rng, CONCEPT_REQUIRES_(range::Range<Rng>())> auto max(const Rng& rng)
{
  return std::max_element(range::begin(rng), range::end(rng));
}

template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          CONCEPT_REQUIRES_(range::Sentinel<Sent, Iter>())>
auto prod(Iter first, Sent last, const T& start = static_cast<T>(1u))
{
  return std::accumulate(first, last, start, std::multiplies<T>{});
}

template <typename Rng, typename T = range::range_value_t<Rng>, CONCEPT_REQUIRES_(range::Range<Rng>())>
auto prod(const Rng& rng, const T& start = static_cast<T>(1u))
{
  return prod(ranges::begin(rng), ranges::end(rng), start);
}

constexpr auto prod_args() -> detail::forward_arithmetic { return {}; }

template <typename T, typename... Args> constexpr auto prod_args(const T& arg, Args&&... args)
{
  return arg * prod_args(std::forward<Args>(args)...);
}

constexpr auto all_args() { return true; }

template <typename... Args> constexpr auto all_args(bool arg, Args&&... args)
{
  return arg && all_args(std::forward<Args>(args)...);
}

template <typename Iter, typename Sent, CONCEPT_REQUIRES_(range::Sentinel<Sent, Iter>())> auto all(Iter first, Sent last)
{
  while (first != last)
    if (!static_cast<bool>(*first++))
      return false;
  return true;
}

template <typename Rng, CONCEPT_REQUIRES_(range::Range<Rng>())> auto all(const Rng& rng)
{
  return all(ranges::begin(rng), ranges::end(rng));
}

constexpr auto any_args() { return false; }

template <typename... Args> constexpr auto any_args(bool arg, Args&&... args)
{
  return arg || any_args(std::forward<Args>(args)...);
}

template <typename T> constexpr auto square(const T& value) { return value * value; }

} // namespace jules

#endif // JULES_BASE_NUMERIC_H
