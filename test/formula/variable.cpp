#include "jules/formula/variable.hpp"

#include <catch.hpp>

#include <type_traits>

TEST_CASE("Variable literals", "[formula]")
{
  using namespace jules;
  using namespace jules::formula_literals;

  auto a1 = var<>("a");
  auto a2 = "a"_v;

  auto b1 = var<numeric>("b");
  auto b2 = "b"_nv;

  auto c1 = var<string>("c");
  auto c2 = "c"_sv;

  static_assert(std::is_same<decltype(a1), decltype(a2)>::value, "");
  static_assert(std::is_same<decltype(b1), decltype(b2)>::value, "");
  static_assert(std::is_same<decltype(c1), decltype(c2)>::value, "");
}
