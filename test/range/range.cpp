#include "catch.hpp"

#include "jules/range/range.hpp"
#include <vector>

TEST_CASE("range", "[range]")
{
    using vector = std::vector<double>;

    static_assert(std::is_same<jules::range_value_t<vector>, typename vector::iterator::value_type>::value,
                  "");
}
