#include "benchmark.hpp"
#include "array/array.hpp"

#include <chrono>

constexpr std::size_t N = 10000;

void f(void*) {}
static volatile auto use = f;

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const jules::detail::ref_ndarray<T, N>& array)
{
    os << "{ ";
    for (auto&& subarray : array)
        os << subarray << ' ';
    return os << '}';
}

int main()
{
    using jules::slice;
    using jules::seq;

    // Vector
    std::cout << "\nCreating vector." << std::endl;
    auto vector = PrintHeapUsage([&] { return jules::vector<double>(N * N); });

    std::cout << "\nFilling odd lines using slice." << std::endl;
    PrintHeapUsage([&] { vector(slice(0, 0, 2)) = 1.0; });

    std::cout << "\nFilling even lines using indirect indexing." << std::endl;
    PrintHeapUsage([&] { vector(seq(1, N * N, 2)) = 2.0; });

    if (N < 5)
        std::cout << "\nVector: " << vector << std::endl;
    use(&vector);

    // Matrix
    std::cout << "\nCreating matrix." << std::endl;
    auto matrix = PrintHeapUsage([&] { return jules::matrix<double>(N, N); });

    std::cout << "\nFilling odd lines using slice." << std::endl;
    PrintHeapUsage([&] { matrix(slice(0, 0, 2), slice()) = 1.0; });

    std::cout << "\nFilling even lines using indirect indexing." << std::endl;
    PrintHeapUsage([&] { matrix(seq(1, N, 2), seq(0, N)) = 2.0; });

    if (N < 5)
        std::cout << "\nMatrix: " << matrix << std::endl;

    std::cout << "\nFilling matrix using mixed indexing." << std::endl;
    PrintHeapUsage([&] { matrix(seq(0, N, 2), slice(0, 0, 2)) = 2.0; });
    PrintHeapUsage([&] { matrix(slice(1, 0, 2), seq(1, N, 2)) = 1.0; });

    if (N < 5)
        std::cout << "\nMatrix: " << matrix << std::endl;

    use(&matrix);

    Memory::Finalize();

    return 0;
}
