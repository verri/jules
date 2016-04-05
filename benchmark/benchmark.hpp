#ifndef JULES_BENCHMARK_BENCHMARK_H
#define JULES_BENCHMARK_BENCHMARK_H

#include <chrono>

#include "track.hpp"

template <typename F, typename = std::enable_if_t<std::is_same<void, std::result_of_t<F()>>::value>> void PrintHeapUsage(F&& f)
{
    Memory::Reset();
    f();
    Memory::Summary(std::cout);
}

template <typename F, typename = std::enable_if_t<!std::is_same<void, std::result_of_t<F()>>::value>> auto PrintHeapUsage(F&& f)
{
    Memory::Reset();
    decltype(auto) result = f();
    Memory::Summary(std::cout);
    return result;
}

template <typename F, typename = std::enable_if_t<std::is_same<void, std::result_of_t<F()>>::value>> void PrintTimeElapsed(F&& f)
{
    using namespace std::chrono;
    auto before = system_clock::now();
    f();
    auto after = system_clock::now();
    std::cerr << "Time elapsed: " << duration_cast<milliseconds>(after - before).count() << "ms" << std::endl;
}

template <typename F, typename = std::enable_if_t<!std::is_same<void, std::result_of_t<F()>>::value>> auto PrintTimeElapsed(F&& f)
{
    using namespace std::chrono;
    auto before = system_clock::now();
    decltype(auto) result = f();
    auto after = system_clock::now();
    std::cerr << "Time elapsed: " << duration_cast<milliseconds>(after - before).count() << "ms" << std::endl;
    return result;
}

template <typename F> auto PrintInfo(F&& f)
{
    return PrintTimeElapsed([&] { return PrintHeapUsage(std::forward<F>(f)); });
}

#endif // JULES_BENCHMARK_BENCHMARK_H
