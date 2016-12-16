// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_RANDOM_H
#define JULES_BASE_RANDOM_H

#include <jules/array/all.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/range.hpp>

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

template <typename Dist> auto sample(Dist& dist) { return dist(random_engine); }

template <typename Dist> auto sample(index_t n, Dist& dist)
{
  auto rng = range::view::generate_n([&] { return dist(random_engine); }, n);
  return vector<bool>(rng);
}

static inline auto bernoulli_sample(index_t n, numeric p)
{
  auto dist = std::bernoulli_distribution(p);
  return sample(n, dist);
}

static inline auto bernoulli_sample(numeric p)
{
  auto dist = std::bernoulli_distribution(p);
  return sample(dist);
}

static inline auto index_sample(index_t end)
{
  auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
  return sample(dist);
}

} // namespace jules

#endif // JULES_BASE_RANDOM_H
