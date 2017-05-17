#include "jules/array/slicing/absolute.hpp"
#include "jules/array/slicing/bounded.hpp"
// #include "jules/array/slicing/range.hpp"
// #include "jules/array/slicing/relative.hpp"

#include <catch.hpp>

using namespace jules::slicing;
using jules::tag;

namespace
{
struct Check {
  constexpr auto operator[](absolute_slice) const { return tag<absolute_slice>{}; };
  constexpr auto operator[](absolute_strided_slice) const { return tag<absolute_strided_slice>{}; }
  constexpr auto operator[](bounded_slice) const { return tag<bounded_slice>{}; };
  constexpr auto operator[](bounded_strided_slice) const { return tag<bounded_strided_slice>{}; }
};
}

TEST_CASE("Slicing functionalities", "[array]")
{
  constexpr auto c = Check{};

  static_assert(c[{0u, 1u}] == tag<absolute_slice>{});
  static_assert(c[{0u, 1u, 2u}] == tag<absolute_strided_slice>{});
  static_assert(c[{0u, at_most(10u)}] == tag<bounded_slice>{});
  static_assert(c[{0u, at_most(10u), 2u}] == tag<bounded_strided_slice>{});

  static_assert(c[slice(0u, 1u)] == tag<absolute_slice>{});
  static_assert(c[slice(0u, 1u, 2u)] == tag<absolute_strided_slice>{});
  static_assert(c[slice(0u, at_most(10u))] == tag<bounded_slice>{});
  static_assert(c[slice(0u, at_most(10u), 2u)] == tag<bounded_strided_slice>{});
}
