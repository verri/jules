#include "jules/dataframe/detail/column_model.hpp"

#include <algorithm>
#include <catch.hpp>
#include <iterator>
#include <numeric>

TEST_CASE("Column model", "[dataframe]")
{
  using namespace jules;
  using namespace jules::detail;

  using column_interface_ptr = std::unique_ptr<column_interface<coercion_rules>>;

  column_interface_ptr icolumn{new column_model<int, coercion_rules>};
  column_interface_ptr dcolumn{new column_model<numeric, coercion_rules>};

  CHECK(icolumn->elements_type() == typeid(int));
  CHECK(dcolumn->elements_type() == typeid(numeric));
  CHECK(dcolumn->elements_type() != typeid(int));
  CHECK(icolumn->elements_type() != typeid(numeric));
  CHECK(icolumn->elements_type() != typeid(index_t));

  CHECK(icolumn->can_coerce<int>());
  CHECK(icolumn->can_coerce<numeric>());
  CHECK(icolumn->can_coerce<unsigned int>());
  CHECK(icolumn->can_coerce<string>());
  CHECK(icolumn->can_coerce<index_t>());

  CHECK(dcolumn->can_coerce<int>());
  CHECK(dcolumn->can_coerce<numeric>());
  CHECK(dcolumn->can_coerce<index_t>());
  CHECK(dcolumn->can_coerce<string>());

  auto& ivec = icolumn->downcast<int>();
  ivec.resize(10);
  std::iota(ivec.begin(), ivec.end(), 0);

  auto& dvec = dcolumn->downcast<numeric>();
  std::transform(ivec.begin(), ivec.end(), std::back_inserter(dvec), [](int x) { return 3.1415 * x; });

  std::vector<string> expected{"0.000000",  "3.141500",  "6.283000",  "9.424500",  "12.566000",
                               "15.707500", "18.849000", "21.990500", "25.132000", "28.273500"};

  auto scolumn = dcolumn->coerce<string>();
  auto& svec = scolumn->downcast<string>();

  auto check = std::mismatch(svec.begin(), svec.end(), expected.begin());
  CHECK(check.first == svec.end());
  CHECK(check.second == expected.end());

  auto back_to_dcolumn = scolumn->coerce<numeric>();
  auto& back_to_dvec = back_to_dcolumn->downcast<numeric>();

  auto check_back = std::mismatch(dvec.begin(), dvec.end(), back_to_dvec.begin(), [](auto x, auto y) { return x == Approx(y); });
  CHECK(check_back.first == dvec.end());
  CHECK(check_back.second == back_to_dvec.end());

  CHECK_THROWS_AS(scolumn->downcast<int>(), std::bad_cast);
  CHECK_NOTHROW(scolumn->downcast<string>());

  auto clone = scolumn->clone();
  CHECK(clone->elements_type() == scolumn->elements_type());
}

TEST_CASE("Column model sanity conversions", "[dataframe]")
{
  using namespace jules;
  using namespace jules::detail;
  using namespace std::string_literals;
  using column_ptr = std::unique_ptr<column_interface<coercion_rules>>;

  // Default types: string, numeric, integer, uinteger, index_t

  struct Foo {
    operator string() const { return "foo"; }
    operator double() const { return 0.0; }
  };

  struct Bar {
  };

  const auto str_column = column_ptr{new column_model<string, coercion_rules>{"1"s, "-1"s}};
  const auto num_column = column_ptr{new column_model<numeric, coercion_rules>};
  const auto ind_column = column_ptr{new column_model<index_t, coercion_rules>};
  const auto int_column = column_ptr{new column_model<integer, coercion_rules>};
  const auto uns_column = column_ptr{new column_model<uinteger, coercion_rules>};
  const auto foo_column = column_ptr{new column_model<Foo, coercion_rules>};
  const auto bar_column = column_ptr{new column_model<Bar, coercion_rules>};
  const auto flt_column = column_ptr{new column_model<float, coercion_rules>{1.0f, -1.0f}};

  // String:
  //    From everything that works with std::to_string
  //    From everything implictly converted to string
  //    To every default type
  //== from ==//
  CHECK(str_column->can_coerce<string>());
  CHECK(num_column->can_coerce<string>());
  CHECK(ind_column->can_coerce<string>());
  CHECK(int_column->can_coerce<string>());
  CHECK(uns_column->can_coerce<string>());
  CHECK(foo_column->can_coerce<string>());
  CHECK(!bar_column->can_coerce<string>());
  CHECK(flt_column->can_coerce<string>());
  //== to ==//
  CHECK(str_column->can_coerce<string>());
  CHECK(str_column->can_coerce<numeric>());
  CHECK(str_column->can_coerce<index_t>());
  CHECK(str_column->can_coerce<integer>());
  CHECK(str_column->can_coerce<uinteger>());
  CHECK(!str_column->can_coerce<Foo>());
  CHECK(!str_column->can_coerce<Bar>());
  CHECK(!str_column->can_coerce<float>());

  // Numeric:
  //    From string
  //    From everything implictly converted to double
  //    To every default type
  //== from ==//
  CHECK(str_column->can_coerce<numeric>());
  CHECK(num_column->can_coerce<numeric>());
  CHECK(ind_column->can_coerce<numeric>());
  CHECK(int_column->can_coerce<numeric>());
  CHECK(uns_column->can_coerce<numeric>());
  CHECK(foo_column->can_coerce<numeric>());
  CHECK(!bar_column->can_coerce<numeric>());
  CHECK(flt_column->can_coerce<numeric>());
  //== to ==//
  CHECK(num_column->can_coerce<string>());
  CHECK(num_column->can_coerce<numeric>());
  CHECK(num_column->can_coerce<index_t>());
  CHECK(num_column->can_coerce<integer>());
  CHECK(num_column->can_coerce<uinteger>());
  CHECK(!num_column->can_coerce<Foo>());
  CHECK(!num_column->can_coerce<Bar>());
  CHECK(!num_column->can_coerce<float>());

  // Index:
  //    From string
  //    From everything implictly converted to std::size_t
  //      If signed, the value must be positive.
  //    To every default type
  //== from ==//
  CHECK(str_column->can_coerce<index_t>());
  CHECK(num_column->can_coerce<index_t>());
  CHECK(ind_column->can_coerce<index_t>());
  CHECK(int_column->can_coerce<index_t>());
  CHECK(uns_column->can_coerce<index_t>());
  CHECK(foo_column->can_coerce<index_t>());
  CHECK(!bar_column->can_coerce<index_t>());
  CHECK(flt_column->can_coerce<index_t>());
  //== to ==//
  CHECK(ind_column->can_coerce<string>());
  CHECK(ind_column->can_coerce<numeric>());
  CHECK(ind_column->can_coerce<index_t>());
  CHECK(ind_column->can_coerce<integer>());
  CHECK(ind_column->can_coerce<uinteger>());
  CHECK(!ind_column->can_coerce<Foo>());
  CHECK(!ind_column->can_coerce<Bar>());
  CHECK(!ind_column->can_coerce<float>());

  // TODO: improve coerce_from so it doesn't accept the string "-1"
  //       also check if the whole string was read
  // CHECK_THROWS(str_column->coerce<index_t>());
  CHECK_THROWS(flt_column->coerce<index_t>());
}

TEST_CASE("Column model successful coercions", "[dataframe]")
{
  using namespace jules;
  using namespace jules::detail;
  using column_ptr = std::unique_ptr<column_interface<coercion_rules>>;

  struct Foo {
    operator string() const { return "17"; }
  };

  const auto foo_column = column_ptr{new column_model<Foo, coercion_rules>(1u)};
  const auto int_column1 = column_ptr{new column_model<integer, coercion_rules>{0, -1}};
  const auto str_column1 = column_ptr{new column_model<string, coercion_rules>{"18446744073709551615"}};

  CHECK_THROWS(int_column1->coerce<index_t>());
  CHECK_THROWS(int_column1->coerce<uinteger>());
  CHECK_THROWS(str_column1->coerce<uinteger>());

  const auto str_column2 = foo_column->coerce<string>();
  const auto int_column2 = str_column2->coerce<integer>();
  const auto uns_column = str_column2->coerce<uinteger>();

  const auto ind_column1 = str_column2->coerce<index_t>();
  const auto ind_column2 = int_column2->coerce<index_t>();
  const auto ind_column3 = uns_column->coerce<index_t>();

  const auto uns_column1 = str_column2->coerce<uinteger>();
  const auto uns_column2 = int_column2->coerce<uinteger>();
  const auto uns_column3 = ind_column1->coerce<uinteger>();

  CHECK(str_column2->downcast<string>().at(0u) == "17");
  CHECK(ind_column1->downcast<index_t>().at(0u) == 17u);
  CHECK(ind_column2->downcast<index_t>().at(0u) == 17u);
  CHECK(ind_column3->downcast<index_t>().at(0u) == 17u);
  CHECK(uns_column1->downcast<uinteger>().at(0u) == 17u);
  CHECK(uns_column2->downcast<uinteger>().at(0u) == 17u);
  CHECK(uns_column3->downcast<uinteger>().at(0u) == 17u);
}

TEST_CASE("Column model partial clone and resize", "[dataframe]")
{
  using namespace jules;
  using namespace jules::detail;
  using column_ptr = std::unique_ptr<column_interface<coercion_rules>>;

  auto column = column_ptr{new column_model<integer, coercion_rules>{0, 1, 2, 3, 4, 5}};

  CHECK(column->size() == 6u);
  CHECK(column->clone()->size() == 6u);

  CHECK_THROWS(column->partial_clone(0u, 7u));
  CHECK_THROWS(column->partial_clone(4u, 3u));

  const auto partial = column->partial_clone(2u, 4u);

  CHECK(partial->size() == 4u);

  const auto& ipartial = partial->downcast<int>();

  CHECK(ipartial[0u] == 2);
  CHECK(ipartial[3u] == 5);

  column->resize(2u);
  column->shrink_to_fit();
  CHECK(column->size() == 2u);
}
