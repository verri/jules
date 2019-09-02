#include "jules/array/array.hpp"
#include "jules/core/ranges.hpp"

#include <catch.hpp>

#include <iterator>
#include <type_traits>

TEST_CASE("Basic ranges::view usage", "[range]")
{
  using namespace jules::ranges;
  using vector = jules::vector<>;
  using jules::as_vector;
  using jules::cat;
  using jules::closed_indices;
  using jules::indices;

  static_assert(std::is_same<range_value_t<vector>, typename std::iterator_traits<vector::iterator>::value_type>::value, "");

  const auto ints_vector = as_vector(views::iota(0, unreachable) | views::take(10u));
  REQUIRE(ints_vector.size() == 10u);
  CHECK(all(ints_vector == cat(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)));

  const auto ix = as_vector(indices(0, 5));
  REQUIRE(ix.size() == 5u);
  CHECK(all(ix == cat(0, 1, 2, 3, 4)));

  const auto cix = as_vector(closed_indices(0, 5));
  REQUIRE(cix.size() == 6u);
  CHECK(all(cix == cat(0, 1, 2, 3, 4, 5)));
}
