#include "jules/dataframe/column.hpp"
#include "jules/dataframe/numeric.hpp"

#include <catch.hpp>

#include <typeindex>

template <typename Range, typename R = jules::range::range_value_t<Range>> auto make_value(const Range&) -> R { return {}; }

TEST_CASE("Column constructor using initializer list", "[dataframe]")
{
  using jules::column;
  using jules::numeric;
  using jules::string;

  struct Toy {
  };

  auto int_column = column{"int", {1, 2, 3, 4, 5}};
  auto c_str_column = column("char*", {"hello", "world"});
  auto toy_column = column("toy", {Toy{}, Toy{}});

  REQUIRE(int_column.elements_type() == typeid(int));
  REQUIRE(c_str_column.elements_type() == typeid(const char*));
  REQUIRE(toy_column.elements_type() == typeid(Toy));

  REQUIRE(int_column.can_coerce<numeric>());
  REQUIRE(int_column.can_coerce<string>());

  REQUIRE(c_str_column.can_coerce<numeric>());
  REQUIRE(c_str_column.can_coerce<string>());

  REQUIRE(!toy_column.can_coerce<numeric>());
  REQUIRE(!toy_column.can_coerce<string>());

  using jules::to_column;
  //   using jules::as_view;

  auto string_numbers = column{"1.0", "2.4", "3.3"};
  auto numeric_numbers = jules::to_column<numeric>(string_numbers);
  //
  //   auto v = as_view<numeric>(numeric_numbers);
  //
  //   CHECK(numeric_numbers.size() == v.size());
  //   CHECK(!numeric_numbers.is_empty());
  //
  //   CHECK(v[0] == 1.0);
  //   CHECK(v[1] == 2.4);
  //   CHECK(v[2] == 3.3);
  //
  //   std::vector<numeric> tmp = {1.0, 2.0, 3.0};
  //   column range_column(tmp);
  //   CHECK(range_column.elements_type() == typeid(numeric));
  //
  //   static_assert(std::is_same<decltype(make_value(std::vector<numeric>{})), numeric>::value, "");
  //   static_assert(std::is_same<decltype(make_value(v)), numeric>::value, "");
}

TEST_CASE("Column constructor inference", "[dataframe]")
{
  using jules::column;
  using jules::numeric;
  using jules::string;
  using namespace std::literals::string_literals;

  auto check_column = [](const column& col, const auto& value) { REQUIRE(col.elements_type() == value); };

  check_column({"int", {1, 2, 3}}, typeid(int));
  check_column({1.0, 2.0, 3.0, 1.0}, typeid(numeric));
  check_column({"1.0"s, "2.0"s, "3.0"s, "1.0"s}, typeid(string));
}

// TEST_CASE("Temporary columns", "[dataframe]")
// {
//   jules::column col{{1, 2, 3, 4, 5}};
//   jules::dataframe df;
//
//   df.colbind(col);
//   auto c = jules::as_column<numeric>(df.select(0));
//   auto view = jules::as_view<numeric>(c);
//   // auto impossible_view = jules::as_view<numeric>(jules::as_column<numeric>(df.select(0)));
//
//   for (std::size_t i = 0; i < df.rows_count(); ++i)
//     CHECK(view[i] == i + 1);
// }

TEST_CASE("Column as_view vs as_vector", "[dataframe]")
{
  //   auto col = jules::column{{0, 1, 2, 3, 4, 5}};
  //
  //   auto view1 = jules::as_view<int>(col);
  //   auto view2 = jules::as_view<int>(col);
  //   auto vector = jules::as_vector<int>(col);
  //
  //   for (int i : jules::slice(0, col.size())) {
  //     view1[i] = -1;
  //     CHECK(view2[i] == -1);
  //     CHECK(vector[i] == i);
  //   }
}
