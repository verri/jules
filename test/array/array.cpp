#include "catch.hpp"

#include "array/array.hpp"

TEST_CASE("array", "[array]")
{
    using jules::scalar;
    using jules::vector;
    using jules::matrix;
    using jules::ndarray;

    scalar<double> x = 10;
    CHECK(10.0 == x);

    vector<double> y; // default empty
    vector<double> z(20); // 20 default elements
}
