#include "jules/array/array.hpp"

#include <catch.hpp>

TEST_CASE("Proper clean-up after throwing recursive initializer", "[array]")
{
  static std::size_t copy_constructed_count = 0u;
  static std::size_t destroyed_from_copy_count = 0u;
  static bool should_throw = false;

  struct Foo
  {
    Foo() = default;
    Foo(Foo&&) = default;
    Foo(const Foo&) : from_copy{true}
    {
      if (should_throw && copy_constructed_count > 8)
        throw std::runtime_error{"..."};
      ++copy_constructed_count;
    }
    ~Foo()
    {
      if (from_copy)
        ++destroyed_from_copy_count;
    }

  private:
    bool from_copy = false;
  };

  {
    const auto m = jules::matrix<Foo>{{{}, {}, {}}, {{}, {}, {}}};
  }
  CHECK(copy_constructed_count == destroyed_from_copy_count);

  should_throw = true;
  copy_constructed_count = destroyed_from_copy_count = 0;
  try {
    const auto m = jules::matrix<Foo>{{{}, {}, {}}, {{}, {}, {}}};
  } catch (...) {
    should_throw = false;
  }
  CHECK_FALSE(should_throw);
  CHECK(copy_constructed_count == destroyed_from_copy_count);
}
