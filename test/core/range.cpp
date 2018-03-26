#include "jules/core/range.hpp"
#include "jules/array/array.hpp"

#include <catch.hpp>

#include <iterator>
#include <type_traits>

TEST_CASE("Basic range::view usage", "[range]")
{
  using namespace jules::range;
  using vector = jules::vector<>;
  using jules::as_vector;
  using jules::cat;
  using jules::indices;
  using jules::closed_indices;

  static_assert(std::is_same<range_value_type_t<vector>, typename std::iterator_traits<vector::iterator>::value_type>::value, "");

  const auto ints_vector = as_vector(view::ints(0) | view::take(10u));
  REQUIRE(ints_vector.size() == 10u);
  CHECK(all(ints_vector == cat(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)));

  const auto numbers_vector = as_vector(view::numbers(0, 2) | view::take(5u));
  REQUIRE(numbers_vector.size() == 5u);
  CHECK(all(numbers_vector == cat(0, 2, 4, 6, 8)));

  const auto ix = as_vector(indices(0, 5));
  REQUIRE(ix.size() == 5u);
  CHECK(all(ix == cat(0, 1, 2, 3, 4)));

  const auto cix = as_vector(closed_indices(0, 5));
  REQUIRE(cix.size() == 6u);
  CHECK(all(cix == cat(0, 1, 2, 3, 4, 5)));
}
