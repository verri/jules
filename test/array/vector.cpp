#include "jules/array/array.hpp"
#include "jules/core/type.hpp"

#include <catch2/catch_test_macros.hpp>

static void (*volatile not_optimize_away)(void*) = [](void*) {};

TEST_CASE("Vector tutorial", "[array]")
{
  SECTION("Constructors")
  {
    // Non-initialized
    auto fast = jules::vector<>(jules::uninitialized, 20u);
    CHECK(fast.size() == 20u);

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
    auto all_same1 = jules::vector<long>(3L, 10u);
    auto all_same2 = jules::vector<long>(10u);

    using namespace jules::slicing;
    all_same2[every] = 3L;

    CHECK(all(all_same1 == 3L));
    CHECK(all(all_same2 == 3L));
    CHECK(all(all_same1 == all_same2));

    auto values = std::array<long, 10>{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    auto a = jules::vector<long>(10u);
    auto b = jules::vector<long>(10u);

    jules::ranges::copy(values, a.begin());
    b[every] = a;

    // Constructors from iterators and ranges.
    auto c = jules::vector<long>(std::begin(values), std::end(values));
    auto d = jules::vector<long>(values);

    // Constructor from generator.
    using jules::generated;
    auto e = jules::vector<long>(
      generated, [values, i = 0]() mutable { return values[i++]; }, values.size());

    // Copy and move constructors.
    auto f = [d]() mutable -> jules::vector<long> {
      not_optimize_away(&d);
      return {std::move(d)};
    }();

    REQUIRE(all(a == b));
    REQUIRE(all(b == c));
    REQUIRE(all(c == d));
    REQUIRE(all(d == e));
    REQUIRE(all(e == f));

    // Constructor from initializer list.
    auto x = jules::vector<long>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Constructors from slicing.
    auto even = jules::array(x[{0u, every, 2u}]);
    CHECK(length(even) == 5u);

    const auto odd_ix1 = jules::seq(1u, length(x) - 1u, 2u);

    auto odd_fast = jules::array(x[odd_ix1]);
    auto odd_smart = jules::vector<long>(x[odd_ix1]);
    auto odd_ind = jules::vector<long>(x[jules::seq(0u, length(x) - 1u)][jules::seq(1u, length(x) - 1u, 2u)]);

    CHECK(all(x == jules::cat(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)));
    CHECK(all(x == jules::as_vector(values)));
    CHECK(all(x == jules::as_vector(jules::matrix<long>(std::begin(values), 5u, 2u))));
    CHECK(all(even == jules::cat(0, 2, 4, 6, 8)));
    CHECK(all(odd_fast == jules::cat(1, 3, 5, 7, 9)));
    CHECK(all(odd_smart == jules::cat(1, 3, 5, 7, 9)));
    CHECK(all(odd_ind == jules::cat(1, 3, 5, 7, 9)));

    // Constructors from expression.
    auto y = jules::array(x + x[jules::seq(length(x) - 1u, 0u, -1)]);
    CHECK(all(y == 9));
  }

  SECTION("Assignments")
  {
    using namespace jules::slicing;

    auto x = jules::vector<long>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto y = jules::vector<long>(-20L, 20u);
    auto z = jules::vector<long>(-10L, 5u);

    y = x;
    CHECK(all(x == y));

    // No memory allocation.
    x = y;
    CHECK(all(x == y));

    z = std::move(x);
    CHECK(all(y == z));

    x = z[jules::slice(0u, every, 2u)];
    CHECK(all(x == jules::cat(0, 2, 4, 6, 8)));

    x = z[jules::seq(0u, length(z) - 1u, 2u)];
    CHECK(all(x == jules::cat(0, 2, 4, 6, 8)));

    z = x + x;
    CHECK(all(z == jules::cat(0, 4, 8, 12, 16)));

    x = eval(x + x);
    CHECK(all(x == jules::cat(0, 4, 8, 12, 16)));

    x += 1;
    CHECK(all(x == jules::cat(0, 4, 8, 12, 16) + 1));

    // Can be dangerous if rhs depends on lhs.
    x += x + x;
    CHECK(all(x == 3 * (jules::cat(0, 4, 8, 12, 16) + 1)));

    // To solve aliasing, eval is not enough.
    // Use overlap instead.
    overlap(x) = x[jules::seq(length(x) - 1, 0u, -1)];
    CHECK(all(x == 3 * (jules::cat(16, 12, 8, 4, 0) + 1)));

    // This one is tricky. We must guarantee that the Memory of x is not freed before the
    // expression is evaluated.
    x = x[{0, 4}] - x[{0, 4}];
    CHECK(all(x == jules::as_vector(jules::repeat<4>(0))));

    // Every-slicing do not allocate any extra memory.
    x[every] = 10;
    x[every] = x - x;
    CHECK(all(x == jules::as_vector(jules::repeat<4>(0))));

    x[every] = 1;
    CHECK(all(x == 1));
  }

  SECTION("Iterators")
  {
    auto x = jules::vector<long>(5u);
    const auto y = jules::vector<long>(17L, 5u);

    for (auto& value : x)
      value = 42L;
    CHECK(all(x == 42L));

    auto it = x.begin();
    for (auto value : y)
      *it++ = value;
    CHECK(all(x == 17L));

    auto x_it = x.cbegin();
    auto y_it = y.cbegin();
    while (x_it != x.cend() && y_it != y.cend())
      CHECK(*x_it++ == *y_it++);
  }

  SECTION("Length, size, and extents")
  {
    auto x = jules::vector<>(10u);
    CHECK(length(x) == 10u);
    CHECK(x.size() == 10u);
    CHECK((x.dimensions() == std::array<jules::index_t, 1u>{{10u}}));
  }

  SECTION("Subvector assignment")
  {
    auto x = jules::to_vector<long>(jules::seq(0u, 10u - 1u));
    const auto y = x;

    x[jules::slice(0u, jules::every, 2u)] = 0u;
    CHECK(all(x == jules::cat(0, 1, 0, 3, 0, 5, 0, 7, 0, 9)));

    x[jules::slice(1u, jules::every, 2u)] = jules::vector<long>{2, 4, 6, 8, 10};
    CHECK(all(x == jules::cat(0, 2, 0, 4, 0, 6, 0, 8, 0, 10)));

    x[jules::slice(0u, jules::every, 2u)] = y[jules::slice(1u, jules::every, 2u)];
    CHECK(all(x == 1 + jules::to_vector<long>(jules::seq(0u, 10u - 1u))));

    x[jules::every] = 0;
    CHECK(all(x == 0));

    for (const auto i : jules::indices(x.size()))
      x[i] = y[length(y) - 1];
    CHECK(all(x == 9));

    x[jules::seq(0u, x.size() - 1u)] = y[jules::every];
    CHECK(all(x == y));

    x[jules::seq(0u, x.size() - 1u, 2u)] = y[jules::seq(1u, y.size() - 1u, 2u)];
    CHECK(all(x == jules::cat(1, 1, 3, 3, 5, 5, 7, 7, 9, 9)));

    x[jules::seq(1u, x.size() - 1u, 2u)] = jules::vector<long>{2, 4, 6, 8, 10};
    CHECK(all(x == jules::cat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)));
  }
}
