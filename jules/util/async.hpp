#ifndef JULES_UTIL_ASYNC_H
#define JULES_UTIL_ASYNC_H

#include <jules/core/type.hpp>

#include <type_traits>
#include <utility>

namespace jules
{
namespace detail
{
template <typename F> struct defer_helper {
    static_assert(is_callable<F>::value, "defered object must be callable and return void.");

    defer_helper(F&& f) : f{f} {}
    ~defer_helper() { f(); }
    F f;
};

} // namespace detail

template <typename F> auto defer(F&& f) { return detail::defer_helper<F>{std::forward<F>(f)}; }
template <typename T> T* move_ptr(T*& pointer)
{
    T* tmp = pointer;
    pointer = nullptr;
    return tmp;
}

} // namespace jules

#endif // JULES_UTIL_ASYNC_H
