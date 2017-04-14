#include "jules/base/const_vector.hpp"
#include "jules/array/all.hpp"

#include <catch.hpp>

TEST_CASE("Constructing constant vectors", "[const_vector]")
{
  using jules::as_vector;

  auto vec = std::vector<int>(5ul);

  // Initializer list
  auto a = jules::const_vector<int>{1, 2, 3, 4, 5};

  // From a vector
  auto b = jules::const_vector<int>(vec);

  // With one argument (the size)
  auto c = jules::const_vector<int>(5ul);

  // With more than one argument
  auto d = jules::const_vector<int>(5ul, 6);

  CHECK(a.size() == 5u);
  CHECK(b.size() == 5u);
  CHECK(c.size() == 5u);
  CHECK(d.size() == 5u);

  CHECK(all(as_vector(a) == as_vector(1, 2, 3, 4, 5)));
  CHECK(all(as_vector(b) == as_vector(0, 0, 0, 0, 0)));
  CHECK(all(as_vector(c) == as_vector(0, 0, 0, 0, 0)));
  CHECK(all(as_vector(d) == as_vector(6, 6, 6, 6, 6)));
}
