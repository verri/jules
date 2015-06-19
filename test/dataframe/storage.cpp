#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dataframe/detail/storage.hpp"

#include <iterator>
#include <numeric>

TEST_CASE("coercion", "[coercion]")
{
    using namespace jules;
    using namespace jules::dataframe_detail;

    using storage_ptr = std::unique_ptr<storage_eraser<default_coercion_rules>>;

    storage_ptr istorage{new storage<int, default_coercion_rules>};
    // Learns the coercions: int -(explicit)-> double
    //                       int -(implicit)-> double -(explicit)-> std::string
    storage_ptr dstorage{new storage<double, default_coercion_rules>};
    // Learns the coercions: double -(implicit)-> double
    //                       double -(explicit)-> std::string

    CHECK(istorage->elements_type() == typeid(int));
    CHECK(dstorage->elements_type() == typeid(double));
    CHECK(dstorage->elements_type() != typeid(int));
    CHECK(istorage->elements_type() != typeid(double));

    CHECK(!istorage->can_coerce_to<int>());
    CHECK(istorage->can_coerce_to<double>());
    CHECK(!istorage->can_coerce_to<unsigned int>());
    CHECK(istorage->can_coerce_to<std::string>());

    CHECK(!dstorage->can_coerce_to<int>());
    CHECK(dstorage->can_coerce_to<double>());
    CHECK(dstorage->can_coerce_to<std::string>());

    auto& ivec = istorage->downcast<int>();
    ivec.resize(10);
    std::iota(ivec.begin(), ivec.end(), 0);

    auto& dvec = dstorage->downcast<double>();
    std::transform(ivec.begin(), ivec.end(), std::back_inserter(dvec), [](int x) { return 3.1415 * x; });

    std::vector<std::string> expected{"0.000000",  "3.141500",  "6.283000",  "9.424500",  "12.566000",
                                      "15.707500", "18.849000", "21.990500", "25.132000", "28.273500"};

    auto sstorage = dstorage->coerce_to<std::string>();
    auto& svec = sstorage->downcast<std::string>();

    auto check = std::mismatch(svec.begin(), svec.end(), expected.begin());
    CHECK(check.first == svec.end());
    CHECK(check.second == expected.end());

    auto back_to_dstorage = sstorage->coerce_to<double>();
    auto& back_to_dvec = back_to_dstorage->downcast<double>();

    auto check_back = std::mismatch(dvec.begin(), dvec.end(), back_to_dvec.begin(),
                                    [](auto x, auto y) { return x == Approx(y); });
    CHECK(check_back.first == dvec.end());
    CHECK(check_back.second == back_to_dvec.end());

    CHECK_THROWS_AS(sstorage->downcast<int>(), std::bad_cast);
    CHECK_NOTHROW(sstorage->downcast<std::string>());
}
