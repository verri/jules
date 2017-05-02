// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>
// Inspired by https://foonathan.github.io/blog/2016/09/09/cpp14-concepts.html

#ifndef JULES_CORE_META_H
#define JULES_CORE_META_H

#include <type_traits>

namespace jules
{
namespace meta
{

template <typename T, template <typename> class Expression, typename = std::void_t<>> struct compiles : std::false_type {
};

template <typename T, template <typename> class Expression>
struct compiles<T, Expression, std::void_t<Expression<T>>> : std::true_type {
};

template <typename... Checks> using requires = std::enable_if_t<std::conjunction<Checks...>::value>;

template <typename... Checks> using fallback = std::enable_if_t<std::conjunction<std::negation<Checks...>>::value>;

template <typename R, typename... Checks> using requires_t = std::enable_if_t<std::conjunction<Checks...>::value, R>;

template <typename R, typename... Checks>
using fallback_t = std::enable_if_t<std::conjunction<std::negation<Checks>...>::value, R>;

} // namespace meta
} // namespace jules

#endif // JULES_CORE_META_H
