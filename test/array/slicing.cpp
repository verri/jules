#include "jules/array/slicing/absolute.hpp"
#include "jules/array/slicing/bounded.hpp"
#include "jules/array/slicing/relative.hpp"
// #include "jules/array/slicing/range.hpp"

#include <catch.hpp>

using namespace jules::slicing;
using jules::tag;

namespace
{
struct Check
{
  constexpr auto operator[](absolute_slice) const { return tag<absolute_slice>{}; };
  constexpr auto operator[](absolute_strided_slice) const { return tag<absolute_strided_slice>{}; }
  constexpr auto operator[](bounded_slice) const { return tag<bounded_slice>{}; };
  constexpr auto operator[](bounded_strided_slice) const { return tag<bounded_strided_slice>{}; }
  constexpr auto operator[](every_index) const { return tag<every_index>{}; }

  auto operator[](relative_slice) const { return tag<relative_slice>{}; }
  auto operator[](relative_strided_slice) const { return tag<relative_strided_slice>{}; }
};
} // namespace

TEST_CASE("Slicing functionalities", "[array]")
{
  constexpr auto c = Check{};

  static_assert(c[{0u, 1u}] == tag<absolute_slice>{});
  static_assert(c[{0u, 1u, 2u}] == tag<absolute_strided_slice>{});
  static_assert(c[{0u, at_most(10u)}] == tag<bounded_slice>{});
  static_assert(c[{0u, at_most(10u), 2u}] == tag<bounded_strided_slice>{});
  static_assert(c[{0u, every, 2u}] == tag<bounded_strided_slice>{});
  static_assert(c[every] == tag<every_index>{});

  CHECK((c[{0u, size}] == tag<relative_slice>{}));
  CHECK((c[{0u, size - 1u}] == tag<relative_slice>{}));
  CHECK((c[{0u, size / 2u}] == tag<relative_slice>{}));
  CHECK((c[{0u, (size - 1u) / 2u}] == tag<relative_slice>{}));
  CHECK((c[{0u, size, 3u}] == tag<relative_strided_slice>{}));
  CHECK((c[{0u, size - 1u, 3u}] == tag<relative_strided_slice>{}));
  CHECK((c[{0u, size / 2u, 3u}] == tag<relative_strided_slice>{}));
  CHECK((c[{0u, (size - 1u) / 2u, 3u}] == tag<relative_strided_slice>{}));

  static_assert(c[slice(0u, 1u)] == tag<absolute_slice>{});
  static_assert(c[slice(0u, 1u, 2u)] == tag<absolute_strided_slice>{});
  static_assert(c[slice(0u, at_most(10u))] == tag<bounded_slice>{});
  static_assert(c[slice(0u, at_most(10u), 2u)] == tag<bounded_strided_slice>{});
  static_assert(c[slice()] == tag<every_index>{});

  CHECK((c[slice(0u, size)] == tag<relative_slice>{}));
  CHECK((c[slice(0u, size - 1u)] == tag<relative_slice>{}));
  CHECK((c[slice(0u, size / 2u)] == tag<relative_slice>{}));
  CHECK((c[slice(0u, (size - 1u) / 2u)] == tag<relative_slice>{}));
  CHECK((c[slice(0u, size, 3u)] == tag<relative_strided_slice>{}));
  CHECK((c[slice(0u, size - 1u, 3u)] == tag<relative_strided_slice>{}));
  CHECK((c[slice(0u, size / 2u, 3u)] == tag<relative_strided_slice>{}));
  CHECK((c[slice(0u, (size - 1u) / 2u, 3u)] == tag<relative_strided_slice>{}));

  CHECK(eval(slice(0u, size), 2u).start == 0u);
  CHECK(eval(slice(0u, size), 2u).extent == 2u);
  CHECK(eval(slice(0u, size / 2u), 2u).extent == 1u);
}
