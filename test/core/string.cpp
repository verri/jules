#include "jules/core/type.hpp"

#include <catch.hpp>

TEST_CASE("Basic strings", "[type]")
{
  using namespace jules;
  using namespace jules::literals;

  string a;
  CHECK(a == ""_s);

  string b = "hello"_s;
  CHECK(b == "hello"_s);

  string c = b;
  CHECK(c == b);

  string d = std::move(c);
  CHECK(d == "hello"_s);

  string e = "foo"_s;
  CHECK(e == "foo"_s);

  e = a;
  CHECK(e == ""_s);

  e = std::move(d);
  CHECK(e == "hello"_s);
}
