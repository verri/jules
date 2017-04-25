// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>
// Inspired by https://foonathan.github.io/blog/2016/09/09/cpp14-concepts.html

#ifndef JULES_CORE_META_H
#define JULES_CORE_META_H

#include <type_traits>

namespace jules
{
namespace meta
{

// TODO: In C++17, use std version.
template <typename...> using void_t = void;

template <typename...> struct conjunction : std::true_type {
};

template <typename B> struct conjunction<B> : B {
};

template <typename B, typename... Bs> struct conjunction<B, Bs...> : std::conditional_t<bool(B::value), conjunction<Bs...>, B> {
};

template <bool B> using bool_constant = std::integral_constant<bool, B>;

template <typename B> struct negation : bool_constant<!B::value> {
};

// Concepts emulation

template <typename T, template <typename> class Expression, typename = void_t<>> struct compiles : std::false_type {
};

template <typename T, template <typename> class Expression>
struct compiles<T, Expression, void_t<Expression<T>>> : std::true_type {
};

template <typename... Checks> using requires = std::enable_if_t<conjunction<Checks...>::value>;

template <typename... Checks> using fallback = std::enable_if_t<conjunction<negation<Checks...>>::value>;

template <typename R, typename... Checks> using requires_t = std::enable_if_t<conjunction<Checks...>::value, R>;

template <typename R, typename... Checks> using fallback_t = std::enable_if_t<conjunction<negation<Checks>...>::value, R>;

} // namespace meta
} // namespace jules

#endif // JULES_CORE_META_H
