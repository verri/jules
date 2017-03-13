// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_ASYNC_H
#define JULES_BASE_ASYNC_H

#include <type_traits>
#include <utility>

namespace jules
{
/// \exclude
namespace detail
{
template <typename F> struct defer_helper {
  defer_helper(F f) : f_{std::move(f)} {}

  defer_helper(const defer_helper&) = default;
  defer_helper(defer_helper&&) noexcept = default;

  auto operator=(const defer_helper&) -> defer_helper& = default;
  auto operator=(defer_helper&&) noexcept -> defer_helper& = default;

  ~defer_helper() { f_(); }

  F f_;
};

template <typename F> auto defer(F&& f) { return defer_helper<F>{std::forward<F>(f)}; }
} // namespace detail

#define JULES_TOKEN_CONCAT(X, Y) X##Y
#define JULES_TOKEN_PASTE(X, Y) JULES_TOKEN_CONCAT(X, Y)

#define JULES_DEFER(...) auto JULES_TOKEN_PASTE(_deferred, __LINE__) = ::jules::detail::defer([&] { __VA_ARGS__; })

template <typename T> auto move_ptr(T*& pointer) { return std::exchange(pointer, nullptr); }

} // namespace jules

#endif // JULES_BASE_ASYNC_H
