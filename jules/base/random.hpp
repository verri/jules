// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_RANDOM_H
#define JULES_BASE_RANDOM_H

#include <jules/array/all.hpp>
#include <jules/base/numeric.hpp>

#include <cmath>
#include <random>

namespace jules
{

static thread_local auto random_engine = std::mt19937{};

template <typename T> auto normal_pdf(const T& x, const T& mu, const T& sigma)
{
  auto sigma2 = 2 * square(sigma);
  return std::exp(-square(x - mu)) / std::sqrt(sigma2 * pi<T>);
}

template <typename Array, typename T = typename Array::value_type, typename = array_request<void, Array>>
auto normal_pdf(const Array& array, T mu, T sigma)
{
  return apply(array, [ mu = std::move(mu), sigma = std::move(sigma) ](const auto& x) { return normal_pdf(x, mu, sigma); });
}

} // namespace jules

#endif // JULES_BASE_RANDOM_H
