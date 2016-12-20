#include "jules/base/random.hpp"

#include <catch.hpp>

TEST_CASE("Basic random operations", "[random]")
{
  using namespace jules;

  CHECK(bernoulli_sample(0.0) == false);
  CHECK(bernoulli_sample(1.0) == true);

  auto samples = bernoulli_sample(10u, 0.5);
  CHECK(samples.size() == 10u);
}
