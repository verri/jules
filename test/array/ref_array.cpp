#include "jules/array/ref_array.hpp"
#include "jules/array/meta/reference.hpp"
#include "jules/array/slicing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <vector>

TEST_CASE("Basic reference array functionalities", "[array]")
{
  auto values = std::vector<double>(10u, 0.0);

  auto vector = jules::ref_array<double, 1u>(values.data(), {{{values.size()}}});

  static_assert(jules::common_array<decltype(vector)>);
  static_assert(jules::reference_array<decltype(vector)>);

  vector = 1.0;
  CHECK((std::find_if(values.begin(), values.end(), [](auto value) { return value != 1.0; }) == values.end()));

  auto matrix = jules::ref_array<double, 2u>(values.data(), {{{2u, 5u}}});

  static_assert(jules::common_array<decltype(matrix)>);
  static_assert(jules::reference_array<decltype(matrix)>);

  matrix = 2.0;
  CHECK((std::find_if(values.begin(), values.end(), [](auto value) { return value != 2.0; }) == values.end()));

  matrix[0u][0u] = 3.0;
  CHECK(values[0] == 3.0);

  matrix[0u][1u] = 4.0;
  CHECK(values[matrix.dimensions()[0]] == 4.0);

  vector[{0u, vector.size()}] = 0.0;
  CHECK((std::find_if(values.begin(), values.end(), [](auto value) { return value != 0.0; }) == values.end()));

  const auto ix = std::vector<jules::index_t>{0u, 7u, 9u};
  vector[ix] = 3.0;
  CHECK((values[0] == 3.0 && values[7] == 3.0 && values[9] == 3.0));

  vector[{1u, 2u}] = 1.0;
  CHECK(values[1] == 1.0);
  CHECK(values[2] == 1.0);
}
