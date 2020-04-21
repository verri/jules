// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_MATH_H
#define JULES_ARRAY_MATH_H

#include <jules/array/functional.hpp>
#include <jules/base/math.hpp>

#include <cmath>

namespace jules
{

template <common_array Array> auto normal_pdf(const Array& array, typename Array::value_type mu, typename Array::value_type sigma)
{
  return apply(array, [mu = std::move(mu), sigma = std::move(sigma)](const auto& x) { return normal_pdf(x, mu, sigma); });
}

template <common_array Array> auto abs(const Array& array)
{
  return apply(array, [](const auto& v) { return abs(v); });
}

template <common_array Array> auto sqrt(const Array& array)
{
  return apply(array, [](const auto& v) { return sqrt(v); });
}

template <common_array Array> auto log(const Array& array)
{
  return apply(array, [](const auto& v) { return log(v); });
}

template <common_array Array> auto sin(const Array& array)
{
  return apply(array, [](const auto& v) { return sin(v); });
}

template <common_array Array> auto cos(const Array& array)
{
  return apply(array, [](const auto& v) { return cos(v); });
}

template <common_array Array> auto tanh(const Array& array)
{
  return apply(array, [](const auto& v) { return tanh(v); });
}

} // namespace jules

#endif // JULES_ARRAY_MATH_H
