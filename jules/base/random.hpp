// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_RANDOM_H
#define JULES_BASE_RANDOM_H

#include <jules/base/const_vector.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>

#include <cmath>
#include <random>
#include <vector>

namespace jules
{

struct no_replacement_t {
  constexpr explicit no_replacement_t() = default;
};

static constexpr auto no_replacement = no_replacement_t{};

static inline auto construct_default_random_engine() noexcept { return std::mt19937{std::random_device{}()}; }

static thread_local auto random_engine = construct_default_random_engine();

static inline auto rand() { return std::generate_canonical<numeric, numeric_traits<numeric>::digits>(random_engine); }

template <typename Dist> auto sample(Dist& dist) { return dist(random_engine); }

template <typename Dist> auto sample(index_t n, Dist& dist)
{
  auto rng = range::view::generate_n([&] { return dist(random_engine); }, n) | range::view::bounded;
  return const_vector<decltype(dist(random_engine))>(range::begin(rng), range::end(rng));
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

static inline auto uniform_index_sample(index_t n, index_t end)
{
  auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
  return sample(n, dist);
}

static inline auto uniform_index_sample(index_t end)
{
  auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
  return sample(dist);
}

static inline auto uniform_index_sample(index_t n, index_t end, no_replacement_t) -> const_vector<index_t>
{
  // inspired by John D. Cook, http://stackoverflow.com/a/311716/15485
  auto t = index_t{0};
  auto m = index_t{0};

  auto result = std::vector<index_t>();
  result.reserve(n);

  while (m < n) {
    const auto u = rand();

    if ((end - n) * u < n - m) {
      result.push_back(t);
      ++m;
    }
    ++t;
  }

  return {std::move(result)};
}

template <typename Rng, typename U = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto discrete_index_sample(index_t n, const Rng& rng)
{
  static_assert(std::is_convertible<U, double>::value, "weights must be convertible to double");
  auto dist = std::discrete_distribution<index_t>(range::begin(rng), range::end(rng));
  return sample(n, dist);
}

template <typename Rng, typename U = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
auto discrete_index_sample(const Rng& rng)
{
  static_assert(std::is_convertible<U, double>::value, "weights must be convertible to double");
  auto dist = std::discrete_distribution<index_t>(range::begin(rng), range::end(rng));
  return sample(dist);
}

} // namespace jules

#endif // JULES_BASE_RANDOM_H
