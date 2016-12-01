#ifndef JULES_BASE_NUMERIC_H
#define JULES_BASE_NUMERIC_H

#include <jules/core/range.hpp>

#include <array>
#include <functional>
#include <numeric>
#include <utility>
#include <type_traits>

namespace jules
{

namespace detail
{

template <typename T, std::size_t... I>
constexpr auto repeat_impl(const T& value, std::index_sequence<I...>) -> std::array<T, sizeof...(I)>
{
  return {{((void)I, value)...}};
}

} // namespace detail

template <std::size_t N, typename T> constexpr auto repeat(const T& value)
{
  return detail::repeat_impl(value, std::make_index_sequence<N>());
}

template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>,
          CONCEPT_REQUIRES_(range::Sentinel<Sent, Iter>())>
auto prod(Iter first, Sent last, const T& start = static_cast<T>(1u))
{
  return std::accumulate(first, last, start, std::multiplies<T>{});
}

template <typename Rng, typename T = range::range_value_t<Rng>, CONCEPT_REQUIRES_(range::Range<Rng>())>
auto prod(Rng&& rng, const T& start = static_cast<T>(1u))
{
  return prod(ranges::begin(rng), ranges::end(rng), start);
}

constexpr auto all() { return true; }

template <typename... Args> constexpr auto all(bool arg, Args&&... args) {
  return arg && all(std::forward<Args>(args)...);
}

} // namespace jules

#endif // JULES_BASE_NUMERIC_H
