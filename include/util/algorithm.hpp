#ifndef JULES_UTIL_ALGORITHM_H
#define JULES_UTIL_ALGORITHM_H

#include <utility>

namespace jules
{

template <typename F, typename... Ts>
void for_each_arg(F&& f, Ts&&... t) {
    [](...){}( (f(std::forward<Ts>(t)), 0)... );
}

template <typename Head, typename... Tail>
bool any_arg(Head head, Tail... tail) {
    return head || any_arg(tail...);
}

template <typename Head>
bool any_arg(Head head) {
    return head;
}

template <typename Head, typename... Tail>
bool all_arg(Head head, Tail... tail) {
    return head && all_arg(tail...);
}

template <typename Head>
bool all_arg(Head head) {
    return head;
}

} // namespace jules

#endif // JULES_UTIL_ALGORITHM_H
