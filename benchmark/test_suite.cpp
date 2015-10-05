#include "track.hpp"
#include "array/array.hpp"

#include <iostream>
#include <chrono>

constexpr std::size_t N = 20000;
constexpr double ToMB = 1.0 / 1024.0 / 1024.0;

template <typename F>
auto PrintHeadUsage(F&& f) {
    Memory::Reset();

    auto start = std::chrono::system_clock::now();
    decltype(auto) result = f();
    auto end = std::chrono::system_clock::now();

    std::cout << (Memory::Size() * ToMB) << " MB in " << Memory::Count() << " allocations ("
              << (Memory::Count() > 0 ? (Memory::Size() * ToMB) / Memory::Count() : 0.0)
              << " MB/alloc)\ntime elapsed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms\n" << std::endl;

    return result;
}

void f(void *) {}
static volatile auto use = f;

int main()
{
    using jules::slice;
    using jules::seq;

    std::cout << "Creating vector." << std::endl;
    auto vector = PrintHeadUsage([&] {
        return jules::vector<double>(N * N);
    });

    std::cout << "Filling even lines with zeros using slice." << std::endl;
    PrintHeadUsage([&] {
        vector(slice(0, 0, 2)) = 1.0;
        return 0;
    });
    use(&vector);

    std::cout << "Filling even lines using indirect indexing." << std::endl;
    PrintHeadUsage([&] {
        vector(seq(0, N * N, 2)) = 2.0;
        return 0;
    });
    use(&vector);

    std::cout << "Creating matrix." << std::endl;
    auto matrix = PrintHeadUsage([&] {
        return jules::matrix<double>(N, N);
    });

    std::cout << "Filling even lines with zeros using slice." << std::endl;
    PrintHeadUsage([&] {
        matrix(slice(0, 0, 2), slice()) = 1.0;
        return 0;
    });
    use(&matrix);

    std::cout << "Filling even lines using indirect indexing." << std::endl;
    PrintHeadUsage([&] {
        matrix(seq(0, N, 2), seq(0, N)) = 2.0;
        return 0;
    });
    use(&matrix);

    Memory::Finalize();

    return 0;
}
