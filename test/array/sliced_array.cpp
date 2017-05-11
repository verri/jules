#include <jules/array/sliced_array.hpp>
#include <jules/array/meta/reference.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/meta/reference.hpp>

#include <catch.hpp>

#include <vector>

TEST_CASE("1-D sliced array view", "[array]")
{
  constexpr auto size = 20ul;

  auto x = std::vector<double>(size, 0.0);
  REQUIRE(x.size() == size);

  auto v = jules::sliced_array<double, 1u>(x.data(), {0u, x.size()});

  static_assert(jules::CommonArray<decltype(v)>::value);
  static_assert(jules::ReferenceArray<decltype(v)>::value);

  CHECK(v.size() == x.size());
  CHECK(v.length() == x.size());
  CHECK((v.dimensions() == std::array<jules::index_t, 1ul>{{x.size()}}));

  v[2u] = 1.0;
  CHECK(x[2u] == 1.0);

  auto w = jules::sliced_array<const double, 1u>(x.data(), {0u, x.size() / 2u, 2u});

  static_assert(jules::CommonArray<decltype(w)>::value);
  static_assert(jules::ReferenceArray<decltype(w)>::value);

  CHECK(w.size() == x.size() / 2u);
  CHECK(w.length() == x.size() / 2u);
  CHECK((w.dimensions() == std::array<jules::index_t, 1ul>{{x.size() / 2u}}));

  CHECK(w[1u] == 1.0);
}
