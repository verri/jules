#include "jules/core/ranges.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iterator>
#include <type_traits>

TEST_CASE("Basic ranges::view usage", "[range]")
{
  using namespace jules;

  {
    const auto ix = indices(0, 5);
    REQUIRE(ix.size() == 5);

    auto j = 0;
    for (const auto i : ix)
      CHECK(i == j++);
    CHECK(j == 5);
  }

  {
    const auto ix = closed_indices(0, 5);
    REQUIRE(ix.size() == 6);

    auto j = 0;
    for (const auto i : ix)
      CHECK(i == j++);
    CHECK(j == 6);
  }
}
