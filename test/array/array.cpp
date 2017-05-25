#ifdef NOT_DEFINED
#include "jules/array/all.hpp"
#include "jules/array/meta/expression.hpp"
#include "jules/base/numeric.hpp"

#include <catch.hpp>

TEST_CASE("Basic array functionalities", "[array]")
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

  {
    auto result = std::mismatch(std::begin(x), std::end(x), matrix3.data());
    CHECK(result.first == std::end(x));
    CHECK(result.second == matrix3.data() + matrix3.size());
  }

  // base_array<int, 2> matrix10(3, 4u, 5u, 4u); // shouldn't not compile

  auto&& vector1 = matrix3[3]; // last line
  vector1 = 0;

  x[3] = x[7] = x[11] = x[15] = x[19] = 0;
  {
    auto result = std::mismatch(std::begin(x), std::end(x), matrix3.data());
    CHECK(result.first == std::end(x));
    CHECK(result.second == matrix3.end());
  }

  REQUIRE(matrix1.extents() == matrix2.extents());
  auto expr0 = jules::make_expr_array<2>(matrix1.begin(), matrix1.end(), matrix2.begin(), matrix2.end(),
                                         [](const int& a, const int& b) { return a + b; }, matrix1.extents());

  static_assert(jules::ExpressionArray<decltype(expr0)>::value);

  auto expr1 = jules::apply(matrix1, matrix2, [](int a, int b) { return a + b; });
  auto expr2 = matrix1 + matrix2;

  CHECK(expr1.extents() == expr2.extents());
  {
    auto result = std::mismatch(expr1.begin(), expr1.end(), expr2.begin());
    CHECK(result.first == expr1.end());
    CHECK(result.second == expr2.end());
  }

  auto expr3 = jules::apply(matrix1, [](int a) { return -a; });
  auto expr4 = -matrix1;

  CHECK(expr3.row_count() == matrix1.row_count());
  CHECK(expr3.column_count() == matrix1.column_count());

  CHECK(expr3.extents() == expr4.extents());
  {
    auto result = std::mismatch(expr3.begin(), expr3.end(), expr4.begin());
    CHECK(result.first == expr3.end());
    CHECK(result.second == expr4.end());
  }

  auto vector2 = eval(matrix1[0] + matrix2[1]);
  static_assert(std::is_same<decltype(vector2), jules::vector<int>>::value, "it should be a vector");

  auto expr5 = -vector2;
  CHECK(expr5.length() == vector2.length());

  static_assert(jules::ExpressionArray<decltype(expr5)>::value);
}

TEST_CASE("Type inference for as_vector", "[array]")
{
  auto array = std::array<jules::integer, 5>{{1, 2, 3, 4, 5}};
  auto vector1 = jules::to_vector<jules::integer>(array);
  auto vector2 = jules::as_vector(array);
  auto vector3 = jules::as_vector(jules::integer{1}, 2, 3, 4, 5);

  static_assert(std::is_same<decltype(vector1), decltype(vector2)>::value, "should be the same type");
  static_assert(std::is_same<decltype(vector2), decltype(vector3)>::value, "should be the same type");

  CHECK(vector1.length() == vector2.length());
  {
    auto result = std::mismatch(vector1.begin(), vector1.end(), vector2.begin());
    CHECK(result.first == vector1.end());
    CHECK(result.second == vector2.end());
  }

  CHECK(vector3.length() == vector2.length());
  {
    auto result = std::mismatch(vector3.begin(), vector3.end(), vector2.begin());
    CHECK(result.first == vector3.end());
    CHECK(result.second == vector2.end());
  }
}

TEST_CASE("Empty array operations", "[array]")
{
  auto empty = jules::vector<>();

  CHECK(empty.size() == 0u);
  CHECK(empty.begin() == empty.end());

  CHECK(sum(empty) == 0.0);
  CHECK(prod(empty) == 1.0);
  CHECK(max(empty) == -std::numeric_limits<jules::numeric>::infinity());
}
#endif
