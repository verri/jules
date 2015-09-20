#include <catch.hpp>
#include "array/detail/array.hpp"
#include "array/detail/expression.hpp"

TEST_CASE("base array", "[base array]")
{
    using jules::detail::base_ndarray;
    using jules::detail::base_slice;
    using jules::detail::indirect_ndarray;
    using jules::detail::ref_ndarray;

    base_ndarray<int, 2> matrix1(4, 5); // matrix 4x5
    CHECK(matrix1.size(0) == 4);
    CHECK(matrix1.size(1) == 5);
    CHECK(matrix1.size() == 4 * 5);

    base_ndarray<int, 2> matrix2(3, 4, 5); // matrix 4x5 with values 3
    CHECK(matrix2.size(0) == 4);
    CHECK(matrix2.size(1) == 5);
    CHECK(matrix2.size() == 4 * 5);
    for (auto it = matrix2.data(); it != matrix2.data() + matrix2.size(); ++it)
        CHECK(*it == 3);

    int x[20];
    std::iota(std::begin(x), std::end(x), 0);
    base_ndarray<int, 2> matrix3(x, 4, 5); // matrix 4x5 with values 0..19

    auto result = std::mismatch(std::begin(x), std::end(x), matrix3.data());
    CHECK(result.first == std::end(x));
    CHECK(result.second == matrix3.data() + matrix3.size());

    // base_ndarray<int, 2> matrix3(3, 4, 5, 4); // do not compile

    auto&& vector1 = matrix3[0]; // first line
    vector1 = 0;

    int y[5] = {0};
    result = std::mismatch(std::begin(y), std::end(y), matrix3.data());
    CHECK(result.first == std::end(y));
    CHECK(result.second == matrix3.data() + 5);

    std::size_t i = 0;
    for (auto&& line : matrix3)
        ++i;

    CHECK(i == matrix3.size(0));

    auto&& expr0 = jules::detail::make_expr_ndarray(
        matrix1.data_begin(), matrix1.data_end(), matrix2.data_begin(), matrix2.data_end(),
        [](const int& a, const int& b) { return a + b; }, matrix1.extents());

    auto&& expr1 = matrix1 + matrix2;
    auto&& expr2 = matrix1[0] + matrix2[1];
    auto&& expr3 = matrix1 + base_ndarray<double, 0>(1.0);
}
