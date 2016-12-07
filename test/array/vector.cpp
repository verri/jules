#include "jules/array/all.hpp"

#include <functional>
#include <iostream> // TODO
#include <type_traits>

#include <catch.hpp>

template <typename T, typename U> constexpr auto same_type(const T&, const U&) { return std::is_same<T, U>::value; }
template <typename T, typename U> constexpr auto is(const U&) { return std::is_same<T, U>::value; }

TEST_CASE("Vector tutorial", "[vector]")
{
  SECTION("Constructors")
  {
    // Default constructor.
    auto empty = jules::vector<>();
    CHECK(empty.size() == 0u);
    CHECK(empty.data() == nullptr);
    CHECK(empty.begin() == empty.end());
    CHECK(empty.cbegin() == empty.cend());

    // Constructors with size and optional default value.
    auto all_same1 = jules::vector<long>(3l, 10u);
    auto all_same2 = jules::vector<long>(10u);
    all_same2() = 3l;

    CHECK(jules::all(all_same1 == 3l));
    CHECK(jules::all(all_same2 == 3l));
    CHECK(jules::all(all_same1 == all_same2));

    auto values = std::array<long, 10>{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    auto a = jules::vector<long>(10u);
    auto b = jules::vector<long>(10u);

    jules::range::copy(values, a.begin());
    b() = a;

    // Constructors from iterators and ranges.
    auto c = jules::vector<long>(std::begin(values), std::end(values));
    auto d = jules::vector<long>(values);

    // Copy and move constructors.
    auto e = [d]() -> jules::vector<long> { return {std::move(d)}; }();

    REQUIRE(jules::all(a == b));
    REQUIRE(jules::all(b == c));
    REQUIRE(jules::all(c == d));
    REQUIRE(jules::all(d == e));

    // Constructors from slicing.
    auto x = jules::vector<long>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto even = jules::vector<long>(x(jules::slice(0u, 0u, 2u)));
    auto odd = jules::vector<long>(x(jules::seq(1u, 10u, 2u)));

    CHECK(jules::all(x == jules::as_vector(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)));
    CHECK(jules::all(x == jules::as_vector(values)));
    CHECK(jules::all(even == jules::as_vector(0, 2, 4, 6, 8)));
    CHECK(jules::all(odd == jules::as_vector(1, 3, 5, 7, 9)));

    // Constructors from expression.
    auto y = jules::vector<long>(x + x(jules::seq(10u, 0u) - 1u));
    CHECK(jules::all(y == 9));
  }
}
