#include "jules/base/random.hpp"

#include <algorithm>
#include <catch.hpp>

TEST_CASE("Basic random operations", "[random]")
{
  using namespace jules;

  CHECK(canon_sample() < 1);
  CHECK(canon_sample() >= 0);

  auto rd = std::random_device{};

  CHECK(canon_sample(rd) < 1);
  CHECK(canon_sample(rd) >= 0);

  CHECK(bernoulli_sample(0.0) == false);
  CHECK(bernoulli_sample(1.0) == true);

  const auto samples = bernoulli_sample(10u, 0.5);
  CHECK(samples.size() == 10u);

  const auto ix = uniform_index_sample(no_replacement, 10, 10);
  CHECK(ix.size() == 10u);

  {
    auto answer = std::array<index_t, 10>{{}};
    std::iota(begin(answer), end(answer), 0u);

    const auto [ita, itb] = std::mismatch(ix.begin(), ix.end(), begin(answer));
    CHECK(ita == ix.end());
  }
}
