#include "jules/array/array.hpp"

#include <catch.hpp>

TEST_CASE("Apply in place", "[array]")
{
  auto v = jules::vector<>{1, 2, 3};
  apply(jules::in_place, v, [](auto& x) { x *= 2.0; });
  CHECK(all(v == jules::cat(2, 4, 6)));

  apply(jules::in_place, v, jules::vector<>{1, 2, 3}, [](auto& x, auto y) { x /= y; });
  CHECK(all(v == 2));
}
