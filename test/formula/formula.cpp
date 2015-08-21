//#define CATCH_CONFIG_MAIN

//#include "catch.hpp"

#include "formula/formula.hpp"

#include <algorithm>

// TEST_CASE("Formula", "[formula]")
int main()
{
    using jules::dataframe;
    using jules::formula;
    using jules::term;
    using jules::remaining;

    dataframe test{{"x", {1, 2, 3}}, {"y", {3, 4, 0}}, {"z", {5, 8, 6}}};

    // z = (2 * x, y)
    formula f1{term<>("z"), (2 * term<double>("x"), term<>("y"))};

    // (z, x, y) = 0
    formula f2{{term<>("z"), remaining}, 0};

    // z = (z, x, y)
    formula f3{term<>("z"), remaining};

    // z = (x, y, z, y)
    formula f3{term<>("z"), (remaining, term<>("y"), remaining)};

    // z = 2 * x + y
    formula f4{term<>("z"), 2 * term<double>("x") + term<>("y")};
}
