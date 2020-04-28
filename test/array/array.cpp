#include "jules/array/array.hpp"

#include <catch.hpp>

TEST_CASE("Basic array functionalities", "[array]")
{
  using namespace jules;
  using namespace jules::ranges;

  array<int, 2> matrix1(4u, 5u); // matrix 4x5
  CHECK(row_count(matrix1) == 4u);
  CHECK(column_count(matrix1) == 5u);
  CHECK(matrix1.size() == 4 * 5);
  CHECK(all(matrix1 == int{}));

  array<int, 2> matrix2(3, 4u, 5u); // matrix 4x5 with values 3
  CHECK(row_count(matrix2) == 4);
  CHECK(column_count(matrix2) == 5);
  CHECK(matrix2.size() == 4 * 5);

  CHECK(all(matrix2 == 3));

  int x[20];
  copy(indices(0, 20), x);
  array<int, 2> matrix3(x, 4u, 5u); // matrix 4x5 with values 0..19

  CHECK(all(flatten(matrix3) == as_vector(indices(0, 20))));

  CHECK(all(reshape(matrix3, matrix3.size()) == as_vector(indices(0, 20))));
  CHECK(all(reshape_to<1>(matrix3, {{matrix3.size()}}) == as_vector(indices(0, 20))));
  CHECK(all(reshape_as(matrix3, flatten(matrix3)) == as_vector(indices(0, 20))));

  CHECK(all(matrix3 == reshape_to(as_vector(indices(0, 20)), matrix3.dimensions())));
  CHECK(all(matrix3 == reshape(as_vector(indices(0, 20)), row_count(matrix3), column_count(matrix3))));
  CHECK(all(matrix3 == reshape_as(as_vector(indices(0, 20)), matrix3)));

  {
    auto result = std::mismatch(begin(x), end(x), matrix3.data());
    CHECK(result.first == end(x));
    CHECK(result.second == matrix3.data() + matrix3.size());
  }

  // array<int, 2> matrix10(3, 4u, 5u, 4u); // shouldn't not compile

  array vector1 = matrix3[3]; // copy last line
  CHECK(all(vector1 == matrix3[3]));

  matrix3[3] = 0;
  x[3] = x[7] = x[11] = x[15] = x[19] = 0;
  {
    auto result = std::mismatch(begin(x), end(x), matrix3.data());
    CHECK(result.first == end(x));
    CHECK(result.second == matrix3.end());
  }

  REQUIRE(matrix1.dimensions() == matrix2.dimensions());
  auto expr0 = binary_expr_array(
    matrix1.begin(), matrix1.end(), matrix2.begin(), matrix2.end(), [](const int& a, const int& b) { return a + b; },
    matrix1.dimensions());

  static_assert(expression_array<decltype(expr0)>);

  auto expr1 = apply(matrix1, matrix2, [](int a, int b) { return a + b; });
  auto expr2 = matrix1 + matrix2;

  array from_expr = expr2;
  CHECK(all(from_expr == expr2));

  CHECK(expr1.dimensions() == expr2.dimensions());
  {
    auto result = std::mismatch(expr1.begin(), expr1.end(), expr2.begin());
    CHECK(result.first == expr1.end());
    CHECK(result.second == expr2.end());
  }

  auto expr3 = apply(matrix1, [](int a) { return -a; });
  auto expr4 = -matrix1;

  CHECK(row_count(expr3) == row_count(matrix1));
  CHECK(column_count(expr3) == column_count(matrix1));

  CHECK(expr3.dimensions() == expr4.dimensions());
  {
    auto result = std::mismatch(expr3.begin(), expr3.end(), expr4.begin());
    CHECK(result.first == expr3.end());
    CHECK(result.second == expr4.end());
  }

  auto vector2 = eval(matrix1[0][every] + matrix2[1][every]);
  static_assert(std::is_same<decltype(vector2), vector<int>>::value);

  auto expr5 = -vector2;
  CHECK(length(expr5) == length(vector2));

  static_assert(expression_array<decltype(expr5)>);
}

TEST_CASE("Type inference for as_vector", "[array]")
{
  using namespace jules;

  auto array = std::array<integer, 5>{{1, 2, 3, 4, 5}};
  auto vector1 = to_vector<integer>(array);
  auto vector2 = as_vector(array);
  auto vector3 = cat(integer{1}, 2, 3, 4, 5);

  static_assert(std::is_same<decltype(vector1), decltype(vector2)>::value);
  static_assert(std::is_same<decltype(vector2), decltype(vector3)>::value);

  CHECK(length(vector1) == length(vector2));
  {
    auto result = std::mismatch(vector1.begin(), vector1.end(), vector2.begin());
    CHECK(result.first == vector1.end());
    CHECK(result.second == vector2.end());
  }

  CHECK(length(vector3) == length(vector2));
  {
    auto result = std::mismatch(vector3.begin(), vector3.end(), vector2.begin());
    CHECK(result.first == vector3.end());
    CHECK(result.second == vector2.end());
  }
}

TEST_CASE("Empty array operations", "[array]")
{
  using namespace jules;

  auto empty = vector<>();

  CHECK(empty.size() == 0u);
  CHECK(empty.begin() == empty.end());

  CHECK(sum(empty) == numeric_traits<numeric>::additive_identity());
  CHECK(prod(empty) == numeric_traits<numeric>::multiplicative_identity());
  CHECK(max(empty) == numeric_traits<numeric>::unbounded_min());
}

TEST_CASE("Safe throwing constructor", "[array]")
{
  struct Foo
  {
    Foo(bool t = true)
    {
      if (t)
        throw t;
    }
    Foo(const Foo&) { throw true; }
    Foo(Foo&&) = default;
  };

  using namespace jules;

  auto foos = std::vector<Foo>();
  for (const auto i : indices(10))
    foos.emplace_back(false);

  auto foo = Foo{false};

  CHECK_NOTHROW(vector<Foo>{});
  CHECK_THROWS(vector<Foo>(10u));
  CHECK_THROWS(vector<Foo>(foo, 10u));
  CHECK_THROWS(vector<Foo>(foos.data(), foos.size()));
  CHECK_THROWS(vector<Foo>(foos.begin(), foos.end()));
  CHECK_THROWS(vector<Foo>(foos));
}
