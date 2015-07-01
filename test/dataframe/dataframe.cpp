#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dataframe/dataframe.hpp"

TEST_CASE("tutorial", "[tutorial]")
{
    using jules::dataframe;

    dataframe df{{"int", {1, 2}}, {"const char*", {"hello", " ", "world", "!"}}};

    for (auto&& rows : df.rows()) {
    }

    for (auto&& rows : df.rows<int, const char*>()) {
    }
}
