#include "catch.hpp"
#include "jules/dataframe/column.hpp"
#include "jules/dataframe/dataframe.hpp"

#include <string>
#include <typeindex>

using jules::range_value_t;

template <typename Range, typename R = range_value_t<Range>> R make_value(const Range&) { return {}; }

TEST_CASE("column constructor using initializer list", "[constructor]")
{
    using jules::column;

    struct Toy {
    };

    auto int_column = column{"int", {1, 2, 3, 4, 5}};
    auto c_str_column = column("char*", {"hello", "world"});
    auto toy_column = column("toy", {Toy{}, Toy{}});

    REQUIRE(int_column.elements_type() == typeid(int));
    REQUIRE(c_str_column.elements_type() == typeid(const char*));
    REQUIRE(toy_column.elements_type() == typeid(Toy));

    REQUIRE(int_column.can_coerce_to<double>());
    REQUIRE(int_column.can_coerce_to<std::string>());

    REQUIRE(c_str_column.can_coerce_to<double>());
    REQUIRE(c_str_column.can_coerce_to<std::string>());

    REQUIRE(!toy_column.can_coerce_to<double>());
    REQUIRE(!toy_column.can_coerce_to<std::string>());

    using jules::coerce_to;
    using jules::make_view;

    auto string_numbers = column{"1.0", "2.4", "3.3"};
    auto double_numbers = coerce_to<double>(string_numbers);

    auto v = make_view<double>(double_numbers);

    CHECK(v[0] == 1.0);
    CHECK(v[1] == 2.4);
    CHECK(v[2] == 3.3);

    std::vector<double> tmp = {1.0, 2.0, 3.0};
    column range_column(tmp);
    CHECK(range_column.elements_type() == typeid(double));

    static_assert(std::is_same<decltype(make_value(std::vector<double>{})), double>::value, "");
    static_assert(std::is_same<decltype(make_value(v)), double>::value, "");
}

TEST_CASE("column constructor inference", "[constructor]")
{
    using jules::column;
    using namespace std::literals::string_literals;

    auto check_column = [](const column& col, const auto& value) { REQUIRE(col.elements_type() == value); };

    check_column({"int", {1, 2, 3}}, typeid(int));
    check_column({1.0, 2.0, 3.0, 1.0}, typeid(double));
    check_column({"1.0"s, "2.0"s, "3.0"s, "1.0"s}, typeid(std::string));
}

TEST_CASE("temporary columns", "[column]")
{
    jules::column col{{1, 2, 3, 4, 5}};
    jules::dataframe df;

    df.colbind(col);
    auto c = jules::coerce_to<double>(df.select(0));
    auto view = c.view<double>();
    // auto view = jules::coerce_to<double>(df.select(0)).view<double>();

    for (std::size_t i = 0; i < df.nrow(); ++i)
        CHECK(view[i] == i + 1);
}
