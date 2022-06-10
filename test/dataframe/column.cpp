#include "jules/dataframe/column.hpp"
#include "jules/array/array.hpp"
#include "jules/dataframe/numeric.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Column constructor using initializer list", "[dataframe]")
{
  using jules::column;
  using jules::numeric;
  using jules::string;
  using namespace jules::literals;

  struct Toy
  {};

  auto int_column = column{1, 2, 3, 4, 5};
  auto str_column = column{"hello"_s, "world"_s};
  auto toy_column = column{Toy{}, Toy{}};

  CHECK(int_column.elements_type() == typeid(int));
  CHECK(str_column.elements_type() == typeid(string));
  CHECK(toy_column.elements_type() == typeid(Toy));

  CHECK(int_column.can_coerce<numeric>());
  CHECK(int_column.can_coerce<string>());

  CHECK(str_column.can_coerce<numeric>());
  CHECK(str_column.can_coerce<string>());

  CHECK(!toy_column.can_coerce<numeric>());
  CHECK(!toy_column.can_coerce<string>());

  using jules::to_column;
  using jules::to_view;

  auto string_numbers = column{"1.0"_s, "2.4"_s, "3.3"_s};
  auto numeric_numbers = to_column<numeric>(string_numbers);

  auto v = to_view<numeric>(numeric_numbers);

  CHECK(numeric_numbers.size() == v.size());
  CHECK(numeric_numbers.size() != 0u);

  CHECK(v[0] == 1.0);
  CHECK(v[1] == 2.4);
  CHECK(v[2] == 3.3);

  auto tmp = std::vector<numeric>{1.0, 2.0, 3.0};
  auto range_column = column(tmp);
  CHECK(range_column.elements_type() == typeid(numeric));
}

// clang-format off
template <typename T, typename U> concept convertible_to_view = requires(T&& t) {
  { jules::to_view<U>(std::forward<T>(t)) };
};
// clang-format on

static_assert(convertible_to_view<jules::column&, int>);
static_assert(convertible_to_view<const jules::column&, int>);
static_assert(!convertible_to_view<jules::column, int>);
static_assert(!convertible_to_view<jules::column&&, int>);
static_assert(!convertible_to_view<const jules::column&&, int>);

TEST_CASE("Column constructor inference", "[dataframe]")
{
  using jules::column;
  using jules::numeric;
  using jules::string;
  using namespace jules::literals;

  auto check_column = [](const column& col, const auto& value) { CHECK(col.elements_type() == value); };

  check_column({1, 2, 3}, typeid(int));
  check_column({1.0, 2.0, 3.0, 1.0}, typeid(numeric));
  check_column({"1.0"_s, "2.0"_s, "3.0"_s, "1.0"_s}, typeid(string));
}

TEST_CASE("Temporary columns", "[dataframe]")
{
  using jules::numeric;

  auto col = jules::column{1, 2, 3, 4, 5};
  auto c = jules::to_column<numeric>(std::move(col));
  auto view = jules::to_view<numeric>(c);

  for (auto i = 0u; i < col.size(); ++i)
    CHECK(view[i] == i + 1);
}

TEST_CASE("Column to_view vs as_vector", "[dataframe]")
{
  auto col = jules::column{{0, 1, 2, 3, 4, 5}};

  auto view1 = jules::to_view<int>(col);
  auto view2 = jules::to_view<int>(col);
  auto vector = jules::to_vector<int>(col);

  for (auto i : jules::indices(col.size()))
    view1[i] = -1;

  CHECK(all(view2 == -1));
  CHECK(all(vector == jules::cat(0, 1, 2, 3, 4, 5)));
}

TEST_CASE("Column tutorial", "[dataframe]")
{
  auto empty_column = jules::column();

  CHECK(empty_column.size() == 0u);
  CHECK_FALSE(empty_column.can_coerce<jules::numeric>());

  CHECK_THROWS(empty_column.elements_type());
  CHECK_THROWS(empty_column.data<void*>());
  CHECK_THROWS(empty_column.coerce<jules::numeric>());

  auto from_initializer_list = jules::column{1, 2, 3, 4};
  CHECK(from_initializer_list.size() == 4u);
  CHECK(from_initializer_list.elements_type() == typeid(int));

  auto repeated_value = jules::column(0.0, 20u);
  CHECK(repeated_value.size() == 20u);
  CHECK(repeated_value.elements_type() == typeid(double));

  auto x = jules::vector<long>{1L, 2L, 3L};

  auto from_range = jules::column(x);
  CHECK(from_range.size() == x.size());
  CHECK(from_range.elements_type() == typeid(decltype(x)::value_type));
  CHECK(all(jules::to_view<long>(from_range) == x));

  auto from_iterators = jules::column(x.begin(), x.end());
  CHECK(from_iterators.size() == x.size());
  CHECK(from_iterators.elements_type() == typeid(decltype(x)::value_type));
  CHECK(all(jules::to_view<long>(from_iterators) == x));

  auto a = jules::column(0.0f, 5u);
  auto b = a;
  auto c = std::move(b);

  CHECK(b.size() == 0u);
  CHECK(c.size() == a.size());
  CHECK(c.elements_type() == a.elements_type());

  b = a;
  c = std::move(a);

  CHECK(a.size() == 0u);
  CHECK(b.size() == c.size());
  CHECK(b.elements_type() == c.elements_type());

  // View with different type
  (void)jules::to_view<double>(b);
  // b now holds double
  CHECK(b.elements_type() == typeid(double));

  // To vector, on the other hand, doesn't convert
  auto fcopy = jules::to_vector<jules::string>(b);
  // b still holds double
  CHECK(b.elements_type() == typeid(double));
}
