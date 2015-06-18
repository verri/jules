#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dataframe/dataframe.hpp"

TEST_CASE("tutorial", "[tutorial]") {
    using jules::data_frame;

    data_frame null_df;
    REQUIRE(null_df == nullptr);

    struct Foo {};
    data_frame manual_df{
        {"int", {0, 1}},
        {"const char*", {"hello", " ", "world", "!"}},
        {"foo", {Foo{}}},
    };

    REQUIRE(manual_df.nrow() == 4);
    REQUIRE(manual_df.ncol() == 3);

    REQUIRE(manual_df["int"].are_elements<int>());
    REQUIRE(manual_df["const char *"].are_elements<const char *>());
    REQUIRE(manual_df["foo"].are_elements<Foo>());

    vector_t<string_t> string_vector{"hello", " ", "world", "!"};
    data_frame range_df{
        {"int", range(0, 2)},
        {"string", string_vector},
        {"foo", rep(Foo{}, 2)},
    };

    REQUIRE(range_df.nrow() == 4);
    REQUIRE(range_df.ncol() == 3);

    REQUIRE(range_df["int"].are_elements<int>());
    REQUIRE(range_df["const char *"].are_elements<string_t>());
    REQUIRE(range_df["foo"].are_elements<Foo>());
}

