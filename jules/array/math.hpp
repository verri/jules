// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_MATH_H
#define JULES_ARRAY_MATH_H

#include <jules/array/functional.hpp>
#include <jules/base/math.hpp>

#include <cmath>

namespace jules
{

template <typename Array>
auto normal_pdf(const common_array_base<Array>& array, typename Array::value_type mu, typename Array::value_type sigma)
{
  return apply(array, [ mu = std::move(mu), sigma = std::move(sigma) ](const auto& x) { return normal_pdf(x, mu, sigma); });
}

template <typename Array> auto abs(const common_array_base<Array>& array)
{
  return apply(array, [](const auto& v) { return std::abs(v); });
}

template <typename Array> auto sqrt(const common_array_base<Array>& array)
{
  return apply(array, [](const auto& v) { return std::sqrt(v); });
}

template <typename Array> auto log(const common_array_base<Array>& array)
{
  return apply(array, [](const auto& v) { return std::log(v); });
}

} // namespace jules

#endif // JULES_ARRAY_MATH_H
