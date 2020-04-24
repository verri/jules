#include "jules/array/slicing/absolute.hpp"
#include "jules/array/slicing/bounded.hpp"
#include "jules/array/slicing/relative.hpp"
// #include "jules/array/slicing/range.hpp"

#include <catch.hpp>

using namespace jules::slicing;
using jules::tag;

namespace
{
struct relative
{};
struct relative_strided
{};
struct Check
{
  constexpr auto operator[](absolute_slice) const { return tag<absolute_slice>{}; };
  constexpr auto operator[](absolute_strided_slice) const { return tag<absolute_strided_slice>{}; }
  constexpr auto operator[](bounded_slice) const { return tag<bounded_slice>{}; };
  constexpr auto operator[](bounded_strided_slice) const { return tag<bounded_strided_slice>{}; }
  constexpr auto operator[](every_index) const { return tag<every_index>{}; }

  template <typename F> auto operator[](relative_slice<F>) const { return tag<relative>{}; }

  template <typename F> auto operator[](relative_strided_slice<F>) const { return tag<relative_strided>{}; }
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

  static_assert(c[slice(0u, 1u)] == tag<absolute_slice>{});
  static_assert(c[slice(0u, 1u, 2u)] == tag<absolute_strided_slice>{});
  static_assert(c[slice(0u, at_most(10u))] == tag<bounded_slice>{});
  static_assert(c[slice(0u, at_most(10u), 2u)] == tag<bounded_strided_slice>{});
  static_assert(c[slice()] == tag<every_index>{});

  CHECK((c[slice(0u, dim())] == tag<relative>{}));
  CHECK((c[slice(0u, dim() - 1u)] == tag<relative>{}));
  CHECK((c[slice(0u, dim() / 2u)] == tag<relative>{}));
  CHECK((c[slice(0u, (dim() - 1u) / 2u)] == tag<relative>{}));
  CHECK((c[slice(0u, dim(), 3u)] == tag<relative_strided>{}));
  CHECK((c[slice(0u, dim() - 1u, 3u)] == tag<relative_strided>{}));
  CHECK((c[slice(0u, dim() / 2u, 3u)] == tag<relative_strided>{}));
  CHECK((c[slice(0u, (dim() - 1u) / 2u, 3u)] == tag<relative_strided>{}));

  CHECK(absolutize(slice(0u, dim()), 2u).start() == 0u);
  CHECK(absolutize(slice(0u, dim()), 2u).extent() == 2u);
  CHECK(absolutize(slice(0u, dim() / 2u), 2u).extent() == 1u);
}
