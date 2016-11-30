#include "jules/array/array.hpp"

#include <algorithm>

#include <catch.hpp>

TEST_CASE("unidimensional", "[base_slice]")
{
  using jules::detail::base_slice;

  auto s1 = base_slice<1>(0, 5, 2);
  auto a1 = {0ul, 2ul, 4ul, 6ul, 8ul};
  auto r1 = std::mismatch(std::begin(s1), std::end(s1), std::begin(a1), std::end(a1));

  CHECK(r1.first == std::end(s1));
  CHECK(r1.second == std::end(a1));
}

TEST_CASE("colwise storage", "[base_slice]")
{
  using jules::detail::base_slice;

  auto s1 = base_slice<2>(0, {3, 2});

  CHECK(s1(0u, 0u) == 0ul);
  CHECK(s1(1u, 0u) == 1ul);
  CHECK(s1(2u, 0u) == 2ul);
  CHECK(s1(0u, 1u) == 3ul);
  CHECK(s1(1u, 1u) == 4ul);
  CHECK(s1(2u, 1u) == 5ul);
}
