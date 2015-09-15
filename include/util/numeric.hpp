#ifndef JULES_UTIL_NUMERIC_H
#define JULES_UTIL_NUMERIC_H

#include "range/range.hpp"

#include <functional>
#include <numeric>
#include <type_traits>
#include <utility>

namespace jules
{
template <typename Range, typename R = typename std::remove_reference<Range>::type::value_type>
auto prod(Range&& rng)
{
    static_assert(std::is_arithmetic<R>::value, "value type must be arithmetic");
    return std::accumulate(range::begin(std::forward<Range>(rng)), range::end(std::forward<Range>(rng)), 1,
                           std::multiplies<R>{});
}

constexpr auto prod_args() { return 1; }
template <typename Head, typename... Tail> constexpr auto prod_args(const Head& head, const Tail&... tail)
{
    return head * prod_args(tail...);
}

template <typename Range, typename R = typename std::remove_reference<Range>::type::value_type>
auto all(Range&& rng)
{
    static_assert(std::is_convertible<R, bool>::value, "value type must be convertible to boolean");
    return std::accumulate(range::begin(std::forward<Range>(rng)), range::end(std::forward<Range>(rng)), true,
                           [](auto&& acc, auto&& value) { return acc && value; });
}

template <typename Range1, typename R1 = typename std::remove_reference<Range1>::type::value_type,
          typename Range2, typename R2 = typename std::remove_reference<Range2>::type::value_type>
auto all(Range1&& rng1, Range2&& rng2)
{
    auto rng1_begin = range::begin(std::forward<Range1>(rng1));
    auto rng2_begin = range::begin(std::forward<Range2>(rng2));
    auto rng1_end = range::end(std::forward<Range1>(rng1));
    auto rng2_end = range::end(std::forward<Range2>(rng2));

    auto result = std::mismatch(rng1_begin, rng1_end, rng2_begin, rng2_end);

    return result.first == rng1_end && result.second == rng2_end;
}

template <typename Range1, typename R1 = typename std::remove_reference<Range1>::type::value_type,
          typename Range2, typename R2 = typename std::remove_reference<Range2>::type::value_type,
          typename BinaryPredicate>
auto all(Range1&& rng1, Range2&& rng2, BinaryPredicate p)
{
    auto rng1_begin = range::begin(std::forward<Range1>(rng1));
    auto rng2_begin = range::begin(std::forward<Range2>(rng2));
    auto rng1_end = range::end(std::forward<Range1>(rng1));
    auto rng2_end = range::end(std::forward<Range2>(rng2));

    auto result = std::mismatch(rng1_begin, rng1_end, rng2_begin, rng2_end, p);

    return result.first == rng1_end && result.second == rng2_end;
}

} // namespace jules

#endif // JULES_UTIL_NUMERIC_H
