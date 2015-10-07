#ifndef JULES_BENCHMARK_BENCHMARK_H
#define JULES_BENCHMARK_BENCHMARK_H

#include "track.hpp"

template <typename F, typename = std::enable_if_t<std::is_same<void, decltype(std::declval<F>()())>::value>>
void PrintHeapUsage(F&& f)
{
    Memory::Reset();
    f();
    Memory::Summary(std::cout);
}

template <typename F, typename = std::enable_if_t<!std::is_same<void, decltype(std::declval<F>()())>::value>>
auto PrintHeapUsage(F&& f)
{
    Memory::Reset();
    decltype(auto) result = f();
    Memory::Summary(std::cout);
    return result;
}

#endif // JULES_BENCHMARK_BENCHMARK_H
