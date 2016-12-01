#include "jules/array/slice.hpp"

#include <algorithm>

#include <catch.hpp>

TEST_CASE("unidimensional steps", "[base_slice]")
{
  using jules::base_slice;

  auto s1 = base_slice<1>(0ul, 5ul, 2ul);
  auto a1 = {0ul, 2ul, 4ul, 6ul, 8ul};
  auto r1 = std::mismatch(std::begin(s1), std::end(s1), std::begin(a1), std::end(a1));

  CHECK(r1.first == std::end(s1));
  CHECK(r1.second == std::end(a1));
}

TEST_CASE("check column-wise storage", "[base_slice]")
{
  using jules::base_slice;

  auto s1 = base_slice<2>(0ul, {{3ul, 2ul}});

  CHECK(static_cast<std::size_t>(s1(0ul, 0ul)) == 0ul);
  CHECK(static_cast<std::size_t>(s1(1ul, 0ul)) == 1ul);
  CHECK(static_cast<std::size_t>(s1(2ul, 0ul)) == 2ul);
  CHECK(static_cast<std::size_t>(s1(0ul, 1ul)) == 3ul);
  CHECK(static_cast<std::size_t>(s1(1ul, 1ul)) == 4ul);
  CHECK(static_cast<std::size_t>(s1(2ul, 1ul)) == 5ul);
}
