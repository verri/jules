#include "jules/array/ref_array.hpp"
#include "jules/array/meta/reference.hpp"

#include <catch.hpp>

#include <vector>

TEST_CASE("Basic reference array functionalities", "[array]")
{
  constexpr auto size = jules::index_t{20u};

  auto values = std::vector<double>(10u, 0.0);

  auto vector = jules::ref_array<double, 1u>(values.data(), {{{values.size()}}});

  static_assert(jules::CommonArray<decltype(vector)>::value);
  static_assert(jules::ReferenceArray<decltype(vector)>::value);

  vector = 1.0;
  CHECK((std::find_if(values.begin(), values.end(), [](auto value) { return value != 1.0; }) == values.end()));

  auto matrix = jules::ref_array<double, 2u>(values.data(), {{{2u, 5u}}});

  static_assert(jules::CommonArray<decltype(matrix)>::value);
  static_assert(jules::ReferenceArray<decltype(matrix)>::value);

  matrix = 2.0;
  CHECK((std::find_if(values.begin(), values.end(), [](auto value) { return value != 2.0; }) == values.end()));

  matrix[0u][0u] = 3.0;
  CHECK(values[0] == 3.0);

  matrix[0u][1u] = 4.0;
  CHECK(values[matrix.row_count()] == 4.0);

  vector[{0u, vector.size()}] = 0.0;
  CHECK((std::find_if(values.begin(), values.end(), [](auto value) { return value != 0.0; }) == values.end()));

  const auto ix = std::vector<jules::index_t>{0u, 7u, 9u};
  vector[ix] = 3.0;
  CHECK((values[0] == 3.0 && values[7] == 3.0 && values[9] == 3.0));
}
