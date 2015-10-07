#include "benchmark.hpp"
#include "array/array.hpp"

#include <chrono>

constexpr std::size_t N = 10000;

void f(void*) {}
static volatile auto use = f;

int main()
{
    using jules::slice;
    using jules::seq;

    // Vector
    std::cout << "\nCreating vector." << std::endl;
    auto vector = PrintHeapUsage([&] { return jules::vector<double>(N * N); });

    std::cout << "\nFilling even lines using slice." << std::endl;
    PrintHeapUsage([&] { vector(slice(0, 0, 2)) = 1.0; });

    std::cout << "\nFilling even lines using indirect indexing." << std::endl;
    PrintHeapUsage([&] { vector(seq(0, N * N, 2)) = 2.0; });

    use(&vector);

    // Matrix
    std::cout << "\nCreating matrix." << std::endl;
    auto matrix = PrintHeapUsage([&] { return jules::matrix<double>(N, N); });

    std::cout << "\nFilling even lines using slice." << std::endl;
    PrintHeapUsage([&] { matrix(slice(0, 0, 2), slice()) = 1.0; });

    std::cout << "\nFilling even lines using indirect indexing." << std::endl;
    PrintHeapUsage([&] { matrix(seq(0, N, 2), seq(0, N)) = 2.0; });

    use(&matrix);

    // Realloc
    std::cout << "\nCreating std::vector." << std::endl;
    auto tmp = PrintHeapUsage([&] { return std::vector<std::size_t>{}; });

    std::cout << "\nPushing back " << N * N << " elements." << std::endl;
    PrintHeapUsage([&] {
        for (std::size_t i = 0; i < N * N; ++i)
            tmp.push_back(i);
    });
    use(&tmp);

    Memory::Finalize();

    return 0;
}
