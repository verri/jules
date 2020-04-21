// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_RANDOM_H
#define JULES_BASE_RANDOM_H

#include <jules/base/const_vector.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/ranges.hpp>

#include <cmath>
#include <random>
#include <vector>

namespace jules
{

struct no_replacement_t
{
  constexpr explicit no_replacement_t() = default;
};

static constexpr auto no_replacement = no_replacement_t{};

static inline auto construct_default_random_engine() noexcept { return std::mt19937{std::random_device{}()}; }

static thread_local auto random_engine = construct_default_random_engine();

template <typename G = decltype(random_engine)> auto canon_sample(G& g = random_engine)
{
  return std::generate_canonical<numeric, numeric_traits<numeric>::digits>(g);
}

template <typename Dist, typename G = decltype(random_engine)> auto sample(Dist& dist, G& g = random_engine) { return dist(g); }

template <typename Dist, typename G = decltype(random_engine)> auto sample(index_t n, Dist& dist, G& g = random_engine)
{
  auto rng = ranges::views::generate_n([&] { return dist(g); }, n) | ranges::views::common;
  return const_vector<decltype(dist(g))>(ranges::begin(rng), ranges::end(rng));
}

template <typename G = decltype(random_engine)> auto bernoulli_sample(index_t n, numeric p, G& g = random_engine)
{
  auto dist = std::bernoulli_distribution(p);
  return sample(n, dist, g);
}

template <typename G = decltype(random_engine)> auto bernoulli_sample(numeric p, G& g = random_engine)
{
  auto dist = std::bernoulli_distribution(p);
  return sample(dist, g);
}

template <typename G = decltype(random_engine)> auto uniform_index_sample(index_t n, index_t end, G& g = random_engine)
{
  auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
  return sample(n, dist, g);
}

template <typename G = decltype(random_engine)> auto uniform_index_sample(index_t end, G& g = random_engine)
{
  auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
  return sample(dist, g);
}

template <typename G = decltype(random_engine)>
auto uniform_index_sample(no_replacement_t, index_t n, index_t end, G& g = random_engine) -> const_vector<index_t>
{
  // inspired by John D. Cook, http://stackoverflow.com/a/311716/15485
  auto t = index_t{0};
  auto m = index_t{0};

  auto result = std::vector<index_t>();
  result.reserve(n);

  while (m < n) {
    if ((end - t) * canon_sample(g) < n - m) {
      result.push_back(t);
      ++m;
    }
    ++t;
  }

  return {std::move(result)};
}

template <ranges::range Rng, typename G = decltype(random_engine), typename U = ranges::range_value_t<Rng>>
auto discrete_index_sample(index_t n, const Rng& rng, G& g = random_engine)
{
  static_assert(std::is_convertible<U, double>::value, "weights must be convertible to double");
  auto dist = std::discrete_distribution<index_t>(ranges::begin(rng), ranges::end(rng));
  return sample(n, dist, g);
}

template <ranges::range Rng, typename G = decltype(random_engine), typename U = ranges::range_value_t<Rng>>
auto discrete_index_sample(const Rng& rng, G& g = random_engine)
{
  static_assert(std::is_convertible<U, double>::value, "weights must be convertible to double");
  auto dist = std::discrete_distribution<index_t>(ranges::begin(rng), ranges::end(rng));
  return sample(dist, g);
}

} // namespace jules

#endif // JULES_BASE_RANDOM_H
