#include "jules/base/math.hpp"

#include <catch.hpp>

TEST_CASE("Basic math operations", "[math]")
{
  using namespace jules;

  CHECK(identity(1) == 1);
  CHECK(lhs(1, 2) == 1);
  CHECK(rhs(1, 2) == 2);

  CHECK(absv(0) == std::abs(0));

  CHECK(cosv(0) == std::cos(0));
  CHECK(sinv(0) == std::sin(0));

  CHECK(expv(0) == std::exp(0));
  CHECK(logv(0) == std::log(0));

  CHECK(sqrtv(0) == std::sqrt(0));
  CHECK(tanhv(0) == std::tanh(0));

  CHECK(sqv(-1) == 1);

  CHECK(pdf(normal_distribution<>{})(0.0) < 1.0);
  CHECK(pdf(uniform_distribution<>{})(0.0) < 1.0);
}
