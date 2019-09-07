#include "jules/base/numeric.hpp"
#include "jules/array/array.hpp"

#include <array>
#include <vector>

#include <catch.hpp>

TEST_CASE("Base numeric utilities", "[numeric]")
{
  using namespace jules;

  SECTION("Repeat")
  {
    auto a = repeat<10>(2);
    auto b = repeat(10u, 2);

    CHECK(all(as_vector(a) == 2));
    CHECK(all(as_vector(b) == 2));
    CHECK(all(as_vector(a) == as_vector(b)));

    // trick index_t repeated
    auto c = repeat(10u, 0u);
    CHECK(c.size() == 10u);
    CHECK(all(as_vector(c) == 0u));
  }
  SECTION("Arithmetic")
  {
    const auto a = std::vector<long>{1, 2, 3, 4};
    const auto b = std::array<long, 4>{{1, 2, 3, 4}};
    const auto c = vector<long>{1, 2, 3, 4};

    CHECK(sum(a.begin(), a.end()) == 10);
    CHECK(sum(b) == 10);
    CHECK(sum(c) == 10);

    CHECK(mean(a.begin(), a.end()) == 10 / 4);
    CHECK(mean(b) == 10 / 4);
    CHECK(mean(c) == 10 / 4);

    CHECK(var(a.begin(), a.end()) == 6 / 3);
    CHECK(var(b) == 6 / 3);
    CHECK(var(c) == 6 / 3);

    CHECK(sd(a.begin(), a.end()) == Approx(std::sqrt(6 / 3)));
    CHECK(sd(b) == Approx(std::sqrt(6 / 3)));
    CHECK(sd(c) == Approx(std::sqrt(6 / 3)));

    CHECK(std::tuple(mean(c), sd(c)) == meansd(c));

    CHECK(prod(a.begin(), a.end()) == 24);
    CHECK(prod(b) == 24);
    CHECK(prod(c) == 24);

    // XXX: how to solve conflict with std::min?
    CHECK(jules::min(a.begin(), a.end()) == 1);
    CHECK(min(b) == 1);
    CHECK(min(c) == 1);

    CHECK(which_min(a.begin(), a.end()) == 0u);
    CHECK(which_min(b) == 0u);
    CHECK(which_min(c) == 0u);

    // XXX: how to solve conflict with std::min?
    CHECK(jules::max(a.begin(), a.end()) == 4);
    CHECK(max(b) == 4);
    CHECK(max(c) == 4);

    CHECK(which_max(a.begin(), a.end()) == 3u);
    CHECK(which_max(b) == 3u);
    CHECK(which_max(c) == 3u);

    CHECK(nth(a.begin(), a.end(), 1) == 2);
    CHECK(nth(b, 1) == 2);
    CHECK(nth(c, 1) == 2);

    // Limits
    CHECK(sum(std::array<unsigned, 0>{}) == 0u);
    CHECK(prod(std::array<unsigned, 0>{}) == 1u);
    CHECK(min(std::array<unsigned, 0>{}) == static_cast<unsigned>(-1));
    CHECK(max(std::array<unsigned, 0>{}) == 0u);

    CHECK(sum(std::array<double, 0>{}) == 0.0);
    CHECK(prod(std::array<double, 0>{}) == 1.0);
    CHECK(min(std::array<double, 0>{}) == std::numeric_limits<double>::infinity());
    CHECK(max(std::array<double, 0>{}) == -std::numeric_limits<double>::infinity());

    // constexpr prod
    constexpr auto a0 = std::array<int, 0>{};
    constexpr auto a1 = std::array<int, 1>{{0}};
    constexpr auto a2 = std::array<int, 2>{{3, 4}};

    static_assert(prod(a0) == 1);
    static_assert(prod(a1) == 0);
    static_assert(prod(a2) == 12);

    static_assert(noexcept(prod(a0)));
    static_assert(noexcept(prod(a1)));
    static_assert(noexcept(prod(a2)));
  }
  SECTION("Logical")
  {
    const auto a = vector<bool>{true, false, true};
    const auto b = vector<bool>{true, true, true};

    CHECK(count(a.begin(), a.end()) == 2u);
    CHECK(count(b) == 3u);

    CHECK(freq(a.begin(), a.end()) == Approx(2.0 / 3.0));
    CHECK(freq(b) == Approx(1.0));

    CHECK(any(a.begin(), a.end()));
    CHECK(any(!a));
    CHECK(!any(!b));

    CHECK(!all(a.begin(), a.end()));
    CHECK(all(b));

    CHECK(!none(a.begin(), a.end()));
    CHECK(none(!b));

    const auto aix = which(a);
    const auto bix = which(b.begin(), b.end());
    const auto cix = which(!b);

    CHECK(aix.size() == count(a));
    CHECK(bix.size() == count(b));
    CHECK(cix.size() == 0u);

    CHECK(all(as_vector(aix) == cat(0u, 2u)));
    CHECK(all(as_vector(bix) == cat(0u, 1u, 2u)));
  }
}
