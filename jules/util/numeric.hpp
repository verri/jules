#ifndef JULES_UTIL_NUMERIC_H
#define JULES_UTIL_NUMERIC_H

#include <jules/range/range.hpp>

#include <array>
#include <functional>
#include <numeric>
#include <type_traits>
#include <utility>

namespace jules
{
template <typename Range, typename R = range_value_t<Range>> auto prod(const Range& rng)
{
    static_assert(std::is_arithmetic<R>::value, "Value type must be arithmetic.");
    return std::accumulate(range::begin(rng), range::end(rng), static_cast<R>(1), std::multiplies<R>{});
}

template <typename Range, typename It = range_iterator_t<Range>> It max(Range&& rng)
{
    decltype(auto) frng = std::forward<Range>(rng);
    return std::max_element(range::begin(frng), range::end(frng));
}

constexpr auto prod_args() { return 1u; }
template <typename Head, typename... Tail> constexpr auto prod_args(const Head& head, const Tail&... tail)
{
    return head * prod_args(tail...);
}

constexpr bool all_args() { return true; }
template <typename Head, typename... Tail> constexpr bool all_args(const Head& head, const Tail&... tail)
{
    return head && all_args(tail...);
}

template <typename Range, typename R = range_value_t<Range>> auto all(Range&& rng)
{
    static_assert(std::is_convertible<R, bool>::value, "value type must be convertible to boolean");
    return std::accumulate(range::begin(std::forward<Range>(rng)), range::end(std::forward<Range>(rng)), true,
                           [](auto&& acc, auto&& value) { return acc && value; });
}

template <typename Range1, typename R1 = range_value_t<Range1>, typename Range2,
          typename R2 = range_value_t<Range2>>
auto all(Range1&& rng1, Range2&& rng2)
{
    auto rng1_begin = range::begin(std::forward<Range1>(rng1));
    auto rng2_begin = range::begin(std::forward<Range2>(rng2));
    auto rng1_end = range::end(std::forward<Range1>(rng1));
    auto rng2_end = range::end(std::forward<Range2>(rng2));

    auto result = std::mismatch(rng1_begin, rng1_end, rng2_begin, rng2_end);

    return result.first == rng1_end && result.second == rng2_end;
}

template <typename Range1, typename R1 = range_value_t<Range1>, typename Range2,
          typename R2 = range_value_t<Range2>, typename BinaryPredicate>
auto all(Range1&& rng1, Range2&& rng2, BinaryPredicate p)
{
    auto rng1_begin = range::begin(std::forward<Range1>(rng1));
    auto rng2_begin = range::begin(std::forward<Range2>(rng2));
    auto rng1_end = range::end(std::forward<Range1>(rng1));
    auto rng2_end = range::end(std::forward<Range2>(rng2));

    auto result = std::mismatch(rng1_begin, rng1_end, rng2_begin, rng2_end, p);

    return result.first == rng1_end && result.second == rng2_end;
}

template <typename T, std::size_t N, std::size_t... I>
static std::array<T, N + 1> cat_helper(const std::array<T, N>& head, const T& tail, std::index_sequence<I...>)
{
    return {{head[I]..., tail}};
}

template <typename T, std::size_t N>
static std::array<T, N + 1> cat(const std::array<T, N>& head, const T& tail)
{
    return cat_helper(head, tail, std::make_index_sequence<N>());
}

} // namespace jules

#endif // JULES_UTIL_NUMERIC_H
