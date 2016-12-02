#include "jules/array/ref_array.hpp"

#include <catch.hpp>

#include <vector>

TEST_CASE("reference array", "[ref_array]")
{
  using jules::base_slice;
  using jules::ref_array;
  using jules::numeric;

  auto values = std::vector<numeric>(10ul, 0.0);

  // TODO: XXX: cannot use AAA-style in C++14 because RVO is not guaranteed.
  ref_array<numeric, 1> vector(values.data(), {0ul, 10ul});
  REQUIRE(values.data() == vector.data());

  vector = 1.0;
  for (auto value : values)
    CHECK(static_cast<double>(value) == 1.0);

  // TODO: XXX: cannot use AAA-style in C++14 because RVO is not guaranteed.
  ref_array<numeric, 2> matrix(values.data(), {0ul, {{2ul, 5ul}}});
  REQUIRE(values.data() == matrix.data());

  matrix = 2.0;
  for (auto value : values)
    CHECK(static_cast<double>(value) == 2.0);
}
