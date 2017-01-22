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
