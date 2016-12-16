// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_RANGE_H
#define JULES_CORE_RANGE_H

#include <jules/core/type.hpp>
#include <range/v3/all.hpp>

#include <type_traits>

namespace ranges
{
inline namespace v3
{
namespace view
{
template <typename T> auto numbers(T start, T step = {})
{
  static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");
  return ::ranges::v3::view::generate([ value = std::move(start), step = std::move(step) ]() mutable->T {
    auto previous = value;
    value += step;
    return previous;
  });
}

template <typename T> auto numbers_n(T start, ::jules::index_t n, T step = {})
{
  static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");
  return ::ranges::v3::view::generate_n([ value = std::move(start), step = std::move(step) ]() mutable->T {
    auto previous = value;
    value += step;
    return previous;
  },
                                        n);
}

template <typename T> auto interval(T start, T end, T step)
{
  static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");
  auto n = static_cast<::jules::index_t>((end - start) / step);
  return numbers_n(std::move(start), n, std::move(step));
}

template <typename T> auto interval_n(T start, T end, ::jules::index_t n)
{
  static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");
  auto step = (end - start) / n;
  return numbers_n(std::move(start), n, std::move(step));
}
} // namespace view
} // namespace v3
} // namespace ranges

namespace jules
{
namespace range
{
using namespace ranges::v3;
} // namespace range
} // namespace jules

#endif // JULES_CORE_RANGE_H
