#include "jules/array/all.hpp"

#include <functional>
#include <type_traits>

#include <catch.hpp>

template <typename T, typename U> constexpr auto same_type(const T&, const U&) { return std::is_same<T, U>::value; }
template <typename T, typename U> constexpr auto is(const U&) { return std::is_same<T, U>::value; }

TEST_CASE("Vector tutorial", "[vector]")
{
  SECTION("Constructors")
  {
    auto empty = jules::vector<>();
    CHECK(empty.size() == 0u);
    CHECK(empty.data() == nullptr);
    CHECK(empty.begin() == empty.end());
    CHECK(empty.cbegin() == empty.cend());

    auto all_same1 = jules::vector<long>(3l, 10u);
    auto all_same2 = jules::vector<long>(10u);
    all_same2() = 3l;

    CHECK(jules::all(all_same1 == 3l));
    CHECK(jules::all(all_same2 == 3l));
    CHECK(jules::all(all_same1 == all_same2));

    auto values = std::array<long, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto a = jules::vector<long>(10u);
    auto b = jules::vector<long>(10u);

    jules::range::copy(values, a.begin());
    b() = a;

    auto c = jules::vector<long>(std::begin(values), std::end(values));
    auto d = jules::vector<long>(values);

    auto e = [d]() -> jules::vector<long> { return {std::move(d)}; }();

    REQUIRE(jules::all(a == b));
    REQUIRE(jules::all(b == c));
    REQUIRE(jules::all(c == d));
    REQUIRE(jules::all(d == e));
  }
}
