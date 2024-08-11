#include "jules/core/type.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Overloaded lambdas", "[type]")
{
  using namespace jules;
  const auto f = overloaded([](int) { return 1; }, [](double) { return 2; });
  CHECK(f(0) == 1);
  CHECK(f(0.0) == 2);

  // overloaded must be trivially copyable
  static_assert(std::is_trivially_copyable_v<decltype(f)>);

  const auto g = f;
  CHECK(f(0) == g(0));
  CHECK(f(0.0) == g(0.0));
}
