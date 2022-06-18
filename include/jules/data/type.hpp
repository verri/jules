// Copyright (c) 2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_TYPE_H
/// \exclude
#define JULES_DATAFRAME_TYPE_H

#include <jules/base/numeric.hpp>
#include <jules/core/type.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <type_traits>

namespace jules
{

using default_types = std::tuple<numeric, integer, index_t, string>;

/// Default class with coercion rules for [numeric](standardese://jules::numeric/) and
/// [string](standardese://jules::string/) classes.
/// \module Coercion Rules
struct coercion_rules
{
  using types = default_types;

  template <typename T>
  requires holds<types, T>
  [[nodiscard]] auto coerce(tag<T>, tag<T>, const T& value) const -> T { return value; }

  [[nodiscard]] auto coerce(tag<string>, tag<numeric>, const string& value) const -> numeric
  {
    const auto view = value.view();
    return boost::lexical_cast<numeric>(view.data(), view.size());
  }

  [[nodiscard]] auto coerce(tag<string>, tag<integer>, const string& value) const -> integer
  {
    const auto view = value.view();
    return boost::numeric_cast<integer>(boost::lexical_cast<std::intmax_t>(view.data(), view.size()));
  }

  [[nodiscard]] auto coerce(tag<string>, tag<index_t>, const string& value) const -> index_t
  {
    const auto view = value.view();
    return boost::numeric_cast<index_t>(boost::lexical_cast<distance_t>(view.data(), view.size()));
  }

  template <integral T> [[nodiscard]] auto coerce(tag<T>, tag<string>, const T& value) const -> string
  {
    return boost::lexical_cast<std::string>(value);
  }

  template <floating_point T> [[nodiscard]] auto coerce(tag<T>, tag<string>, const T& value) const -> string
  {
    return boost::lexical_cast<std::string>(value);
  }

  template <typename T, typename U>
  requires(integral<T> || floating_point<T>) &&
    (!same_as<T, U>)&&(same_as<U, numeric> || same_as<U, integer> || same_as<U, index_t>)
      [[nodiscard]] auto coerce(tag<T>, tag<U>, const T& value) const -> U
  {
    return boost::numeric_cast<U>(value);
  }

  template <typename T, typename U>
  requires convertible_to<T, U> &&(!same_as<T, U>)&&(!holds<types, T>)&&holds<types, U> &&
    (!(integral<T> || floating_point<T>) || !(same_as<U, numeric> || same_as<U, integer> || same_as<U, index_t>))
      [[nodiscard]] auto coerce(tag<T>, tag<U>, const T& value) const -> U
  {
    return value;
  }
};

// clang-format off
template <typename T, typename Rules, typename U> concept coercible_to = requires(const Rules rules, const T& value)
{
  typename Rules::types;
  holds<typename Rules::types, U>;

  { Rules{} } noexcept;
  std::is_trivial_v<Rules>;

  { rules.coerce(tag<T>{}, tag<U>{}, value) } -> same_as<U>;
};
// clang-format on

constexpr auto as_numeric = tag<numeric>{};

constexpr auto as_string = tag<string>{};

constexpr auto as_uinteger = tag<uinteger>{};

constexpr auto as_integer = tag<integer>{};

constexpr auto as_index = tag<index_t>{};

constexpr auto as_size = tag<size_t>{};

constexpr auto as_distance = tag<distance_t>{};

} // namespace jules

#endif // JULES_DATAFRAME_TYPE_H
