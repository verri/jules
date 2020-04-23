// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_RANDOM_H
/// \exclude
#define JULES_BASE_RANDOM_H

#include <jules/base/math.hpp>
#include <jules/core/concepts.hpp>
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

// Generic sampling

template <typename Dist, typename G = decltype(random_engine)> auto sample(Dist& dist, G& g = random_engine) { return dist(g); }

template <typename Dist, typename G = decltype(random_engine), typename T = typename Dist::result_type,
          container_for<T> C = container<T>>
auto sample(index_t n, Dist& dist, G& g = random_engine, C result = container<T>{}) -> C
{
  result.reserve(result.size() + n);

  auto rng = ranges::views::generate_n([&] { return dist(g); }, n);
  ranges::copy(ranges::begin(rng), ranges::end(rng), ranges::back_inserter(result));

  return result;
}

// Canonical sampling

template <typename G = decltype(random_engine)> auto canon_sample(G& g = random_engine)
{
  return std::generate_canonical<numeric, numeric_traits<numeric>::digits>(g);
}

template <typename G = decltype(random_engine), container_for<numeric> C = container<numeric>>
auto canon_sample(index_t n, G& g = random_engine, C result = container<numeric>{}) -> C
{
  result.reserve(result.size() + n);

  auto rng = ranges::views::generate_n([&] { return canon_sample(g); }, n);
  ranges::copy(ranges::begin(rng), ranges::end(rng), ranges::back_inserter(result));

  return result;
}

// Uniform index sampling

template <typename G = decltype(random_engine)> auto uniform_index_sample(index_t end, G& g = random_engine)
{
  auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
  return sample(dist, g);
}

template <typename G = decltype(random_engine), container_for<index_t> C = container<index_t>>
auto uniform_index_sample(index_t n, index_t end, G& g = random_engine, C result = container<index_t>{}) -> C
{
  auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
  return sample(n, dist, g, std::move(result));
}

template <typename G = decltype(random_engine), container_for<index_t> C = container<index_t>>
auto uniform_index_sample(no_replacement_t, index_t n, index_t end, G& g = random_engine, C result = container<index_t>{}) -> C
{
  // inspired by John D. Cook, http://stackoverflow.com/a/311716/15485
  auto t = index_t{0};
  auto m = index_t{0};

  result.reserve(result.size() + n);

  while (m < n) {
    if ((end - t) * canon_sample(g) < n - m) {
      result.push_back(t);
      ++m;
    }
    ++t;
  }

  return result;
}

// Bernoulli samping

namespace detail
{
struct int_bernoulli_distribution
{
  using result_type = int;
  template <typename G> auto operator()(G& g) -> int { return static_cast<int>(dist(g)); }
  std::bernoulli_distribution dist;
};
} // namespace detail

template <typename G = decltype(random_engine)> auto sample(bernoulli_dist<numeric> dist, G& g = random_engine)
{
  auto bdist = detail::int_bernoulli_distribution{std::bernoulli_distribution(dist.p)};
  return sample(bdist, g);
}

template <typename G = decltype(random_engine), container_for<int> C = container<int>>
auto sample(index_t n, bernoulli_dist<numeric> dist, G& g = random_engine, C result = container<int>{}) -> C
{
  auto bdist = detail::int_bernoulli_distribution{std::bernoulli_distribution(dist.p)};
  return sample(n, bdist, g, std::move(result));
}

} // namespace jules

#endif // JULES_BASE_RANDOM_H
