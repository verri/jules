#include "jules/base/math.hpp"

#include <catch2/catch_test_macros.hpp>

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

  constexpr auto vnormal = vpdf(normal_dist{});
  constexpr auto vuniform = vpdf(uniform_dist{});

  CHECK(vnormal(0.0) < 1.0);
  CHECK(vuniform(0.0) < 1.0);

  std::optional<numeric> x;
  REQUIRE(!x.has_value());
  CHECK(vabs(x) == std::nullopt);

  x = -1.0;
  REQUIRE(x.has_value());
  CHECK(vabs(x) == 1.0);

  constexpr auto fix_na = replace_missing(0);

  x = std::nullopt;
  CHECK(fix_na(x) == 0);

  x = 1;
  CHECK(fix_na(x) == 1);

  CHECK(pmax(1, 2) == std::max(1, 2));
  CHECK(pmin(1, 2) == std::min(1, 2));
}
