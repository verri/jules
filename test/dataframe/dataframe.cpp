#include "catch.hpp"
#include "dataframe/dataframe.hpp"

#include <sstream>

TEST_CASE("tutorial", "[tutorial]")
{
    using jules::dataframe;
    using jules::column;

    dataframe some_df{{"int", {1, 2, 3, 4}}, {"const char*", {"hello", " ", "world", "!"}}};
    dataframe null_df;

    auto create_invalid_dataframe = [] { return dataframe{{1, 2, 3, 4}, {1, 2}}; };
    CHECK_THROWS(create_invalid_dataframe());

    CHECK(some_df != nullptr);
    CHECK(null_df == nullptr);

    some_df = nullptr;
    CHECK(some_df == nullptr);

    std::string data = "x\ty\tz\n" + std::to_string(0.0) + "\t" + std::to_string(1.0) + "\t" +
                       std::to_string(2.0) + "\n" + std::to_string(3.0) + "\t" + std::to_string(4.0) + "\t" +
                       std::to_string(5.0) + "\n";

    std::stringstream is(data);

    auto xyz = dataframe::read(is);
    CHECK(xyz.nrow() == 2);
    CHECK(xyz.ncol() == 3);

    std::stringstream os1;
    dataframe::write(xyz, os1);
    CHECK(data == os1.str());

    std::stringstream os2;
    dataframe::write({{"x", {0, 3}}, {"y", {1, 4}}, {"z", {2, 5}}}, os2);
    CHECK(data == os2.str());
}
