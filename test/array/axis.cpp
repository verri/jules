#include "jules/array/array.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Basic axis functionalities", "[array]")
{
  using namespace jules;

  matrix<> m(indices(3 * 4).begin(), 3u, 4u);

  CHECK(row_count(m) == 3u);
  CHECK(column_count(m) == 4u);

  static_assert(std::is_same_v<decltype(drop_to<decltype(m)::order - 1>(m[0][every])), decltype(axis_at<0>(ref(m), 0))>);

  static_assert(std::is_same_v<decltype(drop_to<decltype(m)::order - 1>(m[every][0])), decltype(axis_at<1>(ref(m), 0))>);

  CHECK(all(drop_to<decltype(m)::order - 1>(m[0][every]) == axis_at<0>(ref(m), 0)));
  CHECK(all(drop_to<decltype(m)::order - 1>(m[every][0]) == axis_at<1>(ref(m), 0)));

  size_t r = 0;
  for (auto row : axis<0>(m))
    CHECK(all(row == drop(m[r++][every])));

  r = 0;
  for (auto row : rows(m))
    CHECK(all(row == drop(m[r++][every])));

  size_t c = 0;
  for (auto column : columns(m))
    CHECK(all(column == drop(m[every][c++])));

  vector<> v(indices(3 * 4));
  CHECK(v.size() == 3 * 4);

  static_assert(std::is_same_v<decltype(v[0]), decltype(axis_at<0>(ref(v), 0))>);
  CHECK(v[0] == axis_at<0>(ref(v), 0));

  static_assert(::jules::ranges::random_access_range<decltype(rows(m))>);
  static_assert(::jules::ranges::random_access_range<decltype(columns(m))>);
}
