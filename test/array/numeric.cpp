#include "jules/array/array.hpp"

#include <catch.hpp>

TEST_CASE("Dropping dimensions", "[array]")
{
  using namespace jules::slicing;
  using jules::drop;

  auto m = jules::matrix<>{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  static_assert(decltype(m)::order == 2u);

  auto v = m[every][0u];
  CHECK(all(drop<1>(v) == jules::cat(1.0, 4.0)));

  auto w = m[0u][every];
  CHECK(all(drop<1>(w) == jules::cat(1.0, 2.0, 3.0)));

  CHECK_THROWS(drop<1>(m));
  CHECK_THROWS(drop<1>(m[{0, 2}][{0, 2}]));
}
