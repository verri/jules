// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>
// Inspired by https://foonathan.github.io/blog/2016/09/09/cpp14-concepts.html

#ifndef JULES_CORE_META_H
#define JULES_CORE_META_H

#include <concepts/concepts.hpp>
#include <range/v3/range/concepts.hpp>

#include <type_traits>

namespace jules
{
namespace meta
{

template <typename T, template <typename> typename Expression, typename = std::void_t<>> struct compiles : std::false_type
{};

template <typename T, template <typename> typename Expression>
struct compiles<T, Expression, std::void_t<Expression<T>>> : std::true_type
{};

template <typename T, typename R, template <typename> typename Expression, typename = std::void_t<>>
struct compiles_same : std::false_type
{};

template <typename T, typename R, template <typename> typename Expression>
struct compiles_same<T, R, Expression, std::void_t<std::enable_if_t<std::is_same<R, Expression<T>>::value>>> : std::true_type
{};

template <typename T, template <typename...> typename R, template <typename> typename Expression, typename = std::void_t<>>
struct compiles_models : std::false_type
{};

template <typename T, template <typename...> typename R, template <typename> typename Expression>
struct compiles_models<T, R, Expression, std::void_t<std::enable_if_t<R<Expression<T>>::value>>> : std::true_type
{};

template <typename... Checks> using requires_ = std::enable_if_t<std::conjunction<Checks...>::value>;

template <bool... Checks> using requires_concept = std::enable_if_t<std::conjunction<std::bool_constant<Checks>...>::value>;

template <typename... Checks> using fallback = std::enable_if_t<std::conjunction<std::negation<Checks...>>::value>;

template <bool... Checks> using fallback_concept = std::enable_if_t<std::conjunction<std::bool_constant<!Checks>...>::value>;

template <typename R, typename... Checks> using requires_t = std::enable_if_t<std::conjunction<Checks...>::value, R>;

template <typename R, typename... Checks>
using fallback_t = std::enable_if_t<std::conjunction<std::negation<Checks>...>::value, R>;

template <typename...> struct always_false : std::false_type
{};

template <typename...> struct always_true : std::true_type
{};

} // namespace meta

using concepts::copy_constructible;
using concepts::copyable;
using concepts::default_constructible;
using concepts::movable;
using concepts::move_constructible;

} // namespace jules

#endif // JULES_CORE_META_H
