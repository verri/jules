#include "jules/array/array.hpp"

#include <catch.hpp>

TEST_CASE("array", "[array]")
{
  auto vector = jules::vector<>{1.0, 2.0, 3.0};

  auto matrix = jules::matrix<>{
    {1.0, 2.0, 3.0}, //
    {4.0, 5.0, 6.0}, //
    {7.0, 8.0, 9.0}, //
  };
}
