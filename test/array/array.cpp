#include "jules/array/all.hpp"

#include <catch.hpp>

TEST_CASE("array", "[array]")
{
  auto vector = jules::vector<>{1.0, 2.0, 3.0};

  auto matrix = jules::matrix<>{
    {1.0, 2.0, 3.0}, //
    {4.0, 5.0, 6.0}, //
    {7.0, 8.0, 9.0}, //
  };
}

TEST_CASE("base array", "[base_array]")
{
  using jules::base_array;
  using jules::base_slice;
  using jules::ind_array;
  using jules::ref_array;

  base_array<int, 2> matrix1(4u, 5u); // matrix 4x5
  CHECK(matrix1.row_count() == 4u);
  CHECK(matrix1.column_count() == 5u);
  CHECK(matrix1.size() == 4 * 5);

  base_array<int, 2> matrix2(3, 4u, 5u); // matrix 4x5 with values 3
  CHECK(matrix2.row_count() == 4);
  CHECK(matrix2.column_count() == 5);
  CHECK(matrix2.size() == 4 * 5);

  for (auto it = matrix2.data(); it != matrix2.data() + matrix2.size(); ++it)
    CHECK(*it == 3);

  int x[20];
  std::iota(std::begin(x), std::end(x), 0);
  base_array<int, 2> matrix3(x, 4u, 5u); // matrix 4x5 with values 0..19

  auto result = std::mismatch(std::begin(x), std::end(x), matrix3.data());
  CHECK(result.first == std::end(x));
  CHECK(result.second == matrix3.data() + matrix3.size());

  // base_array<int, 2> matrix10(3, 4u, 5u, 4u); // shouldn't not compile

  auto&& vector1 = matrix3[3]; // last line
  vector1 = 0;

  x[3] = x[7] = x[11] = x[15] = x[19] = 0;
  result = std::mismatch(std::begin(x), std::end(x), matrix3.data());
  CHECK(result.first == std::end(x));
  CHECK(result.second == matrix3.data() + 4);

  // auto&& expr0 =
  //   jules::make_expr_array<2>(matrix1.begin(), matrix1.end(), matrix2.begin(), matrix2.end(),
  //                          [](const int& a, const int& b) { return a + b; }, matrix1.extents());

  // auto&& expr1 = matrix1 + matrix2;
  // auto&& expr2 = matrix1[0] + matrix2[1];
  // auto&& expr3 = matrix1 + base_array<double, 0>(1.0);
}
