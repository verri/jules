#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dataframe/column.hpp"

#include <typeindex>

TEST_CASE("column constructor using initializer list", "[constructor]")
{
    using jules::column;

    struct Toy {
    };

    auto int_column = column{"int", {1, 2, 3, 4, 5}};
    auto c_str_column = column("char*", {"hello", "world"});
    auto toy_column = column("toy", {Toy{}, Toy{}});

    REQUIRE(int_column.can_coerse_to<int>());
    REQUIRE(c_str_column.can_coerse_to<const char*>());
    REQUIRE(toy_column.can_coerse_to<Toy>());

    REQUIRE(int_column.can_coerse_to_numeric());
    REQUIRE(int_column.can_coerse_to_string());

    REQUIRE(!c_str_column.can_coerse_to_numeric());
    REQUIRE(c_str_column.can_coerse_to_string());

    REQUIRE(!toy_column.can_coerse_to_numeric());
    REQUIRE(!toy_column.can_coerse_to_string());
}

TEST_CASE("column constructor inference", "[constructor]")
{
    using jules::column;

    auto check_column = [](const column& col, auto value) { REQUIRE(col.can_coerse_to<decltype(value)>()); };

    check_column({"int", {1, 2, 3}}, int{});
    check_column({1.0, 2.0, 3.0, 1.0}, double{});
}
