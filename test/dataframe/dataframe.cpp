#include "jules/dataframe/dataframe.hpp"
#include "jules/array/all.hpp"

#include <catch.hpp>

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
  //   using jules::column;
  //   using jules::dataframe;

  //   column a(0, 0);
  //   dataframe null;
  //   dataframe df = null.colbind(a).colbind(column(0, 0));

  //   CHECK_FALSE(null.is_null());
  //   CHECK_FALSE(df.is_null());

  //   CHECK(null.is_empty());
  //   CHECK(df.is_empty());

  //   CHECK(df.rows_count() == 0);
  //   CHECK(df.columns_count() == 2);
}

TEST_CASE("Dataframe colbind", "[dataframe]")
{
  //   using jules::column;
  //   using jules::dataframe;

  //   column c("c", 0, 0);
  //   column a("a", 0, 0);
  //   dataframe null;
  //   dataframe df = null.colbind(c).colbind(a).colbind(column("b", 0, 0));

  //   CHECK_FALSE(null.is_null());
  //   CHECK_FALSE(df.is_null());

  //   CHECK(null.is_empty());
  //   CHECK(df.is_empty());

  //   CHECK(df.rows_count() == 0);
  //   CHECK(df.columns_count() == 3);

  //   auto cols = df.columns_names();
  //   auto nullcols = null.columns_names();
  //   struct bla {
  //     std::string unnamed[3];
  //   };
  //   struct foo {
  //     int a;
  //     int b;
  //     int c;
  //   };

  //   foo f = {.a = 1, .b = 2, .c = 3};

  //   std::array<std::string, 3> names{{"c", "a", "b"}};

  //   REQUIRE(cols.size() == 3);
  //   auto mm = std::mismatch(cols.begin(), cols.end(), names.begin());
  //   CHECK(mm.first == cols.end());

  //   REQUIRE(nullcols.size() == 3);
  //   auto mm1 = std::mismatch(cols.begin(), cols.end(), nullcols.begin());
  //   CHECK(mm1.first == cols.end());

  //   column d("d", 0, 0);
  //   dataframe some_df{{"A", {1, 2, 3, 4}}, {"a", {"h", " ", "w", "0"}}};
  //   CHECK_THROWS(some_df.colbind(a));
  //   CHECK_THROWS(some_df.colbind(d));
}

TEST_CASE("Dataframe select by name", "[dataframe]")
{
  using jules::dataframe;
  using namespace std::literals::string_literals;

  dataframe df{{"int", {1, 2, 3, 4}}, {"str", {"h"s, " "s, "w"s, "0"s}}};
  CHECK_THROWS(df.select(""));
  CHECK_THROWS(df.select("h"));
  CHECK(df.select("int").elements_type() == typeid(int));
  CHECK(df.select("str").elements_type() == typeid(std::string));
  CHECK(df.select("str").size() == 4);
}

TEST_CASE("Accessing rows of the dataframe", "[datafrae]")
{
  //   using jules::dataframe;
  //   using namespace std::literals::string_literals;

  //   auto df = dataframe{{0, 1, 2, 3}, {"0"s, "1"s, "2"s, "3"s}, {0.0, 1.0, 2.0, 3.0}};

  //   CHECK_THROWS((df.rows<int, std::string>()));
  //   CHECK_THROWS((df.rows<double, std::string, int>()));

  //   auto rows = df.rows<int, std::string, double>();

  //   for (auto i : jules::seq(0, df.rows_count()))
  //     CHECK((rows[i] == std::tuple<int, std::string, double>(i, std::to_string(i), i)));
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
  //   std::string input = "x\ty\tz\n";

  //   std::stringstream stream(input);

  //   auto df = jules::dataframe::read(stream);
  //   CHECK(df.rows_count() == 0);
  //   CHECK(df.columns_count() == 3);
  //   CHECK(df.is_empty());
}

TEST_CASE("Reading an empty dataframe with line-break", "[dataframe]")
{
  //   std::string input = "\n";

  //   std::stringstream stream(input);

  //   auto df = jules::dataframe::read(stream);
  //   CHECK(df.rows_count() == 0);
  //   CHECK(df.columns_count() == 0);
  //   CHECK(df.is_empty());
}

TEST_CASE("Reading an empty dataframe from an empty string", "[dataframe]")
{
  //   std::string input = "";

  //   std::stringstream stream(input);

  //   auto df = jules::dataframe::read(stream);
  //   CHECK(df.rows_count() == 0);
  //   CHECK(df.columns_count() == 0);
  //   CHECK(df.is_empty());
  //   CHECK(df.is_null());

  //   std::stringstream null_stream;

  //   auto df2 = jules::dataframe::read(null_stream);
  //   CHECK(df2.rows_count() == 0);
  //   CHECK(df2.columns_count() == 0);
  //   CHECK(df2.is_empty());
  //   CHECK(df.is_null());
}

TEST_CASE("Reading a dataframe", "[dataframe]")
{
  //   std::string input = "\t\t \n1 \t 2\t  3\n";

  //   std::stringstream stream(input);

  //   auto df = jules::dataframe::read(stream);
  //   CHECK(df.rows_count() == 1);
  //   CHECK(df.columns_count() == 3);
  //   auto cols = df.columns_names();
  //   CHECK(cols.size() == 3);
  //   CHECK(cols[0] == "");
  //   CHECK(cols[1] == "");
  //   CHECK(cols[2] == " ");
}

TEST_CASE("Reading matrix of integers", "[dataframe]")
{
  //   struct my_integer_rules {
  //     using type = int;
  //     static type coerce_from(const std::string& value) { return std::stoi(value); }
  //   };

  //   struct my_string_rules {
  //     using type = std::string;
  //     static type coerce_from(int value) { return std::to_string(value); }
  //   };

  //   using my_coercion_rules = jules::coercion_rules<my_integer_rules, my_string_rules>;
  //   using my_dataframe = jules::base_dataframe<my_coercion_rules>;

  //   jules::dataframe_read_options opts;
  //   opts.header = false;

  //   std::stringstream stream;
  //   std::size_t N = 10;
  //   for (std::size_t i = 0; i < N; ++i) {
  //     for (std::size_t j = 0; j < N; ++j) {
  //       stream << (i * N + j) << "\t";
  //     }
  //     stream << "\n";
  //   }

  //   auto df = my_dataframe::read(stream, opts);
  //   REQUIRE(df.columns_count() == N);
  //   REQUIRE(df.rows_count() == N);
  //   my_dataframe idf;

  //   std::string -> double: Not OK
  //   CHECK_FALSE(df.select(0).can_coerce_to<double>());
  //   CHECK_THROWS(jules::as_column<double>(df.select(0)));
  //   CHECK(df.select(0).can_coerce_to<int>());

  //   for (std::size_t i = 0; i < df.columns_count(); ++i) {
  //     idf.colbind(jules::as_column<int>(df.select(i)));
  //     REQUIRE(idf.select(idf.columns_count() - 1).elements_type() == typeid(int));
  //   }

  //   coerce all columns
  //   CHECK(jules::can_coerce_to<int>(df));
  //   df.coerce_to<int>();

  //   int -> double: Not OK
  //   CHECK_FALSE(idf.select(0).can_coerce_to<double>());
  //   CHECK_THROWS(jules::as_column<double>(idf.select(0)));
  //   CHECK(idf.select(0).can_coerce_to<int>());

  //   int -> std::string: OK
  //   CHECK(idf.select(0).can_coerce_to<std::string>());
  //   CHECK_NOTHROW(jules::as_column<std::string>(idf.select(0)));

  //   CHECK(idf.columns_count() == N);
  //   CHECK(idf.rows_count() == N);
}

TEST_CASE("Reading an inconsistent dataframe", "[dataframe]")
{
  //   std::string input = "\ty \t\n1 \t 2\t  3";

  //   std::stringstream stream(input);

  //   jules::dataframe df;
  //   CHECK_THROWS(df = jules::dataframe::read(stream));

  //   CHECK(df.rows_count() == 0);
  //   CHECK(df.columns_count() == 0);
  //   CHECK(df.is_null());
  //   CHECK(df.is_empty());
  //   CHECK_THROWS(df.select(0));
}

TEST_CASE("Reading and writing a well-formed dataframe", "[dataframe]")
{
  //   using jules::dataframe;

  //   std::string data = "y\tx\tz\n" + std::to_string(0.0) + "\t" + std::to_string(1.0) + "\t" + std::to_string(2.0) + "\n" +
  //                      std::to_string(3.0) + "\t" + std::to_string(4.0) + "\t" + std::to_string(5.0) + "\n";

  //   std::stringstream is(data);

  //   auto df = jules::dataframe::read(is);
  //   CHECK(df.rows_count() == 2);
  //   CHECK(df.columns_count() == 3);

  //   std::stringstream os1;
  //   os1 << df;
  //   CHECK(data == os1.str());

  //   std::stringstream os2;
  //   write(dataframe{{"y", {0, 3}}, {"x", {1, 4}}, {"z", {2, 5}}}, os2);
  //   CHECK(data == os2.str());

  //   auto cols = df.columns_names();
  //   std::array<std::string, 3> names{{"y", "x", "z"}};
  //   CHECK(cols == names);

  //   REQUIRE(cols.size() == 3);
  //   auto mm = std::mismatch(cols.begin(), cols.end(), names.begin());
  //   CHECK(mm.first == cols.end());

  //   CHECK_THROWS(df.select(-1));
  //   CHECK_THROWS(df.select(4));

  //   auto c1 = df.select(0);
  //   CHECK(c1.name() == "y");

  //   auto c2 = df.select(1);
  //   CHECK(c2.name() == "x");

  //   auto c3 = df.select(2);
  //   CHECK(c3.name() == "z");
}
