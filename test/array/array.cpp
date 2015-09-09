#include <catch.hpp>

#include "array/array.hpp"

TEST_CASE("array", "[array]")
{
    using jules::scalar;
    using jules::vector;
    using jules::matrix;
    using jules::ndarray;

    scalar<double> x = 10;
    CHECK(10.0 == x);

    std::array<double, 4> values{{1.0, 2.0, 3.0, 4.0}};

    vector<double> y;          // default empty
    vector<double> z(20);      // 20 default elements
    vector<double> a(1.0, 30); // 30 elements with value 1.0
    vector<double> b(values.begin(), values.size());
    vector<double> c(values);

    // TODO: initializer list
}
