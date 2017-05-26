#include "jules/array/strided_ref_array.hpp"
#include "jules/array/meta/common.hpp"
#include "jules/array/meta/reference.hpp"

#include <catch.hpp>

#include <vector>

TEST_CASE("1-D strided reference array view", "[array]")
{
  constexpr auto size = jules::index_t{20u};

  auto x = std::vector<double>(size, 0.0);
  REQUIRE(x.size() == size);

  auto v = jules::strided_ref_array<double, 1u>(x.data(), {0u, {{x.size()}}});

  static_assert(jules::CommonArray<decltype(v)>::value);
  static_assert(jules::ReferenceArray<decltype(v)>::value);

  CHECK(v.size() == x.size());
  CHECK(v.length() == x.size());
  CHECK((v.dimensions() == std::array<jules::index_t, 1ul>{{x.size()}}));

  v[2u] = 1.0;
  CHECK(x[2u] == 1.0);

  auto w = jules::strided_ref_array<const double, 1u>(x.data(), {0u, {{x.size() / 2u}}, {{2u}}});

  static_assert(jules::CommonArray<decltype(w)>::value);
  static_assert(jules::ReferenceArray<decltype(w)>::value);

  CHECK(w.size() == x.size() / 2u);
  CHECK(w.length() == x.size() / 2u);
  CHECK((w.dimensions() == std::array<jules::index_t, 1ul>{{x.size() / 2u}}));

  CHECK(w[1u] == 1.0);

  auto y = std::vector<double>(size, 1.0);
  REQUIRE(x.size() == y.size());

  auto a = jules::strided_ref_array<double, 1u>(x.data(), {0u, {{x.size()}}});
  auto b = jules::strided_ref_array<double, 1u>(y.data(), {0u, {{y.size()}}});

  a = b;
  CHECK(x == y);

  a = 2.0;
  CHECK(std::find_if(x.begin(), x.end(), [](auto value) { return value != 2.0; }) == x.end());

  auto c = jules::strided_ref_array<double, 2u>(x.data(), {0u, {{x.size() / 2, 2u}}});

  c[0][0] = 3.0;
  CHECK(x[0] == 3.0);

  c[0][1u] = 4.0;
  CHECK(x[c.row_count()] == 4.0);
}
