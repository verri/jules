// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_TYPE_H
#define JULES_CORE_TYPE_H

#include <jules/core/meta.hpp>

#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

namespace jules
{

namespace detail
{
template <typename T> using to_string_expr = decltype(std::to_string(std::declval<T>()));
}

template <typename T, typename = void> struct StringConvertible : std::false_type
{
};

// TODO: Maybe move it to core/concepts.hpp
template <typename T>
struct StringConvertible<T, std::enable_if_t<meta::compiles<T, detail::to_string_expr>::value>> : std::true_type
{
};

template <typename T, typename = void> struct Signed : std::false_type
{
};

template <typename T> struct Signed<T, std::enable_if_t<std::numeric_limits<T>::is_signed>> : std::true_type
{
};

/// Standard numeric type.
/// \module Basic Types
using numeric = double;

/// Standard string type.
///
/// It is the C++ standard [std::string]().
///
/// \module Basic Types
using string = std::string;

/// Standard unsigned type.
/// \module Basic Types
using uinteger = unsigned;

/// Standard index type.
/// \module Basic Types
using index_t = std::size_t;

/// Standard signed type.
/// \module Basic Types
using integer = int;

/// Standard distance type.
/// \module Basic Types
using distance_t = std::ptrdiff_t;

template <typename T> struct default_rule
{
  using type = T;

  template <typename U, typename = std::enable_if_t<std::is_convertible<const U&, type>::value>>
  static auto coerce_from(const U& value) -> type
  {
    return value;
  }
};

/// Coercion rules for [numeric type](standardese://jules::numeric/).
/// \module Coercion Rules
struct numeric_rule : default_rule<numeric>
{
  using default_rule<numeric>::type;
  using default_rule<numeric>::coerce_from;
  static auto coerce_from(const string& value) -> type { return std::stod(value); }
};

/// Coercion rules for [string type](standardese://jules::string/).
/// \module Coercion Rules
struct string_rule
{
  using type = string;

  template <typename U, typename = meta::requires<StringConvertible<const U&>>> static auto coerce_from(const U& value) -> type
  {
    return std::to_string(value);
  }

  template <typename U, typename = meta::fallback<StringConvertible<const U&>>,
            typename = std::enable_if_t<std::is_convertible<const U&, type>::value>>
  static auto coerce_from(const U& value) -> type
  {
    return value;
  }
};

/// Coercion rules for [index type](standardese://jules::index_t/).
/// \module Coercion Rules
struct index_rule
{
  using type = index_t;

  static auto coerce_from(const string& value) -> type { return std::stoul(value); }

  template <typename U, typename = meta::requires<Signed<U>>> static auto coerce_from(const U& value) -> type
  {
    if (value < 0)
      throw std::invalid_argument{"index cannot be initialized by a negative value"};
    return value;
  }

  template <typename U, typename = meta::fallback<Signed<U>>,
            typename = std::enable_if_t<std::is_convertible<const U&, type>::value>>
  static auto coerce_from(const U& value) -> type
  {
    return value;
  }
};

/// Coercion rules for [integer type](standardese://jules::integer/).
/// \module Coercion Rules
struct integer_rule : default_rule<integer>
{
  using default_rule<integer>::type;
  using default_rule<integer>::coerce_from;
  static auto coerce_from(const string& value) -> type { return std::stoi(value); }
};

/// Coercion rules for [unsigned type](standardese://jules::uinteger/).
/// \module Coercion Rules
struct uinteger_rule
{
  using type = uinteger;

  static auto coerce_from(const string& value) -> type
  {
    auto result = std::stoul(value);
    if (result > std::numeric_limits<uinteger>::max())
      throw std::invalid_argument{"value too big to initialize an unsigned value"};
    return result;
  }

  template <typename U, typename = meta::requires<Signed<U>>> static auto coerce_from(const U& value) -> type
  {
    if (value < 0)
      throw std::invalid_argument{"unsigned cannot be initialized by a negative value"};
    return value;
  }

  template <typename U, typename = meta::fallback<Signed<U>>,
            typename = std::enable_if_t<std::is_convertible<const U&, type>::value>>
  static auto coerce_from(const U& value) -> type
  {
    return value;
  }
};

/// Utility class to combine coercion rules.
/// \module Coercion Rules
template <typename... Rules> class base_coercion_rules
{
private:
  using types = std::tuple<typename Rules::type...>;
  using rules = std::tuple<Rules...>;

public:
  /// Access the `I`-th type.
  template <std::size_t I> using type = typename std::tuple_element<I, types>::type;

  /// Retrieves the number of types.
  static constexpr auto type_count() { return sizeof...(Rules); }

  /// Access the `I`-th rule.
  template <std::size_t I> using rule = typename std::tuple_element<I, rules>::type;

  /// Retrieves the number of rules.
  static constexpr auto rule_count() { return sizeof...(Rules); }
};

/// Default class with coercion rules for [numeric](standardese://jules::numeric/) and
/// [string](standardese://jules::string/) classes.
/// \module Coercion Rules
using coercion_rules = base_coercion_rules<numeric_rule, string_rule, index_rule, integer_rule, uinteger_rule>;

// Tag type utility

template <typename T> struct tag
{
  static_assert(std::is_same<T, std::decay_t<T>>::value, "type cannot have qualifiers");
  using untag = T;
  template <typename U> constexpr auto operator==(const tag<U>&) { return std::is_same<T, U>::value; }
};

// Recursive initializer_list

template <typename T, std::size_t N> struct recursive_initializer_list
{
  using type = std::initializer_list<typename recursive_initializer_list<T, N - 1>::type>;
};

template <typename T> struct recursive_initializer_list<T, 0>
{
  using type = T;
};

template <typename T, std::size_t N> using recursive_initializer_list_t = typename recursive_initializer_list<T, N>::type;

// In-place construction tag

struct in_place_t
{
  constexpr explicit in_place_t() = default;
};

static constexpr auto in_place = in_place_t{};

// Non-initialized construction tag

struct uninitialized_t
{
  constexpr explicit uninitialized_t() = default;
};

static constexpr auto uninitialized = uninitialized_t{};

// Arithmetic rules

template <typename T> struct numeric_traits : std::numeric_limits<T>
{
  static constexpr auto additive_identity() { return static_cast<T>(0); }
  static constexpr auto multiplicative_identity() { return static_cast<T>(1); }
  static constexpr auto unbounded_min()
  {
    return std::numeric_limits<T>::has_infinity ? -std::numeric_limits<T>::infinity() : std::numeric_limits<T>::min();
  }
  static constexpr auto unbounded_max()
  {
    return std::numeric_limits<T>::has_infinity ? std::numeric_limits<T>::infinity() : std::numeric_limits<T>::max();
  }
};

} // namespace jules

#endif // JULES_CORE_TYPE_H
