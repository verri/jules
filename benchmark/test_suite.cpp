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

  // Vector
  std::cout << "\nCreating vector." << std::endl;
  auto vector = PrintInfo([&] { return jules::vector<double>(N * N); });

  std::cout << "\nFilling odd lines using slice." << std::endl;
  PrintInfo([&] { vector(slice(0u, 0u, 2u)) = 1.0; });

  std::cout << "\nFilling even lines using indirect indexing." << std::endl;
  PrintInfo([&] { vector(seq(1u, N * N, 2u)) = 2.0; });

  if (N <= 10)
    std::cout << "\nVector: " << vector << std::endl;
  use(&vector);

  // Matrix
  std::cout << "\nCreating matrix." << std::endl;
  auto matrix = PrintInfo([&] { return jules::matrix<double>(N, N); });

  std::cout << "\nFilling odd lines using slice." << std::endl;
  PrintInfo([&] { matrix(slice(0, 0, 2), slice()) = 1.0; });

  std::cout << "\nFilling even lines using indirect indexing." << std::endl;
  PrintInfo([&] { matrix(seq(1, N, 2), seq(0u, N)) = 2.0; });

  if (N <= 10)
    std::cout << "\nMatrix: " << matrix << std::endl;

  std::cout << "\nZeroing..." << std::endl;
  PrintInfo([&] { matrix() = 0.0; });

  std::cout << "\nFilling matrix using mixed indexing." << std::endl;
  PrintInfo([&] { matrix(seq(0u, N, 2u), slice(0u, 0u, 2u)) = 1.0; });
  PrintInfo([&] { matrix(slice(1u, 0u, 2u), seq(1u, N, 2u)) = 2.0; });

  if (N <= 10)
    std::cout << "\nMatrix: " << matrix << std::endl;

  use(&matrix);

  Memory::Finalize();

  return 0;
}
