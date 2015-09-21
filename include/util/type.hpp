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

} // namespace detail
} // namespace jules

#endif // JULES_UTIL_TYPE_H
