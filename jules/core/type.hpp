// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_TYPE_H
#define JULES_CORE_TYPE_H

#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

namespace jules
{

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

/// Coercion rules for [numeric type](standardese://jules::numeric/).
/// \module Coercion Rules
struct numeric_rule {
  using type = numeric;

  static auto coerce_from(const string& value) -> type { return std::stod(value); }

  template <typename T, typename = std::enable_if_t<std::is_convertible<const T&, type>::value>>
  static auto coerce_from(const T& value) -> type
  {
    return type{value};
  }
};

/// Coercion rules for [string type](standardese://jules::string/).
/// \module Coercion Rules
struct string_rule {
  using type = string;
  static auto coerce_from(const numeric& value) -> type { return std::to_string(static_cast<double>(value)); }
  static auto coerce_from(const index_t& value) -> type { return std::to_string(value); }
  static auto coerce_from(const integer& value) -> type { return std::to_string(value); }
  static auto coerce_from(const uinteger& value) -> type { return std::to_string(value); }
  static auto coerce_from(const type& value) -> type { return value; }
  // TODO: Some automatic rules like the others.
};

/// Coercion rules for [index type](standardese://jules::index_t/).
/// \module Coercion Rules
struct index_rule {
  using type = index_t;

  static auto coerce_from(const numeric& value) -> type
  {
    if (value < 0)
      throw std::invalid_argument{"index from negative numeric value"};
    return static_cast<type>(value);
  }

  static auto coerce_from(const string& value) -> type { return std::stoul(value); }

  static auto coerce_from(const integer& value) -> type
  {
    if (value < 0)
      throw std::invalid_argument{"index from negative value"};
    return static_cast<type>(value);
  }

  template <typename T, typename = std::enable_if_t<std::is_convertible<const T&, type>::value>>
  static auto coerce_from(const T& value) -> type
  {
    return type{value};
  }
};

/// Coercion rules for [integer type](standardese://jules::integer/).
/// \module Coercion Rules
struct integer_rule {
  using type = integer;

  static auto coerce_from(const string& value) -> type { return std::stoi(value); }

  template <typename T, typename = std::enable_if_t<std::is_convertible<const T&, type>::value>>
  static auto coerce_from(const T& value) -> type
  {
    return type{value};
  }
};

/// Coercion rules for [unsigned type](standardese://jules::uinteger/).
/// \module Coercion Rules
struct unsigned_rule {
  using type = uinteger;

  static auto coerce_from(const numeric& value) -> type
  {
    if (value < 0)
      throw std::invalid_argument{"index from negative numeric value"};
    return static_cast<type>(value);
  }

  static auto coerce_from(const string& value) -> type { return static_cast<type>(std::stoul(value)); }

  static auto coerce_from(const integer& value) -> type
  {
    if (value < 0)
      throw std::invalid_argument{"index from negative value"};
    return static_cast<type>(value);
  }

  template <typename T, typename = std::enable_if_t<std::is_convertible<const T&, type>::value>>
  static auto coerce_from(const T& value) -> type
  {
    return type{value};
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
using coercion_rules = base_coercion_rules<numeric_rule, string_rule, index_rule, integer_rule, unsigned_rule>;

namespace detail
{

// Trivial/Non-trivial dispatch

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

// Tag type utility

template <typename T> struct tag {
  static_assert(std::is_same<T, std::decay_t<T>>::value, "type cannot have qualifiers");
  using untag = T;
};

} // namespace detail

// Recursive initializer_list

template <typename T, std::size_t N> struct recursive_initializer_list {
  using type = std::initializer_list<typename recursive_initializer_list<T, N - 1>::type>;
};

template <typename T> struct recursive_initializer_list<T, 0> {
  using type = T;
};

template <typename T, std::size_t N> using recursive_initializer_list_t = typename recursive_initializer_list<T, N>::type;

// In-place construction tag

struct in_place_t {
  constexpr explicit in_place_t() = default;
};

static constexpr auto in_place = in_place_t{};

// Non-initialized construction tag

struct uninitialized_t {
  constexpr explicit uninitialized_t() = default;
};

static constexpr auto uninitialized = uninitialized_t{};

// Arithmetic rules

template <typename T> struct numeric_traits : std::numeric_limits<T> {
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
