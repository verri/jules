#include "track.hpp"
#include "array/array.hpp"

#define N 10000

#include <iostream>
#include <chrono>

constexpr double ToMB = 1.0 / 1024.0 / 1024.0;

template <typename F>
void PrintHeadUsage(F&& f) {
    Memory::Reset();

    auto start = std::chrono::system_clock::now();
    f();
    auto end = std::chrono::system_clock::now();

    std::cout << (Memory::Size() * ToMB) << " MB in " << Memory::Count() << " allocations ("
              << (Memory::Count() > 0 ? (Memory::Size() * ToMB) / Memory::Count() : 0.0)
              << " MB/alloc)\ntime elapsed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms\n" << std::endl;
}

int main()
{
    jules::matrix<double> matrix(N, N);

    auto indirect_indexes = jules::seq(0, N, 2);
    auto slice_indexes = jules::slice(0, 0, 2);

    auto all = jules::slice();

    PrintHeadUsage([&] {
        std::cout << "Filling even lines with zeros using slice." << std::endl;
        matrix(slice_indexes, all) = 0;
    });

    PrintHeadUsage([&] {
        std::cout << "Filling even lines with zeros using indirect indexing." << std::endl;
        matrix(indirect_indexes, all) = 0;
    });

    Memory::Finalize();

    return 0;
}
