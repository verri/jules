#include "jules/base/const_vector.hpp"
#include "jules/array/array.hpp"

#include <catch.hpp>

TEST_CASE("Empty constant vector", "[const_vector]")
{
  auto vec = jules::const_vector<int>();

  CHECK(vec.size() == 0u);
  CHECK_THROWS(vec.at(0u));
}

TEST_CASE("Constructing constant vectors", "[const_vector]")
{
  using jules::as_vector;
  using jules::index_t;

  auto vec = std::vector<index_t>(5ul);

  // Initializer list
  auto a = jules::const_vector<index_t>{1u, 2u, 3u, 4u, 5u};

  // From a vector
  auto b = jules::const_vector<index_t>(vec);

  // With one argument (the size)
  auto c = jules::const_vector<index_t>(5ul);

  // With more than one argument
  auto d = jules::const_vector<index_t>(5ul, 6);

  CHECK(a.size() == 5u);
  CHECK(b.size() == 5u);
  CHECK(c.size() == 5u);
  CHECK(d.size() == 5u);

  CHECK(a == jules::seq(1u, 5u));
  CHECK(all(as_vector(b) == as_vector(0u, 0u, 0u, 0u, 0u)));
  CHECK(all(as_vector(c) == 0u));
  CHECK(all(as_vector(d) == 6u));
}

TEST_CASE("Assigning constant vectors", "[const_vector]")
{
  using jules::const_vector;
  using jules::index_t;
  using jules::seq;

  auto a = seq(1u, 5u);
  auto b = const_vector<index_t>();
  auto c = const_vector<index_t>();

  b = a;
  c = [d = a]()->const_vector<index_t> { return std::move(d); }
  ();
  a = std::vector<index_t>{1u, 2u, 3u, 4u, 5u};

  CHECK(a == seq(1u, 5u));
  CHECK(a == b);
  CHECK(a == c);
}
