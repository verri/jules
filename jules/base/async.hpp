// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_ASYNC_H
#define JULES_BASE_ASYNC_H

#include <type_traits>
#include <utility>

namespace jules
{
namespace detail
{
template <typename F> struct defer_helper {
  defer_helper(F f) : f_{std::move(f)} {}
  defer_helper(const defer_helper&) = default;
  defer_helper(defer_helper&&) = default;
  auto operator=(const defer_helper&) -> defer_helper& = default;
  auto operator=(defer_helper &&) -> defer_helper& = default;
  ~defer_helper() { f_(); }
  F f_;
};

} // namespace detail

template <typename F> auto defer(F&& f) { return detail::defer_helper<F>{std::forward<F>(f)}; }
template <typename T> T* move_ptr(T*& pointer)
{
  T* tmp = pointer;
  pointer = nullptr;
  return tmp;
}

} // namespace jules

#endif // JULES_BASE_ASYNC_H
