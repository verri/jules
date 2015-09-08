#ifndef JULES_UTIL_NUMERIC_H
#define JULES_UTIL_NUMERIC_H

#include <functional>
#include <numeric>
#include <utility>

namespace jules
{
template <typename Range> auto prod(const Range& range)
{
    return std::accumulate(std::begin(range), std::end(range), 1,
                           std::multiplies<decltype(*std::begin(range))>{});
}

} // namespace jules

#endif // JULES_UTIL_NUMERIC_H
