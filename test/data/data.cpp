#include "jules/data/data.hpp"

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <vector>

TEST_CASE("Null data", "[data]")
{
  using jules::data;

  data df;

  CHECK(!df);
  CHECK(df.row_count() == 0);
  CHECK(df.column_count() == 0);

  const auto [nrows, ncols] = df.dimensions();
  CHECK(nrows == 0);
  CHECK(ncols == 0);
}

TEST_CASE("Null data colbind", "[data]")
{
  using jules::column;
  using jules::data;

  const auto a = column{0, 0, 0};
  const auto null = data();

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

  df2 = std::move(df1); // move guarantees null
  CHECK_FALSE(df1);
  CHECK(df2);
}

TEST_CASE("Dataframe colbind", "[data]")
{
  using jules::column;
  using jules::data;
  using jules::string;

  auto c = column(0, 0);
  auto a = column(0, 0);

  auto null = data();
  auto df = data();

  df.bind({{"c", c}}).bind({{"a", a}, {"b", column(0, 0)}});

  CHECK_FALSE(null);
  CHECK(df);

  CHECK(null.row_count() == 0u);
  CHECK(null.column_count() == 0u);

  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 3u);

  const auto df_names = df.names();
  CHECK(df_names.size() == 3u);
  CHECK(all(df_names == jules::vector<std::string>{"c", "a", "b"}));

  const auto d = column(0, 0);
  auto some_df = data{{"A", {1, 2, 3, 4}}, {"a", {"h", " ", "w", "0"}}};
  CHECK_THROWS(some_df.bind(d));
}

TEST_CASE("Dataframe select by name", "[data]")
{
  using jules::data;
  using namespace jules::literals;

  data df{{"int", {1, 2, 3, 4}}, {"str", {"h"_s, " "_s, "w"_s, "0"_s}}};
  CHECK_THROWS(df.at(""));
  CHECK_THROWS(df.at("h"));
  CHECK(df.at("int").column.elements_type() == typeid(int));
  CHECK(df.at("str").column.elements_type() == typeid(jules::string));
  CHECK(df.at("str").column.size() == 4);
}

TEST_CASE("Accessing rows of the data", "[data]")
{
  // TODO: row_view
}

TEST_CASE("Invalid data exception", "[data]")
{
  using jules::data;

  auto create_invalid_data = [] { return data{{"a", {1, 2, 3, 4}}, {"b", {1, 2}}}; };
  CHECK_THROWS(create_invalid_data());
}

TEST_CASE("Constructing a data from a range of columns", "[data]")
{
  using jules::column;
  using jules::data;

  auto columns = std::vector<data::named_column_type>{
    {"a", {1, 2, 3}},
    {"b", {'a', 'b', 'c'}},
    {"c", {1.0, 2.0, 3.0}},
  };

  auto df = data(columns);

  CHECK(df.row_count() == 3u);
  CHECK(df.column_count() == 3u);

  auto df2 = data(columns.begin() + 1, columns.end());

  CHECK(df2.row_count() == 3u);
  CHECK(df2.column_count() == 2u);

  namespace view = jules::ranges::views;

  auto types = jules::as_vector(columns | view::transform([](const auto& elem) { return elem.column.elements_type(); }));
  CHECK(all(df.column_types() == types));

  auto names = jules::as_vector(columns | view::transform([](const auto& elem) { return elem.name; }));
  CHECK(all(df.names() == names));
}

TEST_CASE("Assigning a null data", "[data]")
{
  using jules::data;

  data some_df{{"int", {1, 2, 3, 4}}, {"const char*", {"hello", " ", "world", "!"}}};
  CHECK(some_df);

  some_df = {};
  CHECK(!some_df);
}

TEST_CASE("Assigning a data to itself", "[data]")
{
  using jules::data;

  data some_df{{"int", {1, 2, 3, 4}}, {"const char*", {"hello", " ", "world", "!"}}};
  CHECK_NOTHROW(some_df = some_df);
}

TEST_CASE("Reading an empty data with header", "[data]")
{
  using namespace jules::literals;

  auto stream = std::stringstream("x\ty\tz\n");

  auto df = jules::data::read(stream);

  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 3u);
  CHECK(all(df.names() == jules::cat("x"_s, "y"_s, "z"_s)));
}

TEST_CASE("Reading an empty data with line-break", "[data]")
{
  auto stream = std::stringstream("\n");

  auto df = jules::data::read(stream);
  CHECK_FALSE(df);
  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 0u);
}

TEST_CASE("Reading an empty data from an empty string", "[data]")
{
  auto stream = std::stringstream("");

  auto df = jules::data::read(stream);
  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 0u);
  CHECK_FALSE(df);

  auto bad_stream = std::stringstream();
  auto tmp = 0;
  bad_stream >> tmp;

  auto df2 = jules::data::read(bad_stream);
  CHECK(df2.row_count() == 0u);
  CHECK(df2.column_count() == 0u);
  CHECK_FALSE(df2);
}

TEST_CASE("Reading a data", "[data]")
{
  using namespace jules::literals;

  auto stream = std::stringstream("\t\t \n1 \t 2\t  3\n");

  auto df = jules::data::read(stream);
  CHECK(df.row_count() == 1u);
  CHECK(df.column_count() == 3u);

  auto cols = df.names();
  CHECK(all(cols == jules::cat(""_s, ""_s, ""_s)));
}

TEST_CASE("Reading matrix of integers", "[data]")
{
  using namespace jules;

  struct Foo
  {
    operator int() const { return 0; }
  };

  struct custom_rules
  {
    using types = std::tuple<string, Foo, int>;
    [[nodiscard]] auto coerce(tag<Foo>, tag<int>, const Foo& foo) const -> int { return foo; }
    [[nodiscard]] auto coerce(tag<string>, tag<int>, const string& str) const -> int
    {
      return boost::lexical_cast<int>(str.view().data(), str.view().size());
    }
  };

  (void)custom_rules::types{}; // XXX: weird...
  using custom_data = base_data<custom_rules>;

  auto opts = custom_data::read_options();
  opts.header = false;

  auto stream = std::stringstream();
  constexpr auto N = 3u;
  for (auto i = 0u; i < N; ++i) {
    for (auto j = 0u; j < N; ++j) {
      stream << (i * N + j) << "\t";
    }
    stream << "\n";
  }

  auto df = custom_data::read(stream, opts);
  CHECK(df.column_count() == N);
  CHECK(df.row_count() == N);

  CHECK_FALSE(df.at(0u).column.can_coerce<double>());
  CHECK_THROWS(jules::to_column<double>(df.at(0).column));
  CHECK(df.at(0).column.can_coerce<int>());

  auto idf = custom_data();

  for (auto i = 0u; i < df.column_count(); ++i) {
    idf.bind({string{}, jules::to_column<int>(df.at(i).column)});
    CHECK(idf.at(idf.column_count() - 1).column.elements_type() == typeid(int));
  }

  //   // TODO: coerce all columns
  //   // df.coerce_to<int, ...>();
  //
  //   // int -> double: Not OK
  //   CHECK_FALSE(idf.at(0).column.can_coerce<double>());
  //   CHECK_THROWS(jules::to_column<double>(idf.at(0).column));
  //   CHECK(idf.at(0).column.can_coerce<int>());
  //
  //   // int -> string: OK
  //   CHECK(idf.at(0).column.can_coerce<jules::string>());
  //   CHECK_NOTHROW(jules::to_column<jules::string>(idf.at(0).column));
  //
  //   CHECK(idf.column_count() == N);
  //   CHECK(idf.row_count() == N);
  //
  //   CHECK_THROWS(jules::to_column<Foo>(idf.at(0).column));
  //
  //   // Converting to matrix
  //   const auto imatrix = jules::to_matrix<int>(idf);
  //   CHECK(all(imatrix == jules::matrix<int>{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}}));
  //
  //   const auto smatrix = jules::to_matrix<jules::string>(idf);
  //   CHECK(all(smatrix == jules::matrix<jules::string>{{"0", "1", "2"}, {"3", "4", "5"}, {"6", "7", "8"}}));
}

TEST_CASE("Reading an inconsistent data", "[data]")
{
  auto stream = std::stringstream("\ty \t\n1 \t 2\t  3");

  auto df = jules::data();
  CHECK_THROWS(df = jules::data::read(stream));

  CHECK(df.row_count() == 0u);
  CHECK(df.column_count() == 0u);
  CHECK_FALSE(df);
  CHECK_THROWS(df.at(0));
}

TEST_CASE("Reading and writing a well-formed data", "[data]")
{
  using namespace jules::literals;
  using namespace std::string_literals;
  using jules::data;

  const auto str = "y\tx\tz\n0\t1\t2\n3\t4\t5\n"s;

  auto is = std::stringstream(str);

  auto df = jules::data::read(is);
  CHECK(df.row_count() == 2u);
  CHECK(df.column_count() == 3u);
  CHECK(df);

  auto os1 = std::stringstream();
  os1 << df;
  CHECK(str == os1.str());

  auto os2 = std::stringstream();

  data{{"y", {0.0, 3.0}}, {"x", {1.0, 4.0}}, {"z", {2.0, 5.0}}}.write(os2);
  CHECK(str == os2.str());

  const auto cols = df.names();
  CHECK(cols.size() == 3u);
  CHECK(all(cols == jules::cat("y"_s, "x"_s, "z"_s)));

  CHECK_THROWS(df.at(-1));
  CHECK_THROWS(df.at(4));

  const auto& c1 = df.at(0);
  CHECK(c1.name == "y");

  const auto& c2 = df.at(1);
  CHECK(c2.name == "x");

  const auto& c3 = df.at(2);
  CHECK(c3.name == "z");

  auto os3 = std::stringstream();
  os3 << data{};
  CHECK(os3.str() == "");
}
