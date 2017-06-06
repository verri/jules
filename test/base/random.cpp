#include "jules/base/random.hpp"

#include <algorithm>
#include <catch.hpp>

TEST_CASE("Basic random operations", "[random]")
{
  using namespace jules;

  CHECK(bernoulli_sample(0.0) == false);
  CHECK(bernoulli_sample(1.0) == true);

  const auto samples = bernoulli_sample(10u, 0.5);
  CHECK(samples.size() == 10u);

  const auto ix = uniform_index_sample(10, 10, no_replacement);
  CHECK(ix.size() == 10u);

  {
    auto answer = std::array<index_t, 10>{{}};
    std::iota(begin(answer), end(answer), 0u);

    const auto[ita, itb] = std::mismatch(ix.begin(), ix.end(), begin(answer));
    CHECK(ita == ix.end());
  }
}
