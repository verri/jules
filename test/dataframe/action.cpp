#include "jules/dataframe/dataframe.hpp"

#include <catch.hpp>

TEST_CASE("Head and tail of a matrix", "[dataframe]")
{
  // TODO: missing tail
  using namespace jules::action;

  const auto df = jules::dataframe{
    {"a", {1, 2, 3}},       //
    {"b", {1.0, 2.0, 3.0}}, //
    {"c", {1u, 2u, 3u}},    //
  };

  CHECK(df.row_count() == 3u);

  CHECK_THROWS(df | head(10u));

  const auto hdf = df | head(2u);

  CHECK(hdf.row_count() == 2u);
}
