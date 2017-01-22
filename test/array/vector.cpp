#include "jules/array/all.hpp"

#include <catch.hpp>

static void (*volatile not_optimize_away)(void*) = [](void*) {};

TEST_CASE("Vector tutorial", "[array]")
{
  SECTION("Constructors")
  {
    // Default constructor.
    auto empty = jules::vector<>();
    CHECK(empty.size() == 0u);
    CHECK(empty.data() == nullptr);
    CHECK(empty.begin() == empty.end());
    CHECK(empty.cbegin() == empty.cend());

    // Uninitialized vs initialized
    auto uninit = jules::vector<>(jules::uninitialized, 10u);
    auto init = jules::vector<>(10u);

    CHECK(all(init == jules::numeric{}));

    // Constructors with size and optional default value.
    auto all_same1 = jules::vector<long>(3l, 10u);
    auto all_same2 = jules::vector<long>(10u);
    all_same2() = 3l;

    CHECK(all(all_same1 == 3l));
    CHECK(all(all_same2 == 3l));
    CHECK(all(all_same1 == all_same2));

    auto values = std::array<long, 10>{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    auto a = jules::vector<long>(10u);
    auto b = jules::vector<long>(10u);

    jules::range::copy(values, a.begin());
    b() = a;

    // Constructors from iterators and ranges.
    auto c = jules::vector<long>(std::begin(values), std::end(values));
    auto d = jules::vector<long>(values);

    // Copy and move constructors.
    auto e = [d]() mutable -> jules::vector<long> {
      not_optimize_away(&d);
      return {std::move(d)};
    }();

    REQUIRE(all(a == b));
    REQUIRE(all(b == c));
    REQUIRE(all(c == d));
    REQUIRE(all(d == e));

    // Constructor from initializer list.
    auto x = jules::vector<long>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Constructors from slicing.
    auto even = jules::vector<long>(x(jules::slice(0u, jules::slice::all, 2u)));

    auto odd_ix1 = jules::seq(1u, x.length() - 1u, 2u);
    auto odd_ix2 = odd_ix1;

    auto odd_slow = jules::vector<long>(x(odd_ix1));
    auto odd_smart = jules::vector<long>(x()(std::move(odd_ix1)));
    auto odd_fast = jules::vector<long>(x(std::move(odd_ix2)));
    auto odd_ind = jules::vector<long>(x(jules::seq(0u, x.length() - 1u))(jules::seq(1u, x.length() - 1u, 2u)));

    CHECK(all(x == jules::as_vector(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)));
    CHECK(all(x == jules::as_vector(values)));
    CHECK(all(x == jules::as_vector(jules::matrix<long>(std::begin(values), 5u, 2u))));
    CHECK(all(even == jules::as_vector(0, 2, 4, 6, 8)));
    CHECK(all(odd_slow == jules::as_vector(1, 3, 5, 7, 9)));
    CHECK(all(odd_smart == jules::as_vector(1, 3, 5, 7, 9)));
    CHECK(all(odd_fast == jules::as_vector(1, 3, 5, 7, 9)));
    CHECK(all(odd_ind == jules::as_vector(1, 3, 5, 7, 9)));

    // Constructors from expression.
    auto y = jules::vector<long>(x + x(jules::seq(x.length() - 1u, 0u, -1)));
    CHECK(all(y == 9));
  }

  SECTION("Assignments")
  {
    auto x = jules::vector<long>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto y = jules::vector<long>(-20l, 20u);
    auto z = jules::vector<long>(-10l, 5u);

    y = x;
    CHECK(all(x == y));

    z = std::move(x);
    CHECK(all(y == z));

    x = z(jules::slice(0u, jules::slice::all, 2u));
    CHECK(all(x == jules::as_vector(0, 2, 4, 6, 8)));

    x = z(jules::seq(0u, z.length() - 1u, 2u));
    CHECK(all(x == jules::as_vector(0, 2, 4, 6, 8)));

    z = x + x;
    CHECK(all(z == jules::as_vector(0, 4, 8, 12, 16)));

    x = eval(x + x);
    CHECK(all(x == jules::as_vector(0, 4, 8, 12, 16)));

    // This one is tricky. We must guarantee that the Memory of x is not freed before the
    // expression is evaluated.
    x = x - x;
    CHECK(all(x == jules::as_vector(jules::repeat<5>(0))));

    x.fill(1);
    CHECK(all(x == 1));
  }

  SECTION("Iterators")
  {
    auto x = jules::vector<long>(5u);
    const auto y = jules::vector<long>(17l, 5u);

    for (auto& value : x)
      value = 42l;
    CHECK(all(x == 42l));

    auto it = x.begin();
    for (auto value : y)
      *it++ = value;
    CHECK(all(x == 17l));

    auto x_it = x.cbegin();
    auto y_it = y.cbegin();
    while (x_it != x.cend() && y_it != y.cend())
      CHECK(*x_it++ == *y_it++);
  }

  SECTION("Length, size, and extents")
  {
    auto x = jules::vector<>(10u);
    CHECK(x.length() == 10u);
    CHECK(x.size() == 10u);
    CHECK(x.extents() == x.size());
  }

  SECTION("Subvector assignment")
  {
    auto x = jules::to_vector<long>(jules::seq(0u, 10u - 1u));
    const auto y = x;

    x(jules::slice(0u, jules::slice::all, 2u)) = 0u;
    CHECK(all(x == jules::as_vector(0, 1, 0, 3, 0, 5, 0, 7, 0, 9)));

    x(jules::slice(1u, jules::slice::all, 2u)) = jules::vector<long>{2, 4, 6, 8, 10};
    CHECK(all(x == jules::as_vector(0, 2, 0, 4, 0, 6, 0, 8, 0, 10)));

    x(jules::slice(0u, jules::slice::all, 2u)) = y(jules::slice(1u, jules::slice::all, 2u));
    CHECK(all(x == 1 + jules::to_vector<long>(jules::seq(0u, 10u - 1u))));

    x() = 0;
    CHECK(all(x == 0));

    for (auto i = 0u; i < x.size(); ++i)
      x(i) = y(y.length() - 1);
    CHECK(all(x == 9));

    x(jules::seq(0u, x.size() - 1u)) = y();
    CHECK(all(x == y));

    x(jules::seq(0u, x.size() - 1u, 2u)) = y(jules::seq(1u, y.size() - 1u, 2u));
    CHECK(all(x == jules::as_vector(1, 1, 3, 3, 5, 5, 7, 7, 9, 9)));

    x(jules::seq(1u, x.size() - 1u, 2u)) = jules::vector<long>{2, 4, 6, 8, 10};
    CHECK(all(x == jules::as_vector(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)));
  }
}
