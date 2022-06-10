#include "jules/core/debug.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Testing unreachable", "[debug]")
{
  using namespace jules;
  CHECK_THROWS(DEBUG_UNREACHABLE(debug::default_module, debug::level::unreachable, ""));
}
