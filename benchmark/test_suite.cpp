#include "benchmark.hpp"
#include "jules/array/all.hpp"

#include <chrono>
#include <iostream>

constexpr auto N = 10000u;

void f(void*) {}
static volatile auto use = f;

int main()
{
  using jules::slice;
  using jules::seq;

  JULES_DEFER({
    std::cout << "\nExiting." << std::endl;
    Memory::Summary(std::cout);
    Memory::Finalize();
  });

  std::cout << "\nCreating slices." << std::endl;
  Memory::Reset();
  const auto s_even = slice(1u, 0u, 2u);
  const auto s_odd = slice(0u, 0u, 2u);
  const auto s_all = slice();
  Memory::Summary(std::cout);

  std::cout << "\nCreating const vectors of indexes." << std::endl;
  Memory::Reset();
  const auto v_even2 = seq(1u, N * N - 1, 2u);
  const auto v_even = seq(1u, N - 1, 2u);
  const auto v_odd = seq(0u, N - 1, 2u);
  const auto v_all = seq(0u, N - 1);
  Memory::Summary(std::cout);

  // Vector
  std::cout << "\nCreating vector." << std::endl;
  auto vector = PrintInfo([&] { return jules::vector<double>(N * N); });

  std::cout << "\nFilling odd lines using slice." << std::endl;
  PrintInfo([&] { vector(s_odd) = 1.0; });

  std::cout << "\nFilling even lines using indirect indexing." << std::endl;
  PrintInfo([&] { vector(v_even2) = 2.0; });

  if (N <= 10)
    std::cout << "\nVector: " << vector << std::endl;
  use(&vector);

  // Matrix
  std::cout << "\nCreating matrix." << std::endl;
  auto matrix = PrintInfo([&] { return jules::matrix<double>(N, N); });

  std::cout << "\nFilling odd lines using slice." << std::endl;
  PrintInfo([&] { matrix(s_odd, s_all) = 1.0; });

  std::cout << "\nFilling even lines using indirect indexing." << std::endl;
  PrintInfo([&] { matrix(v_even, v_all) = 2.0; });

  if (N <= 10)
    std::cout << "\nMatrix: " << matrix << std::endl;

  std::cout << "\nZeroing..." << std::endl;
  PrintInfo([&] { matrix() = 0.0; });

  std::cout << "\nFast zeroing..." << std::endl;
  PrintInfo([&] { matrix.fill(0.0); });

  std::cout << "\nFilling matrix using mixed indexing." << std::endl;
  PrintInfo([&] { matrix(v_odd, s_odd) = 1.0; });
  PrintInfo([&] { matrix(s_even, v_even) = 2.0; });

  if (N <= 10)
    std::cout << "\nMatrix: " << matrix << std::endl;

  use(&matrix);

  Memory::Reset();

  return 0;
}
