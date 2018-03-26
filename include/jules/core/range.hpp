// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_RANGE_H
#define JULES_CORE_RANGE_H

#include <jules/core/detail/numbers.hpp>
#include <jules/core/type.hpp>
#include <range/v3/all.hpp>

namespace ranges
{
inline namespace v3
{
namespace view
{
template <typename T> auto& numbers(T start, T step = {})
{
  // TODO: fix this ugly workaround: the view cannot be a r-value to interact with other
  // views.  An alternative is to implement numbers class.
  thread_local auto view = ::ranges::v3::view::unbounded(::jules::detail::number_iterator<T>{});
  view = ::ranges::v3::view::unbounded(::jules::detail::number_iterator<T>{start, step});
  return view;
}

template <typename T> auto numbers_n(T start, ::jules::index_t n, T step = {})
{
  return numbers(start, step) | ::ranges::v3::view::take_exactly(n);
}

template <typename T> auto interval(T start, T end, T step)
{
  const auto n = static_cast<::jules::index_t>((end - start) / step);
  return numbers_n(std::move(start), n, std::move(step));
}

template <typename T> auto interval_n(T start, T end, ::jules::index_t n)
{
  return numbers_n(std::move(start), n, (end - start) / n);
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
using ranges::v3::view::indices;
using ranges::v3::view::closed_indices;
} // namespace jules

#endif // JULES_CORE_RANGE_H
