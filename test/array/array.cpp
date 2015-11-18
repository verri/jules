#include <catch.hpp>

#include "jules/array/array.hpp"

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
    vector<double> c = jules::as_vector(values);

    matrix<double> M(1.0, 10, 10);
    CHECK(M[0][0] == 1.0);
    M[0][0] = 2.0;
    CHECK(M[0][0] == 2.0);

    // usage:
    // x(1, 2, 3); // element access (no copy)
    // x[1][2][3]; // element access (no copy)

    // y(slice(0, 3), 1, slice(...));          // slicing access (no copy unless explicit indicated)
    // y({true, false, true}, 1, slice(0, 3)); // mask->indirect access (will copy if operated)
    // y({0, 2, 4}, 1, 2);                     // indirect access (will copy if operated)

    // TODO: initializer list
}
