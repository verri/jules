#ifdef NOT_DEFINED
#include "jules/array/all.hpp"

#include <catch.hpp>

TEST_CASE("Basic array math", "[array]")
{
  const auto values = jules::vector<int>{-3, -2, -1, 0, 1, 2, 3};
  CHECK(all(abs(values) == jules::as_vector(3, 2, 1, 0, 1, 2, 3)));
}
#endif
