#define CATCH_CONFIG_MAIN

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

    std::stringstream ss("x\ty\tz\n"
                         "0\t1\t2\n"
                         "3\t4\t5");

    auto xyz = dataframe::read(ss);
    CHECK(xyz.nrow() == 2);
    CHECK(xyz.ncol() == 3);
}
