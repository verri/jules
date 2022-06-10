#include "jules/array/array.hpp"

#include <catch2/catch_test_macros.hpp>

static void (*volatile not_optimize_away)(void*) = [](void*) {};

TEST_CASE("Matrix tutorial", "[array]")
{
  using namespace jules::slicing;

  SECTION("Constructors")
  {
    // Non-initialized
    auto fast = jules::matrix<>(jules::uninitialized, 2u, 5u);
    CHECK(row_count(fast) == 2u);
    CHECK(column_count(fast) == 5u);

    // Default constructor.
    auto empty = jules::matrix<>();
    CHECK(empty.size() == 0u);
    CHECK(empty.data() == nullptr);
    CHECK(empty.begin() == empty.end());
    CHECK(empty.cbegin() == empty.cend());

    // Uninitialized vs initialized
    auto uninit = jules::matrix<>(jules::uninitialized, 3u, 3u);
    auto init = jules::matrix<>(3u, 3u);

    CHECK(all(init == jules::numeric{}));

    // Constructors with size and optional default value.
    auto all_same1 = jules::matrix<long>(3L, 4u, 4u);
    auto all_same2 = jules::matrix<long>(4u, 4u);
    all_same2[every][every] = 3L; // very fast!

    CHECK(all(all_same1 == 3L));
    CHECK(all(all_same2 == 3L));
    CHECK(all(all_same1 == all_same2));

    auto values = std::array<long, 16>{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};

    auto a = jules::matrix<long>(4u, 4u);
    auto b = jules::matrix<long>(4u, 4u);

    jules::ranges::copy(values, a.begin());
    b[every][every] = a;

    // Constructors from iterators.
    auto c = jules::matrix<long>(std::begin(values), 4u, 4u);

    // Copy and move constructors.
    auto d = [c]() mutable -> jules::matrix<long> {
      not_optimize_away(&c);
      return {std::move(c)};
    }();

    REQUIRE(all(a == b));
    REQUIRE(all(b == c));
    REQUIRE(all(c == d));

    // Constructor from recursive initializer list.
    auto x = jules::matrix<long>{
      {0, 4, 8, 12},  //
      {1, 5, 9, 13},  //
      {2, 6, 10, 14}, //
      {3, 7, 11, 15}, //
    };

    CHECK(all(x == a));

    // Invalid initializer list
    CHECK_THROWS((jules::matrix<int>{{1, 2}, {3, 4}, {1, 2, 3}}));

    // Constructors from slicing.
    auto even_ix = jules::slice(0u, every, 2u);
    auto odd_ix = jules::seq(1u, 4u - 1u, 2u);
    auto all_ix = jules::slice();

    auto even_rows = jules::array(x[even_ix][all_ix]);
    auto even_columns = jules::array(x[all_ix][even_ix]);

    auto odd_rows = jules::array(x[odd_ix][all_ix]);
    auto odd_columns = jules::array(x[all_ix][odd_ix]);

    auto only_even = jules::array(x[even_ix][even_ix]);
    auto only_odd = jules::array(x[odd_ix][odd_ix]);

    auto first_row_even_columns = jules::array(x[{0u, 1u, 1u}][even_ix]);
    auto first_row_odd_columns = jules::array(x[{0u, 1u}][odd_ix]);

    auto last_row_even_columns = jules::array(x[row_count(x) - 1][even_ix]);
    auto last_row_odd_columns = jules::array(x[row_count(x) - 1][odd_ix]);

    auto first_column_even_rows = jules::array(x[even_ix][{0u, 1u}]);
    auto first_column_odd_rows = jules::array(x[odd_ix][{0u, 1u}]);

    auto last_column_even_rows = jules::array(x[even_ix][{column_count(x) - 1, 1u}]);
    auto last_column_odd_rows = jules::array(x[odd_ix][{column_count(x) - 1, 1u}]);

    auto second_row_even_columns = jules::array(x[{1u, 1u}][even_ix]);
    auto second_row_odd_columns = jules::array(x[{1u, 1u}][odd_ix]);

    auto second_column_even_rows = jules::array(x[even_ix][{1u, 1u}]);
    auto second_column_odd_rows = jules::array(x[odd_ix][{1u, 1u}]);

    CHECK(all(even_rows == jules::matrix<long>{{0, 4, 8, 12}, {2, 6, 10, 14}}));
    CHECK(all(even_columns == jules::matrix<long>{{0, 8}, {1, 9}, {2, 10}, {3, 11}}));

    CHECK(all(odd_rows == jules::matrix<long>{{1, 5, 9, 13}, {3, 7, 11, 15}}));
    CHECK(all(odd_columns == jules::matrix<long>{{4, 12}, {5, 13}, {6, 14}, {7, 15}}));

    CHECK(all(only_even == jules::matrix<long>{{0, 8}, {2, 10}}));
    CHECK(all(only_odd == jules::matrix<long>{{5, 13}, {7, 15}}));

    CHECK(all(first_row_even_columns == jules::matrix<long>{{0, 8}}));
    CHECK(all(first_row_odd_columns == jules::matrix<long>{{4, 12}}));

    CHECK(all(last_row_even_columns == jules::vector<long>{3, 11}));
    CHECK(all(last_row_odd_columns == jules::vector<long>{7, 15}));

    CHECK(all(first_column_even_rows == jules::matrix<long>{{0}, {2}}));
    CHECK(all(first_column_odd_rows == jules::matrix<long>{{1}, {3}}));

    CHECK(all(last_column_even_rows == jules::matrix<long>{{12}, {14}}));
    CHECK(all(last_column_odd_rows == jules::matrix<long>{{13}, {15}}));

    CHECK(all(second_row_even_columns == jules::matrix<long>{{1, 9}}));
    CHECK(all(second_row_odd_columns == jules::matrix<long>{{5, 13}}));

    CHECK(all(second_column_even_rows == jules::matrix<long>{{4}, {6}}));
    CHECK(all(second_column_odd_rows == jules::matrix<long>{{5}, {7}}));

    // Constructors from expression.
    const auto r_inv = jules::seq(row_count(x) - 1u, 0u, -1);
    const auto c_inv = jules::seq(column_count(x) - 1u, 0u, -1);

    auto y = jules::array(x + x[r_inv][c_inv]);
    CHECK(all(y == 15));
  }

  SECTION("Assignments")
  {
    auto x = jules::matrix<long>{
      {0, 4, 8, 12},  //
      {1, 5, 9, 13},  //
      {2, 6, 10, 14}, //
      {3, 7, 11, 15}, //
    };

    auto y = jules::matrix<long>(-20L, 20u, 10u);
    auto z = jules::matrix<long>(-10L, 5u, 12u);

    y = x;
    CHECK(all(x == y));

    z = std::move(x);
    CHECK(all(y == z));

    x = z[jules::slice(0u, every, 2u)][jules::slice(0u, every, 2u)];
    CHECK(all(x == jules::matrix<long>{{0, 8}, {2, 10}}));

    x = z[jules::seq(0u, row_count(z) - 1u, 2u)][jules::seq(0u, column_count(z) - 1u, 2u)];
    CHECK(all(x == jules::matrix<long>{{0, 8}, {2, 10}}));

    z = x + x;
    CHECK(all(z == 2 * x));

    x = eval(x + x);
    CHECK(all(x == z));

    // This one is tricky. We must guarantee that the memory of x is not freed before the
    // expression is evaluated.
    x = x - x;
    CHECK(all(x == jules::matrix<long>(0, row_count(x), column_count(x))));
  }

  SECTION("Iterators")
  {
    auto x = jules::matrix<long>(5u, 5u);
    const auto y = jules::matrix<long>(17L, 5u, 5u);

    for (auto& value : x)
      value = 42L;
    CHECK(all(x == 42L));

    auto it = x.begin();
    for (auto value : y)
      *it++ = value;
    CHECK(all(x == 17L));

    auto x_it = x.cbegin();
    auto y_it = y.cbegin();
    while (x_it != x.cend() && y_it != y.cend())
      CHECK(*x_it++ == *y_it++);
  }

  SECTION("Number of rows and columns, size, and extents")
  {
    auto x = jules::matrix<>(5u, 20u);
    CHECK(row_count(x) == 5u);
    CHECK(column_count(x) == 20u);
    CHECK(x.size() == 100u);
    CHECK(jules::prod(x.dimensions()) == x.size());
    CHECK(x.dimensions()[0] == 5u);
    CHECK(x.dimensions()[1] == 20u);

    auto [nrow, ncol] = x.dimensions();
    CHECK(nrow == 5u);
    CHECK(ncol == 20u);
  }

  SECTION("Submatrix assignment")
  {
    auto x = jules::matrix<long>(jules::seq(0u, 6u - 1u).begin(), 2u, 3u);
    const auto y = x;

    REQUIRE(all(x == jules::matrix<long>{{0, 2, 4}, {1, 3, 5}}));
    REQUIRE(all(y == x));

    auto all_even = jules::slice(0u, every, 2u);

    x[all_even][all_even] = -1;
    CHECK(all(x == jules::matrix<long>{{-1, 2, -1}, {1, 3, 5}}));

    x[all_even][all_even] = jules::matrix<long>{{-2, -3}};
    CHECK(all(x == jules::matrix<long>{{-2, 2, -3}, {1, 3, 5}}));

    x[all_even][all_even] = y[all_even][all_even];
    CHECK(all(x == y));

    x[every][every] = 0;
    CHECK(all(x == 0));

    for (const auto i : jules::indices(row_count(x)))
      for (const auto j : jules::indices(column_count(x)))
        x[i][j] = y[row_count(y) - 1][column_count(y) - 1];
    CHECK(all(x == 5));

    x[jules::seq(0u, row_count(x) - 1u)][jules::seq(0u, column_count(x) - 1u)] = y;
    CHECK(all(x == y));

    auto ix = jules::as_vector(jules::seq(0u, row_count(x) - 1u, 2u));
    auto jx = jules::as_vector(jules::seq(0u, column_count(x) - 1u, 2u));

    x[ix][jx] = y[eval(ix + 1)][jx];
    CHECK(all(x == jules::matrix<long>{{1, 2, 5}, {1, 3, 5}}));

    x[jules::seq(0u, 1u)][1u] = jules::vector<long>{4, 4};
    CHECK(all(x == jules::matrix<long>{{1, 4, 5}, {1, 4, 5}}));

    x[0u][every] = jules::as_vector(jules::repeat<3>(0u));
    x[1u][every] = -y[1u][every];

    CHECK(all(x == jules::matrix<long>{{0, 0, 0}, {-1, -3, -5}}));
  }
}
