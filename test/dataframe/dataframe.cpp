#include "catch.hpp"
#include "dataframe/dataframe.hpp"

#include <sstream>

TEST_CASE("null dataframe", "[dataframe]")
{
    using jules::dataframe;

    dataframe df;

    CHECK(df.empty());
    CHECK(df.null());
    CHECK(df.nrow() == 0);
    CHECK(df.ncol() == 0);
}

TEST_CASE("invalid exception dataframe", "[dataframe]")
{
    using jules::dataframe;

    auto create_invalid_dataframe = [] { return dataframe{{1, 2, 3, 4}, {1, 2}}; };
    CHECK_THROWS(create_invalid_dataframe());
}

TEST_CASE("assigning a null dataframe", "[dataframe]")
{
    using jules::dataframe;

    dataframe some_df{{"int", {1, 2, 3, 4}}, {"const char*", {"hello", " ", "world", "!"}}};

    CHECK(!some_df.empty());

    some_df = {};
    CHECK(some_df.empty());
}

TEST_CASE("assigning itself", "[dataframe]")
{
    using jules::dataframe;

    dataframe some_df{{"int", {1, 2, 3, 4}}, {"const char*", {"hello", " ", "world", "!"}}};

    some_df = some_df;
}

TEST_CASE("reading an empty dataframe with header", "[dataframe]")
{
    std::string input = "x\ty\tz\n";

    std::stringstream stream(input);

    auto df = jules::dataframe::read(stream);
    CHECK(df.nrow() == 0);
    CHECK(df.ncol() == 3);
    CHECK(df.empty());
}

TEST_CASE("reading an empty dataframe with line-break", "[dataframe]")
{
    std::string input = "\n";

    std::stringstream stream(input);

    auto df = jules::dataframe::read(stream);
    CHECK(df.nrow() == 0);
    CHECK(df.ncol() == 0);
    CHECK(df.empty());
}

TEST_CASE("reading an empty dataframe from an empty string", "[dataframe]")
{
    std::string input = "";

    std::stringstream stream(input);

    auto df = jules::dataframe::read(stream);
    CHECK(df.nrow() == 0);
    CHECK(df.ncol() == 0);
    CHECK(df.empty());
}

TEST_CASE("reading a dataframe", "[dataframe]")
{
    std::string input = "\t\t \n1 \t 2\t  3\n";

    std::stringstream stream(input);

    auto df = jules::dataframe::read(stream);
    CHECK(df.nrow() == 1);
    CHECK(df.ncol() == 3);
    auto cols = df.colnames();
    // CHECK(cols.at(0) == "")
    // CHECK(cols.at(1) == "")
    // CHECK(cols.at(2) == " ")
}

TEST_CASE("reading an inconsistent dataframe (no line-break)", "[dataframe]")
{
    std::string input = "\ty \t\n1 \t 2\t  3";

    std::stringstream stream(input);

    auto df = jules::dataframe::read(stream);
    CHECK(df.nrow() == 1);
    CHECK(df.ncol() == 3);
}

TEST_CASE("reading and writing a well-formed dataframe", "[dataframe]")
{
    using jules::dataframe;

    std::string data = "x\ty\tz\n" + std::to_string(0.0) + "\t" + std::to_string(1.0) + "\t" +
                       std::to_string(2.0) + "\n" + std::to_string(3.0) + "\t" + std::to_string(4.0) + "\t" +
                       std::to_string(5.0) + "\n";

    std::stringstream is(data);

    auto xyz = jules::dataframe::read(is);
    CHECK(xyz.nrow() == 2);
    CHECK(xyz.ncol() == 3);

    std::stringstream os1;
    dataframe::write(xyz, os1);
    CHECK(data == os1.str());

    std::stringstream os2;
    dataframe::write({{"x", {0, 3}}, {"y", {1, 4}}, {"z", {2, 5}}}, os2);
    CHECK(data == os2.str());
}
