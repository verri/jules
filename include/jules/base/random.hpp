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

namespace detail
{
struct int_bernoulli_distribution
{
  using result_type = int;
  template <typename G> auto operator()(G& rng_) -> int { return static_cast<int>(dist(rng_)); }
  std::bernoulli_distribution dist;
};
} // namespace detail

template <typename Rng = std::mt19937> class random_engine
{
public:
  random_engine() : rng_(std::random_device{}()) {}

  template <typename... Args> random_engine(Args&&... args) : rng_(std::forward<Args>(args)...) {}

  // Generic sampling
  template <typename Dist> auto sample(Dist& dist) { return dist(rng_); }

  template <typename Dist, typename T = typename Dist::result_type, container_for<T> C = container<T>>
  auto sample(index_t n, Dist& dist, C result = container<T>{}) -> C
  {
    result.reserve(result.size() + n);

    auto rng = ranges::views::generate_n([&] { return dist(rng_); }, n);
    ranges::copy(ranges::begin(rng), ranges::end(rng), ranges::back_inserter(result));

    return result;
  }

  // Canonical sampling

  auto canon_sample() { return std::generate_canonical<numeric, numeric_traits<numeric>::digits>(rng_); }

  template <container_for<numeric> C = container<numeric>> auto canon_sample(index_t n, C result = container<numeric>{}) -> C
  {
    result.reserve(result.size() + n);

    auto rng = ranges::views::generate_n([&] { return this->canon_sample(); }, n);
    ranges::copy(ranges::begin(rng), ranges::end(rng), ranges::back_inserter(result));

    return result;
  }

  // Uniform index sampling

  auto uniform_index_sample(index_t end)
  {
    auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
    return this->sample(dist);
  }

  template <container_for<index_t> C = container<index_t>>
  auto uniform_index_sample(index_t n, index_t end, C result = container<index_t>{}) -> C
  {
    auto dist = std::uniform_int_distribution<index_t>(0u, end - 1);
    return this->sample(n, dist, std::move(result));
  }

  template <container_for<index_t> C = container<index_t>>
  auto uniform_index_sample(no_replacement_t, index_t n, index_t end, C result = container<index_t>{}) -> C
  {
    // inspired by John D. Cook, http://stackoverflow.com/a/311716/15485
    auto t = index_t{0};
    auto m = index_t{0};

    result.reserve(result.size() + n);

    while (m < n) {
      if ((end - t) * this->canon_sample() < n - m) {
        result.push_back(t);
        ++m;
      }
      ++t;
    }

    return result;
  }

  // Bernoulli samping

  auto sample(bernoulli_dist<numeric> dist)
  {
    auto bdist = detail::int_bernoulli_distribution{std::bernoulli_distribution(dist.p)};
    return this->sample(bdist);
  }

  template <container_for<int> C = container<int>>
  auto sample(index_t n, bernoulli_dist<numeric> dist, C result = container<int>{}) -> C
  {
    auto bdist = detail::int_bernoulli_distribution{std::bernoulli_distribution(dist.p)};
    return this->sample(n, bdist, std::move(result));
  }

private:
  Rng rng_;
};

} // namespace jules

#endif // JULES_BASE_RANDOM_H
