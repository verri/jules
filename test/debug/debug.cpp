#include "jules/core/debug.hpp"

#include <catch.hpp>

TEST_CASE("Testing unreachable", "[debug]")
{
  using namespace jules;
  CHECK_THROWS(DEBUG_UNREACHABLE(debug::default_module, debug::level::unreachable, ""));
}
