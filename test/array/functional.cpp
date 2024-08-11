#include "jules/array/array.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Apply in place", "[array]")
{
  using namespace jules;

  auto v = vector<>{1, 2, 3};
  apply(in_place, v, [](auto& x) { x *= 2.0; });
  CHECK(all(v == cat(2, 4, 6)));

  apply(in_place, v, vector<>{1, 2, 3}, [](auto& x, auto y) { x /= y; });
  CHECK(all(v == 2));
}
