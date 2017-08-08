#include "jules/array/builder.hpp"

#include <catch.hpp>

TEST_CASE("Array builder", "[array]")
{
  struct Foo {
    Foo(int, char, double) : ptr{new int[10u]} {}
    ~Foo() { delete[] ptr; }
    int *ptr;
  };

  auto b = jules::array_builder<Foo, 1>(std::array<jules::index_t, 1u>{{3u}});
  b.emplace_back(1, 'a', 1.0);
  auto c = std::move(b);
  // no memory leak, or double destruction.
}
