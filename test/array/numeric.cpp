#include "jules/array/array.hpp"

#include <catch.hpp>

TEST_CASE("Dropping dimensions", "[array]")
{
  using namespace jules;

  auto m = matrix<>{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  static_assert(decltype(m)::order == 2u);

  auto v = m[every][0u];
  CHECK(all(drop(v) == cat(1.0, 4.0)));

  auto w = m[0u][every];
  CHECK(all(drop(w) == cat(1.0, 2.0, 3.0)));

  CHECK_THROWS(drop(m));
  CHECK_THROWS(drop(m[{0, 2}][{0, 2}]));

  CHECK(all(drop(m[{0, 2}][{0, 1}]) == cat(1.0, 4.0)));
  CHECK(all(drop(m[{0, 2}][{1, 1}]) == cat(2.0, 5.0)));
  CHECK(all(drop(m[{0, 1}][{0, 3}]) == cat(1.0, 2.0, 3.0)));
}
