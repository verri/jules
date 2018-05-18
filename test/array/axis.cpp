#include "jules/array/axis.hpp"
#include "jules/array/array.hpp"
#include <catch.hpp>

TEST_CASE("Basic axis functionalities", "[array]")
{
  using namespace jules;

  matrix<> m(indices(3 * 4).begin(), 3u, 4u);

  CHECK(m.row_count() == 3u);
  CHECK(m.column_count() == 4u);

  static_assert(std::is_same_v<decltype(drop_to<decltype(m)::order - 1>(m[0][every])), decltype(axis_at<0>(ref(m), 0))>);

  static_assert(std::is_same_v<decltype(drop_to<decltype(m)::order - 1>(m[every][0])), decltype(axis_at<1>(ref(m), 0))>);

  CHECK(all(drop_to<decltype(m)::order - 1>(m[0][every]) == axis_at<0>(ref(m), 0)));
  CHECK(all(drop_to<decltype(m)::order - 1>(m[every][0]) == axis_at<1>(ref(m), 0)));

  vector<> v(indices(3 * 4));
  CHECK(v.size() == 3 * 4);

  static_assert(std::is_same_v<decltype(v[0]), decltype(axis_at<0>(ref(v), 0))>);
  CHECK(v[0] == axis_at<0>(ref(v), 0));
}
