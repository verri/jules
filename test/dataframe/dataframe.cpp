#include "jules/dataframe/dataframe.hpp"
#include "jules/array/all.hpp"

#include <catch.hpp>

#include <sstream>
#include <vector>

TEST_CASE("Null dataframe", "[dataframe]")
{
  using jules::dataframe;

  dataframe df;

  CHECK(!df);
  CHECK(df.row_count() == 0);
  CHECK(df.column_count() == 0);
}

TEST_CASE("Null dataframe colbind", "[dataframe]")
{
  using jules::column;
  using jules::dataframe;

  const auto a = column{0, 0, 0};
  const auto null = dataframe();

  auto df1 = null;
  auto df2 = null;

  df1.bind(a).bind(column{0, 0, 0});
  df2.bind({{"", a}, {"", {0, 0, 0}}});

  CHECK_FALSE(null);
  CHECK(df1);
  CHECK(df2);

  CHECK(df1.row_count() == 3u);
  CHECK(df1.column_count() == 2u);

  CHECK(df2.row_count() == df1.row_count());
  CHECK(df2.column_count() == df1.column_count());
}

TEST_CASE("Dataframe colbind", "[dataframe]")
{
  using jules::column;
  using jules::dataframe;
  using jules::string;

  auto c = column(0, 0);
  auto a = column(0, 0);

  auto null = dataframe();
  auto df = dataframe();

  df.bind({{"c", c}}).bind({{"a", a}, {"b", column(0, 0)}});

  CHECK_FALSE(null);
  CHECK(df);

  CHECK(null.row_count() == 0u);
  CHECK(null.column_count() == 0u);

  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 3u);

  const auto df_names = df.names();
  REQUIRE(df_names.size() == 3u);
  CHECK(all(df_names == jules::as_vector("c", "a", "b")));

  const auto d = column(0, 0);
  auto some_df = dataframe{{"A", {1, 2, 3, 4}}, {"a", {"h", " ", "w", "0"}}};
  CHECK_THROWS(some_df.bind(d));
}

TEST_CASE("Dataframe select by name", "[dataframe]")
{
  using jules::dataframe;
  using namespace std::literals::string_literals;

  dataframe df{{"int", {1, 2, 3, 4}}, {"str", {"h"s, " "s, "w"s, "0"s}}};
  CHECK_THROWS(df.at(""));
  CHECK_THROWS(df.at("h"));
  CHECK(df.at("int").column.elements_type() == typeid(int));
  CHECK(df.at("str").column.elements_type() == typeid(std::string));
  CHECK(df.at("str").column.size() == 4);
}

TEST_CASE("Accessing rows of the dataframe", "[dataframe]")
{
  // TODO: row_view
}

TEST_CASE("Invalid dataframe exception", "[dataframe]")
{
  using jules::dataframe;

  auto create_invalid_dataframe = [] { return dataframe{{"a", {1, 2, 3, 4}}, {"b", {1, 2}}}; };
  CHECK_THROWS(create_invalid_dataframe());
}

TEST_CASE("Constructing a dataframe from a range of columns", "[dataframe]")
{
  using jules::column;
  using jules::dataframe;

  auto columns = std::vector<dataframe::named_column_type>{
    {"a", {1, 2, 3}}, {"b", {'a', 'b', 'c'}}, {"c", {1.0, 2.0, 3.0}},
  };

  auto df = dataframe(columns);

  CHECK(df.row_count() == 3u);
  CHECK(df.column_count() == 3u);

  auto df2 = dataframe(columns.begin() + 1, columns.end());

  CHECK(df2.row_count() == 3u);
  CHECK(df2.column_count() == 2u);

  namespace view = jules::range::view;

  auto types = jules::as_vector(columns | view::transform([](const auto& elem) { return elem.column.elements_type(); }));
  CHECK(all(df.column_types() == types));

  auto names = jules::as_vector(columns | view::transform([](const auto& elem) { return elem.name; }));
  CHECK(all(df.names() == names));
}

TEST_CASE("Implicit conversion from a column to a dataframe", "[dataframe]")
{
  using jules::column;
  using jules::dataframe;
  using jules::index_t;

  const auto f = [](dataframe df, index_t size, std::type_index type) {
    CHECK(df.row_count() == size);
    CHECK(df.at(0u).column.elements_type() == type);
  };

  auto col = column{1, 2, 3, 4};
  const auto size = col.size();
  const auto type = col.elements_type();

  f(col, size, type);
  f(std::move(col), size, type);
}

TEST_CASE("Assigning a null dataframe", "[dataframe]")
{
  using jules::dataframe;

  dataframe some_df{{"int", {1, 2, 3, 4}}, {"const char*", {"hello", " ", "world", "!"}}};
  CHECK(some_df);

  some_df = {};
  CHECK(!some_df);
}

TEST_CASE("Assigning a dataframe to itself", "[dataframe]")
{
  using jules::dataframe;

  dataframe some_df{{"int", {1, 2, 3, 4}}, {"const char*", {"hello", " ", "world", "!"}}};
  CHECK_NOTHROW(some_df = some_df);
}

TEST_CASE("Reading an empty dataframe with header", "[dataframe]")
{
  std::stringstream stream("x\ty\tz\n");

  auto df = jules::dataframe::read(stream);

  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 3u);
  CHECK(all(df.names() == jules::as_vector("x", "y", "z")));
}

TEST_CASE("Reading an empty dataframe with line-break", "[dataframe]")
{
  std::stringstream stream("\n");

  auto df = jules::dataframe::read(stream);
  CHECK_FALSE(df);
  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 0u);
}

TEST_CASE("Reading an empty dataframe from an empty string", "[dataframe]")
{
  std::stringstream stream("");

  auto df = jules::dataframe::read(stream);
  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 0u);
  CHECK_FALSE(df);

  std::stringstream null_stream;

  auto df2 = jules::dataframe::read(null_stream);
  CHECK(df2.row_count() == 0u);
  CHECK(df2.column_count() == 0u);
  CHECK_FALSE(df2);
}

TEST_CASE("Reading a dataframe", "[dataframe]")
{
  std::stringstream stream("\t\t \n1 \t 2\t  3\n");

  auto df = jules::dataframe::read(stream);
  CHECK(df.row_count() == 1u);
  CHECK(df.column_count() == 3u);

  auto cols = df.names();
  CHECK(all(cols == jules::as_vector("", "", " ")));
}

TEST_CASE("Reading matrix of integers", "[dataframe]")
{
  struct my_integer_rules {
    using type = int;
    static auto coerce_from(const jules::string& value) -> type { return std::stoi(value); }
  };

  struct my_string_rules {
    using type = jules::string;
    static auto coerce_from(int value) -> type { return std::to_string(value); }
  };

  using my_coercion_rules = jules::base_coercion_rules<my_integer_rules, my_string_rules>;
  using my_dataframe = jules::base_dataframe<my_coercion_rules>;

  auto opts = my_dataframe::read_options();
  opts.header = false;

  std::stringstream stream;
  constexpr auto N = 10u;
  for (auto i = 0u; i < N; ++i) {
    for (auto j = 0u; j < N; ++j) {
      stream << (i * N + j) << "\t";
    }
    stream << "\n";
  }

  auto df = my_dataframe::read(stream, opts);
  REQUIRE(df.column_count() == N);
  REQUIRE(df.row_count() == N);

  // jules::string -> double: Not OK
  CHECK_FALSE(df.at(0u).column.can_coerce<double>());
  CHECK_THROWS(jules::to_column<double>(df.at(0).column));
  CHECK(df.at(0).column.can_coerce<int>());

  auto idf = my_dataframe();

  for (auto i = 0u; i < df.column_count(); ++i) {
    idf.bind(jules::to_column<int>(df.at(i).column));
    REQUIRE(idf.at(idf.column_count() - 1).column.elements_type() == typeid(int));
  }

  // TODO: coerce all columns
  // df.coerce_to<int, ...>();

  // int -> double: Not OK
  CHECK_FALSE(idf.at(0).column.can_coerce<double>());
  CHECK_THROWS(jules::to_column<double>(idf.at(0).column));
  CHECK(idf.at(0).column.can_coerce<int>());

  // int -> std::string: OK
  CHECK(idf.at(0).column.can_coerce<std::string>());
  CHECK_NOTHROW(jules::to_column<jules::string>(idf.at(0).column));

  CHECK(idf.column_count() == N);
  CHECK(idf.row_count() == N);
}

TEST_CASE("Reading an inconsistent dataframe", "[dataframe]")
{
  std::stringstream stream("\ty \t\n1 \t 2\t  3");

  auto df = jules::dataframe();
  CHECK_THROWS(df = jules::dataframe::read(stream));

  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 0u);
  CHECK_FALSE(df);
  CHECK_THROWS(df.at(0));
}

TEST_CASE("Reading and writing a well-formed dataframe", "[dataframe]")
{
  using jules::dataframe;

  const auto data = "y\tx\tz\n" + std::to_string(0.0) + "\t" + std::to_string(1.0) + "\t" + std::to_string(2.0) + "\n" +
                    std::to_string(3.0) + "\t" + std::to_string(4.0) + "\t" + std::to_string(5.0) + "\n";

  std::stringstream is(data);

  auto df = jules::dataframe::read(is);
  CHECK(df.row_count() == 2u);
  CHECK(df.column_count() == 3u);
  CHECK(df);

  std::stringstream os1;
  os1 << df;
  CHECK(data == os1.str());

  std::stringstream os2;

  dataframe{{"y", {0, 3}}, {"x", {1, 4}}, {"z", {2, 5}}}.write(os2);
  CHECK(data == os2.str());

  const auto cols = df.names();
  REQUIRE(cols.size() == 3u);
  CHECK(all(cols == jules::as_vector("y", "x", "z")));

  CHECK_THROWS(df.at(-1));
  CHECK_THROWS(df.at(4));

  const auto& c1 = df.at(0);
  CHECK(c1.name == "y");

  const auto& c2 = df.at(1);
  CHECK(c2.name == "x");

  const auto& c3 = df.at(2);
  CHECK(c3.name == "z");
}
