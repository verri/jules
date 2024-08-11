// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_MATH_H
/// \exclude
#define JULES_BASE_MATH_H

#include <jules/base/concepts.hpp>
#include <jules/core/type.hpp>

#include <cmath>
#include <optional>
#include <utility>

namespace jules
{

template <class T> constexpr auto pi = T(3.14159265358979323846264338327950288419716939937510);

constexpr auto identity = []<typename T>(T value) -> T { return value; };
constexpr auto lhs = []<typename T>(T value, const T&) -> T { return value; };
constexpr auto rhs = []<typename T>(const T&, T value) -> T { return value; };

template <typename T, applies_to<T> Op> constexpr auto apply(T&& operand, Op op)
{
  constexpr typename apply_traits<std::decay_t<T>>::apply_type a{};
  return a(std::forward<T>(operand), std::move(op));
}

template <typename T, typename U, applies_to_by<T, U> Op> constexpr auto apply(T&& lhs, U&& rhs, Op op)
{
  constexpr typename apply_traits<std::decay_t<T>>::apply_type a{};
  return a(std::forward<T>(lhs), std::forward<U>(rhs), std::move(op));
}

template <typename T, applies_in_place_to<T> Op> constexpr auto apply(in_place_t, T&& operand, Op op) -> decltype(auto)
{
  constexpr typename apply_traits<std::decay_t<T>>::apply_type a{};
  return a(in_place, std::forward<T>(operand), std::move(op));
}

template <typename T, typename U, applies_in_place_to_by<T, U> Op>
constexpr auto apply(in_place_t, T&& lhs, U&& rhs, Op op) -> decltype(auto)
{
  constexpr typename apply_traits<std::decay_t<T>>::apply_type a{};
  return a(in_place, std::forward<T>(lhs), std::forward<U>(rhs), std::move(op));
}

template <typename Op> class optional_operator
{
public:
  constexpr optional_operator(Op op) noexcept : op_(std::move(op)) {}

  template <typename T> constexpr auto operator()(const T& x) const { return op_(x); }

  template <typename T, typename U> constexpr auto operator()(const T& x, const U& y) const { return op_(x, y); }

  template <typename T> constexpr auto operator()(const std::optional<T>& x) const
  {
    return x.has_value() ? std::make_optional(op_(x.value())) : std::nullopt;
  }

  template <typename T, typename U> constexpr auto operator()(const std::optional<T>& x, const U& y) const
  {
    return x.has_value() ? std::make_optional(op_(x.value(), y)) : std::nullopt;
  }

  template <typename T, typename U> constexpr auto operator()(const T& x, const std::optional<U>& y) const
  {
    return y.has_value() ? std::make_optional(op_(x, y.value())) : std::nullopt;
  }

private:
  Op op_;
};

template <typename Op> constexpr auto unary_operator(Op op) noexcept
{
  return [op = std::move(op)]<typename T>(T&& value) {
    if constexpr (applies_to<Op, T>) {
      return apply(std::forward<T>(value), op);
    } else {
      return op(std::as_const(value));
    }
  };
}

template <typename Op> constexpr auto binary_operator(Op op) noexcept
{
  return [op = std::move(op)]<typename T, typename U>(T&& lhs, U&& rhs) {
    if constexpr (applies_to_by<Op, T, U>) {
      return apply(std::forward<T>(lhs), std::forward<U>(rhs), op);
    } else {
      return op(std::as_const(lhs), std::as_const(rhs));
    }
  };
}

namespace detail
{
template <typename T> auto abs_fn(const T& value)
{
  using std::abs;
  return abs(value);
}

template <typename T> auto exp_fn(const T& value)
{
  using std::exp;
  return exp(value);
}

template <typename T> auto log_fn(const T& value)
{
  using std::log;
  return log(value);
}

template <typename T> auto sin_fn(const T& value)
{
  using std::sin;
  return sin(value);
}

template <typename T> auto cos_fn(const T& value)
{
  using std::cos;
  return cos(value);
}

template <typename T> auto sqrt_fn(const T& value)
{
  using std::sqrt;
  return sqrt(value);
}

template <typename T> auto tanh_fn(const T& value)
{
  using std::tanh;
  return tanh(value);
}

template <typename T, typename U> auto min_fn(const T& lhs, const U& rhs)
{
  using std::fmin;
  return fmin(lhs, rhs);
}

template <typename T, typename U> auto max_fn(const T& lhs, const U& rhs)
{
  using std::fmax;
  return fmax(lhs, rhs);
}
} // namespace detail

constexpr auto vabs = unary_operator(optional_operator([]<common_numeric T>(const T& value) { return detail::abs_fn(value); }));

constexpr auto vexp = unary_operator(optional_operator([]<common_numeric T>(const T& value) { return detail::exp_fn(value); }));

constexpr auto vlog = unary_operator(optional_operator([]<common_numeric T>(const T& value) { return detail::log_fn(value); }));

constexpr auto vsin = unary_operator(optional_operator([]<common_numeric T>(const T& value) { return detail::sin_fn(value); }));

constexpr auto vcos = unary_operator(optional_operator([]<common_numeric T>(const T& value) { return detail::cos_fn(value); }));

constexpr auto vsqrt = unary_operator(optional_operator([]<common_numeric T>(const T& value) { return detail::sqrt_fn(value); }));

constexpr auto vtanh = unary_operator(optional_operator([]<common_numeric T>(const T& value) { return detail::tanh_fn(value); }));

constexpr auto vsq = unary_operator(optional_operator([]<common_numeric T>(const T& value) { return value * value; }));

template <common_numeric T = numeric> struct normal_dist
{
  T mu = numeric_traits<T>::additive_identity();
  T sigma = numeric_traits<T>::multiplicative_identity();
};

template <typename T> normal_dist(T, T) -> normal_dist<T>;

template <common_numeric T = numeric> struct uniform_dist
{
  T alpha = numeric_traits<T>::additive_identity();
  T beta = numeric_traits<T>::additive_identity() + 1;
};

template <typename T> uniform_dist(T, T) -> uniform_dist<T>;

template <common_numeric T = numeric> struct bernoulli_dist
{
  T p = 0.5;
};

template <typename T> bernoulli_dist(T) -> bernoulli_dist<T>;

constexpr auto vpdf = overloaded{
  []<typename T>(normal_dist<T> dist) {
    return unary_operator(optional_operator([dist](const T& x) -> T {
      const auto sigma2 = 2 * vsq(dist.sigma);
      return exp(-vsq(x - dist.mu) / sigma2) / sqrt(sigma2 * pi<T>);
    }));
  },
  []<typename T>(uniform_dist<T> dist) {
    return unary_operator(optional_operator(
      [dist](const T& x) -> T { return (x > dist.alpha && x < dist.beta) ? 1 / (dist.beta - dist.alpha) : 0; }));
  },
};

constexpr auto replace_missing = []<typename T>(T value) {
  return unary_operator([value = std::move(value)]<typename U>(const std::optional<U>& x) -> U {
    static_assert(convertible_to<T, U>);
    return x.value_or(value);
  });
};

constexpr auto pmin = binary_operator(
  optional_operator([]<common_numeric T, common_numeric U>(const T& lhs, const U& rhs) { return detail::min_fn(lhs, rhs); }));

constexpr auto pmax = binary_operator(
  optional_operator([]<common_numeric T, common_numeric U>(const T& lhs, const U& rhs) { return detail::max_fn(lhs, rhs); }));

} // namespace jules

#endif // JULES_BASE_MATH_H
