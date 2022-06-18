#include "jules/data/data.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Head and tail of a matrix", "[data]")
{
  // TODO: missing tail
  using namespace jules::action;

  const auto df = jules::data{
    {"a", {1, 2, 3}},       //
    {"b", {1.0, 2.0, 3.0}}, //
    {"c", {1u, 2u, 3u}},    //
  };

  CHECK(df.row_count() == 3u);

  CHECK_THROWS(df | head(10u));

  auto hdf = df | head(2u);

  CHECK(hdf.row_count() == 2u);

  auto h1df = std::move(hdf) | head(1u);

  CHECK_FALSE(hdf);

  CHECK(h1df.row_count() == 1u);
}
