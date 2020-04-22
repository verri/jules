// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_MATH_H
/// \exclude
#define JULES_BASE_MATH_H

#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>

#include <cmath>
#include <utility>

namespace jules
{

constexpr auto identity = []<typename T>(const T& value) -> T { return value; };
constexpr auto lhs = []<typename T>(const T& value, const T&) -> T { return value; };
constexpr auto rhs = []<typename T>(const T&, const T& value) -> T { return value; };

template <typename T> struct apply_traits
{
  using apply_t = void;
};

template <typename T, typename Vector> concept apply_for = requires { typename Vector::value_type; }
&&requires(const T& apply, const Vector& vec, const typename Vector::value_type& value)
{
  {apply(vec, identity)};
  {apply(vec, vec, lhs)};
  {apply(value, vec, lhs)};
  {apply(vec, value, lhs)};
};

template <typename T> concept common_vector = ranges::range<T>&& apply_for<typename apply_traits<T>::apply_t, T>;

template <class T> constexpr auto pi = T(3.14159265358979323846264338327950288419716939937510);

template <typename Op> constexpr auto unary_operator(Op op) noexcept
{
  return [op]<typename T>(T&& value) {
    if constexpr (common_vector<std::decay_t<T>>) {
      constexpr typename apply_traits<std::decay_t<T>>::apply_t apply{};
      return apply(std::forward<T>(value), op);
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

// TODO: better names?

constexpr auto absv = unary_operator([]<common_numeric T>(const T& value) { return detail::abs_fn(value); });

constexpr auto expv = unary_operator([]<common_numeric T>(const T& value) { return detail::exp_fn(value); });

constexpr auto logv = unary_operator([]<common_numeric T>(const T& value) { return detail::log_fn(value); });

constexpr auto sinv = unary_operator([]<common_numeric T>(const T& value) { return detail::sin_fn(value); });

constexpr auto cosv = unary_operator([]<common_numeric T>(const T& value) { return detail::cos_fn(value); });

constexpr auto sqrtv = unary_operator([]<common_numeric T>(const T& value) { return detail::sqrt_fn(value); });

constexpr auto tanhv = unary_operator([]<common_numeric T>(const T& value) { return detail::tanh_fn(value); });

constexpr auto sqv = unary_operator([]<common_numeric T>(const T& value) { return value * value; });

template <common_numeric T = numeric> struct normal_distribution
{
  T mu = numeric_traits<T>::additive_identity();
  T sigma = numeric_traits<T>::multiplicative_identity();
};

template <common_numeric T = numeric> struct uniform_distribution
{
  T alpha = numeric_traits<T>::additive_identity();
  T beta = numeric_traits<T>::additive_identity() + 1;
};

constexpr auto pdf = overloaded{
  []<typename T>(normal_distribution<T> dist) {
    return unary_operator([dist](const T& x) -> T {
      const auto sigma2 = 2 * sqv(dist.sigma);
      return exp(-sqv(x - dist.mu) / sigma2) / sqrt(sigma2 * pi<T>);
    });
  },
  []<typename T>(uniform_distribution<T> dist) {
    return unary_operator(
      [dist](const T& x) -> T { return (x > dist.alpha && x < dist.beta) ? 1 / (dist.beta - dist.alpha) : 0; });
  },
};

} // namespace jules

#endif // JULES_BASE_MATH_H
