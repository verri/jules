#ifdef NOT_DEFINED
#include "jules/array/blas.hpp"
#include "jules/array/all.hpp"

#include <catch.hpp>

TEST_CASE("Matrix multiplication", "[blas]")
{
  namespace view = jules::range::view;
  auto values = jules::as_vector(view::ints(0) | view::transform([](auto i) -> double { return i; }) | view::take(128u));

  auto a = jules::matrix<>(values.begin(), 8u, 16u);
  auto b = jules::matrix<>(values.begin(), 16u, 8u);

  auto prod = jules::blas::product(a, b);
  auto expected = jules::matrix<>{
    {9920, 25280, 40640, 56000, 71360, 86720, 102080, 117440},  //
    {10040, 25656, 41272, 56888, 72504, 88120, 103736, 119352}, //
    {10160, 26032, 41904, 57776, 73648, 89520, 105392, 121264}, //
    {10280, 26408, 42536, 58664, 74792, 90920, 107048, 123176}, //
    {10400, 26784, 43168, 59552, 75936, 92320, 108704, 125088}, //
    {10520, 27160, 43800, 60440, 77080, 93720, 110360, 127000}, //
    {10640, 27536, 44432, 61328, 78224, 95120, 112016, 128912}, //
    {10760, 27912, 45064, 62216, 79368, 96520, 113672, 130824}, //
  };

  static_assert(std::is_same<decltype(expected), decltype(prod)>::value, "");
  CHECK(all(expected == prod));
}
#endif
