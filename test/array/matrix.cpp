#include "jules/array/all.hpp"

#include <catch.hpp>

TEST_CASE("Matrix tutorial", "[array]")
{
  SECTION("Constructors")
  {
    // Default constructor.
    auto empty = jules::matrix<>();
    CHECK(empty.size() == 0u);
    CHECK(empty.data() == nullptr);
    CHECK(empty.begin() == empty.end());
    CHECK(empty.cbegin() == empty.cend());

    // Constructors with size and optional default value.
    auto all_same1 = jules::matrix<long>(3l, 4u, 4u);
    auto all_same2 = jules::matrix<long>(4u, 4u);
    all_same2() = 3l;

    CHECK(all(all_same1 == 3l));
    CHECK(all(all_same2 == 3l));
    CHECK(all(all_same1 == all_same2));

    auto values = std::array<long, 16>{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};

    auto a = jules::matrix<long>(4u, 4u);
    auto b = jules::matrix<long>(4u, 4u);

    jules::range::copy(values, a.begin());
    b() = a;

    // Constructors from iterators.
    auto c = jules::matrix<long>(std::begin(values), 4u, 4u);

    // Copy and move constructors.
    auto d = [c]() -> jules::matrix<long> { return {std::move(c)}; }();

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

    // Constructors from slicing.
    auto even_ix = jules::slice(0u, jules::slice::all, 2u);
    auto odd_ix = jules::seq(1u, 4u - 1u, 2u);
    auto all_ix = jules::slice();

    auto even_rows = jules::matrix<long>(x(even_ix, all_ix));
    auto even_columns = jules::matrix<long>(x(all_ix, even_ix));

    auto odd_rows = jules::matrix<long>(x(odd_ix, all_ix));
    auto odd_columns = jules::matrix<long>(x(all_ix, odd_ix));

    auto only_even = jules::matrix<long>(x(even_ix, even_ix));
    auto only_odd = jules::matrix<long>(x(odd_ix, odd_ix));

    auto first_row_even_columns = jules::matrix<long>(x(0u, even_ix));
    auto first_row_odd_columns = jules::matrix<long>(x(0u, odd_ix));

    auto last_row_even_columns = jules::matrix<long>(x(x.row_count() - 1, even_ix));
    auto last_row_odd_columns = jules::matrix<long>(x(x.row_count() - 1, odd_ix));

    auto first_column_even_rows = jules::matrix<long>(x(even_ix, 0u));
    auto first_column_odd_rows = jules::matrix<long>(x(odd_ix, 0u));

    auto last_column_even_rows = jules::matrix<long>(x(even_ix, x.column_count() - 1));
    auto last_column_odd_rows = jules::matrix<long>(x(odd_ix, x.column_count() - 1));

    auto second_row_even_columns = jules::matrix<long>(x(1u, even_ix));
    auto second_row_odd_columns = jules::matrix<long>(x(1u, odd_ix));

    auto second_column_even_rows = jules::matrix<long>(x(even_ix, 1u));
    auto second_column_odd_rows = jules::matrix<long>(x(odd_ix, 1u));

    CHECK(all(even_rows == jules::matrix<long>{{0, 4, 8, 12}, {2, 6, 10, 14}}));
    CHECK(all(even_columns == jules::matrix<long>{{0, 8}, {1, 9}, {2, 10}, {3, 11}}));

    CHECK(all(odd_rows == jules::matrix<long>{{1, 5, 9, 13}, {3, 7, 11, 15}}));
    CHECK(all(odd_columns == jules::matrix<long>{{4, 12}, {5, 13}, {6, 14}, {7, 15}}));

    CHECK(all(only_even == jules::matrix<long>{{0, 8}, {2, 10}}));
    CHECK(all(only_odd == jules::matrix<long>{{5, 13}, {7, 15}}));

    CHECK(all(first_row_even_columns == jules::matrix<long>{{0, 8}}));
    CHECK(all(first_row_odd_columns == jules::matrix<long>{{4, 12}}));

    CHECK(all(last_row_even_columns == jules::matrix<long>{{3, 11}}));
    CHECK(all(last_row_odd_columns == jules::matrix<long>{{7, 15}}));

    CHECK(all(first_column_even_rows == jules::matrix<long>{{0}, {2}}));
    CHECK(all(first_column_odd_rows == jules::matrix<long>{{1}, {3}}));

    CHECK(all(last_column_even_rows == jules::matrix<long>{{12}, {14}}));
    CHECK(all(last_column_odd_rows == jules::matrix<long>{{13}, {15}}));

    CHECK(all(second_row_even_columns == jules::matrix<long>{{1, 9}}));
    CHECK(all(second_row_odd_columns == jules::matrix<long>{{5, 13}}));

    CHECK(all(second_column_even_rows == jules::matrix<long>{{4}, {6}}));
    CHECK(all(second_column_odd_rows == jules::matrix<long>{{5}, {7}}));

    // Constructors from expression.
    const auto r_inv = jules::seq(x.row_count() - 1u, 0u, -1);
    const auto c_inv = jules::seq(x.column_count() - 1u, 0u, -1);

    auto y = jules::matrix<long>(x + x(r_inv, c_inv));
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

    auto y = jules::matrix<long>(-20l, 20u, 10u);
    auto z = jules::matrix<long>(-10l, 5u, 12u);

    y = x;
    CHECK(all(x == y));

    z = std::move(x);
    CHECK(all(y == z));

    x = z(jules::slice(0u, jules::slice::all, 2u), jules::slice(0u, jules::slice::all, 2u));
    CHECK(all(x == jules::matrix<long>{{0, 8}, {2, 10}}));

    x = z(jules::seq(0u, z.row_count() - 1u, 2u), jules::seq(0u, z.column_count() - 1u, 2u));
    CHECK(all(x == jules::matrix<long>{{0, 8}, {2, 10}}));

    z = x + x;
    CHECK(all(z == 2 * x));

    x = eval(x + x);
    CHECK(all(x == z));

    // This one is tricky. We must guarantee that the memory of x is not freed before the
    // expression is evaluated.
    x = x - x;
    CHECK(all(x == jules::matrix<long>(0, x.row_count(), x.column_count())));
  }

  SECTION("Iterators")
  {
    auto x = jules::matrix<long>(5u, 5u);
    const auto y = jules::matrix<long>(17l, 5u, 5u);

    for (auto& value : x)
      value = 42l;
    CHECK(all(x == 42l));

    auto it = x.begin();
    for (auto value : y)
      *it++ = value;
    CHECK(all(x == 17l));

    auto x_it = x.cbegin();
    auto y_it = y.cbegin();
    while (x_it != x.cend() && y_it != y.cend())
      CHECK(*x_it++ == *y_it++);
  }

  SECTION("Number of rows and columns, size, and extents")
  {
    auto x = jules::matrix<>(5u, 20u);
    CHECK(x.row_count() == 5u);
    CHECK(x.column_count() == 20u);
    CHECK(x.size() == 100u);
    CHECK(jules::prod(x.extents()) == x.size());
    CHECK(x.extents()[0] == 5u);
    CHECK(x.extents()[1] == 20u);

    // TODO: with C++17
    // auto [nrow, ncol] = x.dimensions();
    // CHECK(nrow == 10u);
    // CHECK(ncol == 10u);
  }

  SECTION("Submatrix assignment")
  {
    auto x = jules::matrix<long>(jules::seq(0u, 6u - 1u).begin(), 2u, 3u);
    const auto y = x;

    REQUIRE(all(x == jules::matrix<long>{{0, 2, 4}, {1, 3, 5}}));
    REQUIRE(all(y == x));

    auto all_even = jules::slice(0u, jules::slice::all, 2u);

    x(all_even, all_even) = -1;
    CHECK(all(x == jules::matrix<long>{{-1, 2, -1}, {1, 3, 5}}));

    x(all_even, all_even) = jules::matrix<long>{{-2, -3}};
    CHECK(all(x == jules::matrix<long>{{-2, 2, -3}, {1, 3, 5}}));

    x(all_even, all_even) = y(all_even, all_even);
    CHECK(all(x == y));

    x() = 0;
    CHECK(all(x == 0));

    for (auto i : jules::slice(0u, x.row_count()))
      for (auto j : jules::slice(0u, x.column_count()))
        x(i, j) = y(y.row_count() - 1, y.column_count() - 1);
    CHECK(all(x == 5));

    x(jules::seq(0u, x.row_count() - 1u), jules::seq(0u, x.column_count() - 1u)) = y();
    CHECK(all(x == y));

    auto ix = jules::as_vector(jules::seq(0u, x.row_count() - 1u, 2u));
    auto jx = jules::as_vector(jules::seq(0u, x.column_count() - 1u, 2u));

    x(ix, jx) = y(ix + 1, jx);
    CHECK(all(x == jules::matrix<long>{{1, 2, 5}, {1, 3, 5}}));

    x(jules::seq(0u, 2u - 1u), 1u) = jules::matrix<long>{{4, 4}};
    CHECK(all(x == jules::matrix<long>{{1, 4, 5}, {1, 4, 5}}));

    x[0u] = jules::as_vector(jules::repeat<3>(0u));
    x[1u] = -y[1u];
    CHECK(all(x == jules::matrix<long>{{0, 0, 0}, {-1, -3, -5}}));
  }
}
