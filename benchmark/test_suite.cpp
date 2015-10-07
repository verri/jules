#include "track.hpp"
#include "array/array.hpp"

#include <iostream>
#include <chrono>

using vec_t = std::vector<double>;

static_assert(std::is_same<jules::range_iterator_t<vec_t>, typename vec_t::iterator>::value, "");
static_assert(std::is_same<jules::range_iterator_t<vec_t&>, typename vec_t::iterator>::value, "");
static_assert(std::is_same<jules::range_iterator_t<const vec_t>, typename vec_t::const_iterator>::value, "");
static_assert(std::is_same<jules::range_iterator_t<const vec_t&>, typename vec_t::const_iterator>::value, "");

constexpr std::size_t N = 10000;
constexpr double ToMB = 1.0 / 1024.0 / 1024.0;

template <typename F> auto PrintHeapUsage(F&& f)
{
    Memory::Reset();

    auto start = std::chrono::system_clock::now();
    decltype(auto) result = f();
    auto end = std::chrono::system_clock::now();

    auto size = Memory::Size();
    auto count = Memory::Count();

    std::cout << (size * ToMB) << " MB in " << count << " allocations ("
              << (count > 0 ? (size * ToMB) / count : 0.0) << " MB/alloc)\ntime elapsed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n"
              << std::endl;

    return result;
}

void f(void*) {}
static volatile auto use = f;

int main()
{
    using jules::slice;
    using jules::seq;

    std::cout << "Creating vector." << std::endl;
    auto vector = PrintHeapUsage([&] { return jules::vector<double>(N * N); });

    std::cout << "Filling even lines using slice." << std::endl;
    PrintHeapUsage([&] {
        vector(slice(0, 0, 2)) = 1.0;
        return 0;
    });
    use(&vector);

    std::cout << "Filling even lines using indirect indexing." << std::endl;
    PrintHeapUsage([&] {
        vector(seq(0, N * N, 2)) = 2.0;
        return 0;
    });
    use(&vector);

    std::cout << "Creating matrix." << std::endl;
    auto matrix = PrintHeapUsage([&] { return jules::matrix<double>(N, N); });

    std::cout << "Filling even lines using slice." << std::endl;
    PrintHeapUsage([&] {
        matrix(slice(0, 0, 2), slice()) = 1.0;
        return 0;
    });
    use(&matrix);

    std::cout << "Filling even lines using indirect indexing." << std::endl;
    PrintHeapUsage([&] {
        matrix(seq(0, N, 2), seq(0, N)) = 2.0;
        return 0;
    });
    use(&matrix);

    Memory::Finalize();

    return 0;
}
