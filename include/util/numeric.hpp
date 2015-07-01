#ifndef JULES_UTIL_NUMERIC_H
#define JULES_UTIL_NUMERIC_H

#include "core/type.hpp"

#include <functional>
#include <numeric>

namespace jules
{
template <typename Range, typename T = typename range_traits<Range>::value_type> T prod(const Range& range)
{
    return std::accumulate(std::begin(range), std::end(range), 1, std::multiplies<T>{});
}

} // namespace jules

#endif // JULES_UTIL_NUMERIC_H
