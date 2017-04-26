#include "jules/array/ref_array.hpp"

#include <catch.hpp>

#include <vector>

TEST_CASE("Basic reference array functionalities", "[array]")
{
  using jules::base_slice;
  using jules::ref_array;
  using jules::numeric;

  auto values = std::vector<numeric>(10ul, 0.0);

  auto vector = ref_array<numeric, 1>(values.data(), {0ul, 10ul});
  REQUIRE(values.data() == vector.data());

  vector = 1.0;
  for (auto value : values)
    CHECK(static_cast<double>(value) == 1.0);

  auto matrix = ref_array<numeric, 2>(values.data(), {0ul, {{2ul, 5ul}}});
  REQUIRE(values.data() == matrix.data());

  matrix = 2.0;
  for (auto value : values)
    CHECK(static_cast<double>(value) == 2.0);
}
