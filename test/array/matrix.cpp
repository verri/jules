#include "jules/array/all.hpp"

#include <catch.hpp>

TEST_CASE("Matrix tutorial", "[matrix]")
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
    auto odd_ix = jules::seq(1u, 4u, 2u);
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

    // // Constructors from expression.
    // auto y = jules::matrix<long>(x + x(jules::seq(x.length(), 0u) - 1u));
    // CHECK(all(y == 9));
  }
}
