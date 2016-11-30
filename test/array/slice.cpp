#include "jules/array/slice.hpp"

#include <algorithm>

#include <catch.hpp>

TEST_CASE("unidimensional steps", "[base_slice]")
{
  using jules::base_slice;

  auto s1 = base_slice<1>(0u, 5u, 2u);
  // auto a1 = {0ul, 2ul, 4ul, 6ul, 8ul};
  // auto r1 = std::mismatch(std::begin(s1), std::end(s1), std::begin(a1), std::end(a1));

  // CHECK(r1.first == std::end(s1));
  // CHECK(r1.second == std::end(a1));
}

TEST_CASE("check column-wise storage", "[base_slice]")
{
  using jules::base_slice;

  auto s1 = base_slice<2>(0u, {{3u, 2u}});

  // CHECK(s1(0u, 0u) == 0ul);
  // CHECK(s1(1u, 0u) == 1ul);
  // CHECK(s1(2u, 0u) == 2ul);
  // CHECK(s1(0u, 1u) == 3ul);
  // CHECK(s1(1u, 1u) == 4ul);
  // CHECK(s1(2u, 1u) == 5ul);
}
