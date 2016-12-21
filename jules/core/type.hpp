// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_TYPE_H
#define JULES_CORE_TYPE_H

#include <initializer_list>
#include <limits>
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
using uinteger = std::uint32_t;

/// Standard index type.
/// \module Basic Types
using index_t = std::size_t;

/// Standard signed type.
/// \module Basic Types
using integer = std::int64_t;

/// Standard distance type.
/// \module Basic Types
using distance_t = std::ptrdiff_t;

/// Coercion rules for [numeric type](standardese://jules::numeric/).
/// \module Coercion Rules
struct numeric_rule {
  using type = numeric;
  static auto coerce_from(const string& value) -> type { return std::stod(value); }
};

/// Coercion rules for [string type](standardese://jules::string/).
/// \module Coercion Rules
struct string_rule {
  using type = std::string;
  static auto coerce_from(const numeric& value) -> type { return std::to_string(static_cast<double>(value)); }
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
using coercion_rules = base_coercion_rules<numeric_rule, string_rule>;

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

static constexpr in_place_t in_place{};

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
