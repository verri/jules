#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "formula/formula.hpp"

TEST_CASE("Formula", "[formula]")
{
    using jules::dataframe;
    using jules::formula;
    using jules::expr;

    dataframe test{{"x", {1, 2, 3}}, {"y", {3, 4, 0}}, {"z", {5, 8, 6}}};

    auto f1 = expr<>("z") == expr<double>("x") * 2 + expr<double>("y");
}
