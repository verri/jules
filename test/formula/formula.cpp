#define CATCH_CONFIG_MAIN

#include "jules/formula/formula.hpp"

#include <catch.hpp>

TEST_CASE("Formula", "[formula]")
{
  using jules::dataframe;
  using jules::expr;

  dataframe test{{"x", {1, 2, 3}}, {"y", {3, 4, 0}}, {"z", {5, 8, 6}}};

  auto f1 = expr<double>("z") == 2 * expr<double>("x") + expr<double>("y");
}
