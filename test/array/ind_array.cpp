#include "jules/array/ind_array.hpp"

#include <catch.hpp>

#include <vector>

TEST_CASE("Basic indirect array functionalities", "[array]")
{
  using jules::base_slice;
  using jules::ind_array;
  using jules::numeric;
  using jules::index_t;

  auto values = std::vector<numeric>(10ul, 0.0);
  auto indexes = std::vector<index_t>(10u);
  std::iota(indexes.begin(), indexes.end(), 0u);

  // TODO: XXX: cannot use AAA-style in C++14 because RVO is not guaranteed.
  ind_array<numeric, 1> vector(values.data(), 10u, indexes);
  REQUIRE(values.data() == vector.data());

  vector = 2.0;
  for (auto value : values)
    CHECK(static_cast<double>(value) == 2.0);

  // TODO: XXX: cannot use AAA-style in C++14 because RVO is not guaranteed.
  ind_array<numeric, 2> matrix(values.data(), {{2ul, 5ul}}, indexes);
  REQUIRE(values.data() == matrix.data());

  matrix = 2.0;
  for (auto value : values)
    CHECK(static_cast<double>(value) == 2.0);
}
