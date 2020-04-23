// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_TYPE_H
/// \exclude
#define JULES_CORE_TYPE_H

#include <jules/core/concepts.hpp>
#include <jules/core/string.hpp>

#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace jules
{

/// Standard numeric type.
/// \module Basic Types
using numeric = double;

/// Infinity value.
/// \module Basic Types
constexpr auto infinity = std::numeric_limits<numeric>::infinity();

/// Standard string type.
///
/// Small-string optimized tyeiG6dec6pe that do not behave as a range.
///
/// \module Basic Types

using string = sso_string<16>;

namespace literals
{
constexpr auto operator""_s(const char* p, std::size_t n) -> string { return std::string_view(p, n); }
} // namespace literals

/// Standard unsigned type.
/// \module Basic Types
using uinteger = unsigned;

/// Standard index type.
/// \module Basic Types
using index_t = std::size_t;

/// Standard size type.
/// \module Basic Types
using size_t = std::size_t;

/// Standard signed type.
/// \module Basic Types
using integer = int;

/// Standard distance type.
/// \module Basic Types
using distance_t = std::ptrdiff_t;

template <typename T> using container = std::vector<T>;

template <typename T> struct default_rule
{
  using type = T;

  template <typename U> requires convertible_to<const U&, type> static auto coerce_from(const U& value) -> type { return value; }
};

/// Coercion rules for [numeric type](standardese://jules::numeric/).
/// \module Coercion Rules
struct numeric_rule : public default_rule<numeric>
{
  using default_rule<numeric>::type;
  using default_rule<numeric>::coerce_from;
  static auto coerce_from(const string& value) -> type { return std::stod(std::string(value.view())); }
};

/// Coercion rules for [string type](standardese://jules::string/).
/// \module Coercion Rules
struct string_rule : public default_rule<string>
{
  using default_rule<string>::type;
  using default_rule<string>::coerce_from;

  template <convertible_to_string U> static auto coerce_from(const U& value) -> type
  {
    using std::to_string;
    return to_string(value);
  }
};

/// Coercion rules for [index type](standardese://jules::index_t/).
/// \module Coercion Rules
struct index_rule
{
  using type = index_t;

  static auto coerce_from(const string& ovalue) -> type
  {
    const auto value = ovalue.view();
    auto it = value.begin();
    while (it != value.end() && std::isspace(*it))
      ++it;
    if (it != value.end() && *it == '-')
      throw std::invalid_argument{"index cannot be initialized by a negative value"};
    return std::stoul(std::string(value));
  }

  template <typename U> requires std::is_arithmetic_v<U> static auto coerce_from(const U& value) -> type
  {
    if constexpr (signed_integral<U> || std::is_floating_point_v<U>) {
      if (value < 0)
        throw std::invalid_argument{"index cannot be initialized by a negative value"};
    }
    return value;
  }

  template <convertible_to<type> U> requires(!std::is_arithmetic_v<U>) static auto coerce_from(const U& value) -> type
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
  static auto coerce_from(const string& value) -> type { return std::stoi(std::string(value.view())); }
};

/// Coercion rules for [unsigned type](standardese://jules::uinteger/).
/// \module Coercion Rules
struct uinteger_rule
{
  using type = uinteger;

  static auto coerce_from(const string& value) -> type
  {
    auto result = std::stoul(std::string(value.view()));
    if (result > std::numeric_limits<uinteger>::max())
      throw std::invalid_argument{"value too big to initialize an unsigned value"};
    return result;
  }

  template <signed_integral U> static auto coerce_from(const U& value) -> type
  {
    if (value < 0)
      throw std::invalid_argument{"unsigned cannot be initialized by a negative value"};
    return value;
  }

  template <convertible_to<type> U> requires(!signed_integral<U>) static auto coerce_from(const U& value) -> type
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
};

template <typename T, typename U> constexpr auto operator==(const tag<T>&, const tag<U>&) noexcept
{
  return std::is_same<T, U>::value;
}

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

using in_place_t = std::in_place_t;
constexpr auto in_place = std::in_place;

template <typename T> using in_place_type_t = std::in_place_type_t<T>;
template <typename T> constexpr auto in_place_type = std::in_place_type<T>;

// Non-initialized construction tag

struct uninitialized_t
{
  constexpr explicit uninitialized_t() = default;
};

constexpr auto uninitialized = uninitialized_t{};

struct generated_t
{
  constexpr explicit generated_t() = default;
};

constexpr auto generated = generated_t{};

// Arithmetic rules

template <typename T> struct numeric_traits : std::numeric_limits<T>
{
  static constexpr auto additive_identity() { return static_cast<T>(0); }
  static constexpr auto multiplicative_identity() { return static_cast<T>(1); }

  static constexpr auto unbounded_min()
  {
    if constexpr (std::numeric_limits<T>::has_infinity)
      return -std::numeric_limits<T>::infinity();
    else
      return std::numeric_limits<T>::min();
  }

  static constexpr auto unbounded_max()
  {
    if constexpr (std::numeric_limits<T>::has_infinity)
      return std::numeric_limits<T>::infinity();
    else
      return std::numeric_limits<T>::max();
  }
};

template <typename T> concept common_numeric = std::is_arithmetic_v<T>&& requires { typename numeric_traits<T>; };

template <typename... Fs> struct overloaded : public Fs...
{
  constexpr explicit overloaded(Fs... fs) noexcept : Fs(std::move(fs))... {}
  using Fs::operator()...;
};

} // namespace jules

#endif // JULES_CORE_TYPE_H
