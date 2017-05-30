// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_MATH_H
#define JULES_BASE_MATH_H

#include <cmath>

namespace jules
{

template <class T> constexpr auto pi = T(3.1415926535897932385);

template <typename T> constexpr auto square(const T& value) { return value * value; }

template <typename T> auto normal_pdf(const T& x, const T& mu, const T& sigma)
{
  auto sigma2 = 2 * square(sigma);
  return std::exp(-square(x - mu) / sigma2) / std::sqrt(sigma2 * pi<T>);
}

} // namespace jules

#endif // JULES_BASE_MATH_H
