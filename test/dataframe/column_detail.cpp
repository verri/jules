#include "catch.hpp"
#include "jules/dataframe/detail/column.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>

TEST_CASE("coercion", "[coercion]")
{
    using namespace jules;
    using namespace jules::detail;

    using column_interface_ptr = std::unique_ptr<column_interface<default_coercion_rules>>;

    column_interface_ptr icolumn{new column_model<int, default_coercion_rules>};
    // Learns the coercions: int -(explicit)-> double
    //                       int -(implicit)-> double -(explicit)-> std::string
    column_interface_ptr dcolumn{new column_model<double, default_coercion_rules>};
    // Learns the coercions: double -(implicit)-> double
    //                       double -(explicit)-> std::string

    CHECK(icolumn->elements_type() == typeid(int));
    CHECK(dcolumn->elements_type() == typeid(double));
    CHECK(dcolumn->elements_type() != typeid(int));
    CHECK(icolumn->elements_type() != typeid(double));

    CHECK(!icolumn->can_coerce_to<int>());
    CHECK(icolumn->can_coerce_to<double>());
    CHECK(!icolumn->can_coerce_to<unsigned int>());
    CHECK(icolumn->can_coerce_to<std::string>());

    CHECK(!dcolumn->can_coerce_to<int>());
    CHECK(dcolumn->can_coerce_to<double>());
    CHECK(dcolumn->can_coerce_to<std::string>());

    auto& ivec = icolumn->downcast<int>();
    ivec.resize(10);
    std::iota(ivec.begin(), ivec.end(), 0);

    auto& dvec = dcolumn->downcast<double>();
    std::transform(ivec.begin(), ivec.end(), std::back_inserter(dvec), [](int x) { return 3.1415 * x; });

    std::vector<std::string> expected{"0.000000",  "3.141500",  "6.283000",  "9.424500",  "12.566000",
                                      "15.707500", "18.849000", "21.990500", "25.132000", "28.273500"};

    auto scolumn = dcolumn->coerce_to<std::string>();
    auto& svec = scolumn->downcast<std::string>();

    auto check = std::mismatch(svec.begin(), svec.end(), expected.begin());
    CHECK(check.first == svec.end());
    CHECK(check.second == expected.end());

    auto back_to_dcolumn = scolumn->coerce_to<double>();
    auto& back_to_dvec = back_to_dcolumn->downcast<double>();

    auto check_back = std::mismatch(dvec.begin(), dvec.end(), back_to_dvec.begin(),
                                    [](auto x, auto y) { return x == Approx(y); });
    CHECK(check_back.first == dvec.end());
    CHECK(check_back.second == back_to_dvec.end());

    CHECK_THROWS_AS(scolumn->downcast<int>(), std::bad_cast);
    CHECK_NOTHROW(scolumn->downcast<std::string>());

    auto clone = scolumn->clone();
    CHECK(clone->elements_type() == scolumn->elements_type());
}
