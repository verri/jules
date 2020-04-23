#include "jules/base/math.hpp"

#include <catch.hpp>

TEST_CASE("Basic math operations", "[math]")
{
  using namespace jules;

  CHECK(identity(1) == 1);
  CHECK(lhs(1, 2) == 1);
  CHECK(rhs(1, 2) == 2);

  CHECK(vabs(0) == std::abs(0));

  CHECK(vcos(0) == std::cos(0));
  CHECK(vsin(0) == std::sin(0));

  CHECK(vexp(0) == std::exp(0));
  CHECK(vlog(0) == std::log(0));

  CHECK(vsqrt(0) == std::sqrt(0));
  CHECK(vtanh(0) == std::tanh(0));

  CHECK(vsq(-1) == 1);

  CHECK(vpdf(normal_dist{})(0.0) < 1.0);
  CHECK(vpdf(uniform_dist{})(0.0) < 1.0);
}
