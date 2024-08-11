// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_ASYNC_H
/// \exclude
#define JULES_BASE_ASYNC_H

#include <type_traits>
#include <utility>

namespace jules::detail
{
template <typename F> struct defer_helper
{
  static_assert(std::is_nothrow_invocable_r_v<void, F>);

  defer_helper(F f) : f_{std::move(f)} {}

  defer_helper(const defer_helper&) = delete;
  defer_helper(defer_helper&&) noexcept = delete;

  auto operator=(const defer_helper&) -> defer_helper& = delete;
  auto operator=(defer_helper&&) noexcept -> defer_helper& = delete;

  ~defer_helper() noexcept { f_(); }

private:
  F f_;
};

template <typename F> auto defer(F&& f) { return defer_helper<F>{std::forward<F>(f)}; }
} // namespace jules::detail

#define JULES_TOKEN_CONCAT(X, Y) X##Y
#define JULES_TOKEN_PASTE(X, Y) JULES_TOKEN_CONCAT(X, Y)

#define JULES_DEFER(...) auto JULES_TOKEN_PASTE(_deferred, __LINE__) = ::jules::detail::defer([&]() noexcept { __VA_ARGS__; })

#endif // JULES_BASE_ASYNC_H
