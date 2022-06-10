#include "jules/array/array.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Basic array math", "[array]")
{
  using namespace jules;

  const auto values = vector<int>{-3, -2, -1, 0, 1, 2, 3};
  CHECK(all(vabs(values) == cat(seq(3, 1, -1), 0, seq(1, 3))));

  {
    vector<int> x = {1, 2, 3};
    vector<int> y = {3, 2, 1};

    CHECK(all(pmin(x, y) == vector<int>{1, 2, 1}));
    CHECK(all(pmax(x, y) == vector<int>{3, 2, 3}));
  }
}
