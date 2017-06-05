#include "jules/array/array.hpp"

#include <catch.hpp>

TEST_CASE("Basic array math", "[array]")
{
  const auto values = jules::vector<int>{-3, -2, -1, 0, 1, 2, 3};
  CHECK(all(abs(values) == jules::cat(jules::seq(3, 1, -1), 0, jules::seq(1, 3))));
}
