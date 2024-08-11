#include "jules/array/mapped_ref_array.hpp"
#include "jules/array/meta/common.hpp"
#include "jules/array/meta/reference.hpp"

#include <catch2/catch_test_macros.hpp>

#include <vector>

TEST_CASE("1-D strided reference array view", "[array]")
{
  using jules::strided_mapper;

  constexpr auto size = jules::index_t{20u};

  auto x = std::vector<double>(size, 0.0);
  REQUIRE(x.size() == size);

  auto v = jules::mapped_ref_array(x.data(), strided_mapper<1>({{x.size()}}));

  static_assert(jules::common_array<decltype(v)>);
  static_assert(jules::reference_array<decltype(v)>);

  CHECK(v.size() == x.size());
  CHECK(v.dimensions()[0] == x.size());
  CHECK((v.dimensions() == std::array<jules::index_t, 1ul>{{x.size()}}));

  v[2u] = 1.0;
  CHECK(x[2u] == 1.0);

  auto w = jules::mapped_ref_array<const double, strided_mapper<1>>(x.data(), {{{{x.size() / 2u}}, {{2u}}}});

  static_assert(jules::common_array<decltype(w)>);
  static_assert(jules::reference_array<decltype(w)>);

  CHECK(w.size() == x.size() / 2u);
  CHECK(w.dimensions()[0] == x.size() / 2u);
  CHECK((w.dimensions() == std::array<jules::index_t, 1ul>{{x.size() / 2u}}));

  CHECK(w[1u] == 1.0);

  auto y = std::vector<double>(size, 1.0);
  REQUIRE(x.size() == y.size());

  auto a = jules::mapped_ref_array(x.data(), strided_mapper<1>({{x.size()}}));
  auto b = jules::mapped_ref_array(y.data(), strided_mapper<1>({{y.size()}}));

  a = b;
  CHECK(x == y);

  a = 2.0;
  CHECK(std::find_if(x.begin(), x.end(), [](auto value) { return value != 2.0; }) == x.end());

  auto c = jules::mapped_ref_array(x.data(), strided_mapper<2>({{x.size() / 2, 2u}}));

  c[0][0] = 3.0;
  CHECK(x[0] == 3.0);

  c[0][1u] = 4.0;
  CHECK(x[c.dimensions()[0]] == 4.0);

  a[{0u, a.size()}] = 0.0;
  CHECK(std::find_if(x.begin(), x.end(), [](auto value) { return value != 0.0; }) == x.end());

  CHECK_THROWS((a[{0u, a.size(), 2u}]));

  a[{1u, 2u, 2u}] = 1.0;
  CHECK(x[0] == 0.0);
  CHECK(x[1] == 1.0);
  CHECK(x[2] == 0.0);
  CHECK(x[3] == 1.0);

  c[0u][{0u, c.dimensions()[1]}] = 2.0;
  CHECK((x[0] == 2.0 && x[c.dimensions()[0]] == 2.0));

  const auto ix = std::vector<jules::index_t>{0u, 7u, 9u};
  a[ix] = 3.0;
  CHECK((x[0] == 3.0 && x[7] == 3.0 && x[9] == 3.0));
}
