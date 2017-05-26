#include "jules/array/ref_array.hpp"

#include <catch.hpp>

#include <vector>

TEST_CASE("Basic reference array functionalities", "[array]")
{
  constexpr auto size = jules::index_t{20u};

  auto values = std::vector<double>(10u, 0.0);

  auto vector = jules::ref_array<double, 1u>(values.data(), {{{values.size()}}});

  vector = 1.0;
  CHECK((std::find_if(values.begin(), values.end(), [](auto value) { return value != 1.0; }) == values.end()));

  auto matrix = jules::ref_array<double, 2u>(values.data(), {{{2u, 5u}}});

  matrix = 2.0;
  CHECK((std::find_if(values.begin(), values.end(), [](auto value) { return value != 2.0; }) == values.end()));

  matrix[0u][0u] = 3.0;
  CHECK(values[0] == 3.0);

  matrix[0u][1u] = 4.0;
  CHECK(values[matrix.row_count()] == 4.0);
}
