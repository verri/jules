// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_MATH_H
/// \exclude
#define JULES_BASE_MATH_H

#include <jules/base/concepts.hpp>
#include <jules/core/type.hpp>

#include <cmath>
#include <utility>

namespace jules
{

template <class T> constexpr auto pi = T(3.14159265358979323846264338327950288419716939937510);

constexpr auto identity = []<typename T>(T value) -> T { return value; };
constexpr auto lhs = []<typename T>(T value, const T&) -> T { return value; };
constexpr auto rhs = []<typename T>(const T&, T value) -> T { return value; };

template <typename T, applies_to<T> Op> constexpr decltype(auto) apply(T&& operand, Op op)
{
  constexpr typename apply_traits<std::decay_t<T>>::apply_type apply{};
  return apply(std::forward<T>(operand), std::move(op));
}

template <typename T, typename U, applies_to_by<T, U> Op> constexpr decltype(auto) apply(T&& lhs, U&& rhs, Op op)
{
  constexpr typename apply_traits<std::decay_t<T>>::apply_type apply{};
  return apply(std::forward<T>(lhs), std::forward<U>(rhs), std::move(op));
}

template <typename T, applies_in_place_to<T> Op> constexpr decltype(auto) apply(in_place_t, T&& operand, Op op)
{
  constexpr typename apply_traits<std::decay_t<T>>::apply_type apply{};
  return apply(in_place, std::forward<T>(operand), std::move(op));
}

template <typename T, typename U, applies_in_place_to_by<T, U> Op>
constexpr decltype(auto) apply(in_place_t, T&& lhs, U&& rhs, Op op)
{
  constexpr typename apply_traits<std::decay_t<T>>::apply_type apply{};
  return apply(in_place, std::forward<T>(lhs), std::forward<U>(rhs), std::move(op));
}

template <typename Op> constexpr auto unary_operator(Op op) noexcept
{
  return [op]<typename T>(T&& value) {
    if constexpr (applies_to<Op, decltype(value)>) {
      return apply(std::forward<T>(value), std::move(op));
    } else {
      return op(std::as_const(value));
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
} // namespace detail

constexpr auto vabs = unary_operator([]<common_numeric T>(const T& value) { return detail::abs_fn(value); });

constexpr auto vexp = unary_operator([]<common_numeric T>(const T& value) { return detail::exp_fn(value); });

constexpr auto vlog = unary_operator([]<common_numeric T>(const T& value) { return detail::log_fn(value); });

constexpr auto vsin = unary_operator([]<common_numeric T>(const T& value) { return detail::sin_fn(value); });

constexpr auto vcos = unary_operator([]<common_numeric T>(const T& value) { return detail::cos_fn(value); });

constexpr auto vsqrt = unary_operator([]<common_numeric T>(const T& value) { return detail::sqrt_fn(value); });

constexpr auto vtanh = unary_operator([]<common_numeric T>(const T& value) { return detail::tanh_fn(value); });

constexpr auto vsq = unary_operator([]<common_numeric T>(const T& value) { return value * value; });

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
    return unary_operator([dist](const T& x) -> T {
      const auto sigma2 = 2 * vsq(dist.sigma);
      return exp(-vsq(x - dist.mu) / sigma2) / sqrt(sigma2 * pi<T>);
    });
  },
  []<typename T>(uniform_dist<T> dist) {
    return unary_operator(
      [dist](const T& x) -> T { return (x > dist.alpha && x < dist.beta) ? 1 / (dist.beta - dist.alpha) : 0; });
  },
};

// TODO: constexpr auto pmax =
// TODO: constexpr auto pmin =

} // namespace jules

#endif // JULES_BASE_MATH_H
