// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_MATH_H
#define JULES_BASE_MATH_H

#include <cmath>

namespace jules
{

template <class T> constexpr auto pi = T(3.1415926535897932385);

using std::sqrt;
using std::exp;
using std::abs;
using std::log;
using std::sin;
using std::cos;

template <typename T> constexpr auto square(const T& value) { return value * value; }

template <typename T> auto normal_pdf(const T& x, const T& mu, const T& sigma)
{
  auto sigma2 = 2 * square(sigma);
  return exp(-square(x - mu) / sigma2) / sqrt(sigma2 * pi<T>);
}

} // namespace jules

#endif // JULES_BASE_MATH_H
