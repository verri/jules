// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>
// Inspired by https://foonathan.github.io/blog/2016/09/09/cpp14-concepts.html

#ifndef JULES_CORE_META_H
#define JULES_CORE_META_H

#include <range/v3/utility/concepts.hpp>

#include <type_traits>

namespace jules
{
namespace meta
{

template <typename T, template <typename> typename Expression, typename = std::void_t<>> struct compiles : std::false_type {
};

template <typename T, template <typename> typename Expression>
struct compiles<T, Expression, std::void_t<Expression<T>>> : std::true_type {
};

template <typename T, typename R, template <typename> typename Expression, typename = std::void_t<>>
struct compiles_same : std::false_type {
};

template <typename T, typename R, template <typename> typename Expression>
struct compiles_same<T, R, Expression, std::void_t<std::enable_if_t<std::is_same<R, Expression<T>>::value>>> : std::true_type {
};

template <typename T, template <typename...> typename R, template <typename> typename Expression, typename = std::void_t<>>
struct compiles_models : std::false_type {
};

template <typename T, template <typename...> typename R, template <typename> typename Expression>
struct compiles_models<T, R, Expression, std::void_t<std::enable_if_t<R<Expression<T>>::value>>> : std::true_type {
};

template <typename... Checks> using requires = std::enable_if_t<std::conjunction<Checks...>::value>;

template <typename... Checks> using fallback = std::enable_if_t<std::conjunction<std::negation<Checks...>>::value>;

template <typename R, typename... Checks> using requires_t = std::enable_if_t<std::conjunction<Checks...>::value, R>;

template <typename R, typename... Checks>
using fallback_t = std::enable_if_t<std::conjunction<std::negation<Checks>...>::value, R>;

template <typename...> struct always_false : std::false_type {
};

template <typename...> struct always_true : std::true_type {
};

} // namespace meta

using ranges::v3::DefaultConstructible;
using ranges::v3::CopyConstructible;
using ranges::v3::MoveConstructible;
using ranges::v3::Copyable;
using ranges::v3::Movable;

} // namespace jules

#endif // JULES_CORE_META_H
