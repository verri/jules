// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_MATH_H
#define JULES_ARRAY_MATH_H

#include <jules/array/functional.hpp>
#include <jules/base/math.hpp>

#include <cmath>

namespace jules
{

template <typename A, typename T = typename A::value_type, typename = meta::requires<Array<A>>>
auto normal_pdf(const A& array, T mu, T sigma)
{
  return apply(array, [ mu = std::move(mu), sigma = std::move(sigma) ](const auto& x) { return normal_pdf(x, mu, sigma); });
}

template <typename A, typename = meta::requires<Array<A>>> auto abs(const A& array)
{
  return apply(array, [](const auto& v) { return std::abs(v); });
}

template <typename A, typename = meta::requires<Array<A>>> auto sqrt(const A& array)
{
  return apply(array, [](const auto& v) { return std::sqrt(v); });
}

template <typename A, typename = meta::requires<Array<A>>> auto log(const A& array)
{
  return apply(array, [](const auto& v) { return std::log(v); });
}

} // namespace jules

#endif // JULES_ARRAY_MATH_H
