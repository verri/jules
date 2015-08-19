#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dataframe/dataframe.hpp"

TEST_CASE("tutorial", "[tutorial]")
{
    using jules::dataframe;
    using jules::column;

    dataframe some_df{{"int", {1, 2, 3, 4}}, {"const char*", {"hello", " ", "world", "!"}}};
    dataframe null_df;

    auto create_invalid_dataframe = []() { return dataframe{{1, 2, 3, 4}, {1, 2}}; };
    CHECK_THROWS(create_invalid_dataframe());

    CHECK(some_df != nullptr);
    CHECK(null_df == nullptr);

    some_df = nullptr;
    CHECK(some_df == nullptr);
}
