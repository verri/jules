#ifndef JULES_UTIL_TYPE_H
#define JULES_UTIL_TYPE_H

#include <type_traits>

namespace jules
{
namespace detail
{
struct trivial_tag {
};
struct non_trivial_tag {
};

template <typename T, typename = void> struct trivial_dispatch_helper {
    using type = non_trivial_tag;
};

template <typename T> struct trivial_dispatch_helper<T, std::enable_if_t<std::is_trivial<T>::value>> {
    using type = trivial_tag;
};

template <typename T> auto trivial_dispatch() { return typename trivial_dispatch_helper<T>::type{}; }

template <typename... Ts> struct make_void {
    using type = void;
};
template <typename... Ts> using void_t = typename make_void<Ts...>::type;

template <typename F, typename Enabler = void> struct is_callable {
    static constexpr bool value = false;
};

template <typename F> struct is_callable<F, typename std::enable_if_t<std::is_same<void, decltype(std::declval<F>()())>::value>> {
    static constexpr bool value = true;
};

template <typename T> struct tag {
    using untag = T;
};

} // namespace detail
} // namespace jules

#endif // JULES_UTIL_TYPE_H
