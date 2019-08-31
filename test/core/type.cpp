#include "jules/core/type.hpp"

#include <catch.hpp>

TEST_CASE("Overloaded lambdas", "[type]")
{
  using namespace jules;
  const auto f = overloaded([](int) { return 1; }, [](double) { return 2; });
  CHECK(f(0) == 1);
  CHECK(f(0.0) == 2);
}
