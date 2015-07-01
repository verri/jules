#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "formula/formula.hpp"

#include <algorithm>

TEST_CASE("Formula", "[formula]") {

    using jules::dataframe;
    using jules::formula;
    using jules::response;
    using jules::term;

    auto f = ( response<double>("z") = { 2 * term<double>("x"), term<>("y") } );

    // dataframe test{
    //     {"x", {1, 2, 3}},
    //     {"y", {3, 4, 0}},
    //     {"z", {5, 8, 6}}
    // };

    // auto z = f.response(test).template as_array<double>();
    // auto xy = f.terms(test).template as_array<double>();

    // auto z_ = rowapply(xy, [](auto&& row) { return sum(row); });

    // CHECK(all(z_ == z));
}
