#include "jules/base/numeric.hpp"
#include "jules/array/all.hpp"

#include <array>
#include <vector>

#include <catch.hpp>

TEST_CASE("Base numeric utilities", "[numeric]")
{
  SECTION("Repeat")
  {
    auto a = jules::repeat<10>(2);
    auto b = jules::repeat(10u, 2);

    CHECK(jules::length(a) == jules::length(b));
    CHECK(all(jules::as_vector(a) == 2));
    CHECK(all(jules::as_vector(b) == 2));
    CHECK(all(jules::as_vector(a) == jules::as_vector(b)));

    // trick index_t repeated
    auto c = jules::repeat(10u, 0u);
    CHECK(c.size() == 10u);
    CHECK(all(jules::as_vector(c) == 0u));
  }
  SECTION("Arithmetic")
  {
    const auto a = std::vector<long>{1, 2, 3, 4};
    const auto b = std::array<long, 4>{{1, 2, 3, 4}};
    const auto c = jules::vector<long>{1, 2, 3, 4};

    CHECK(jules::length(a.begin(), a.end()) == a.size());
    CHECK(jules::length(b) == b.size());
    CHECK(length(c) == c.length());

    CHECK(jules::sum(a.begin(), a.end()) == 10);
    CHECK(jules::sum(b) == 10);
    CHECK(sum(c) == 10);

    CHECK(jules::mean(a.begin(), a.end()) == 10 / 4);
    CHECK(jules::mean(b) == 10 / 4);
    CHECK(mean(c) == 10 / 4);

    CHECK(jules::prod(a.begin(), a.end()) == 24);
    CHECK(jules::prod(b) == 24);
    CHECK(prod(c) == 24);

    CHECK(jules::min(a.begin(), a.end()) == 1);
    CHECK(jules::min(b) == 1);
    CHECK(min(c) == 1);

    CHECK(jules::max(a.begin(), a.end()) == 4);
    CHECK(jules::max(b) == 4);
    CHECK(max(c) == 4);

    // Limits
    CHECK(jules::sum(std::array<unsigned, 0>{}) == 0u);
    CHECK(jules::prod(std::array<unsigned, 0>{}) == 1u);
    CHECK(jules::min(std::array<unsigned, 0>{}) == static_cast<unsigned>(-1));
    CHECK(jules::max(std::array<unsigned, 0>{}) == 0u);

    CHECK(jules::sum(std::array<double, 0>{}) == 0.0);
    CHECK(jules::prod(std::array<double, 0>{}) == 1.0);
    CHECK(jules::min(std::array<double, 0>{}) == std::numeric_limits<double>::infinity());
    CHECK(jules::max(std::array<double, 0>{}) == -std::numeric_limits<double>::infinity());
  }
  SECTION("Logical")
  {
    const auto a = jules::vector<bool>{true, false, true};
    const auto b = jules::vector<bool>{true, true, true};

    CHECK(jules::count(a.begin(), a.end()) == 2u);
    CHECK(count(b) == 3u);

    CHECK(jules::freq(a.begin(), a.end()) == Approx(2.0 / 3.0));
    CHECK(freq(b) == Approx(1.0));

    CHECK(jules::any(a.begin(), a.end()));
    CHECK(any(!a));
    CHECK(!any(!b));

    CHECK(!jules::all(a.begin(), a.end()));
    CHECK(all(b));

    CHECK(!jules::none(a.begin(), a.end()));
    CHECK(none(!b));

    const auto aix = which(a);
    const auto bix = jules::which(b.begin(), b.end());
    const auto cix = which(!b);

    CHECK(aix.size() == count(a));
    CHECK(bix.size() == count(b));
    CHECK(cix.size() == 0u);

    CHECK(all(jules::as_vector(aix) == jules::as_vector(0u, 2u)));
    CHECK(all(jules::as_vector(bix) == jules::as_vector(0u, 1u, 2u)));
  }
}
