#ifndef JULES_CORE_TYPE_H
#define JULES_CORE_TYPE_H

#include <initializer_list>
#include <string>
#include <utility>

#include <type_safe/floating_point.hpp>
#include <type_safe/integer.hpp>

namespace jules
{

/// Standard numeric type.
///
/// It is a type-safe double-precision floating-point.
/// \notes For more details consult [here](https://github.com/foonathan/type_safe).
///
/// \module Basic Types
using numeric = type_safe::floating_point<double>;

/// Standard string type.
///
/// It is the C++ standard [std::string]().
///
/// \module Basic Types
using string = std::string;

/// Standard unsigned type.
///
/// It is a type-safe unsigned integer that can represent memory addresses.
/// \notes For more details consult [here](https://github.com/foonathan/type_safe).
///
/// \module Basic Types
using uint = type_safe::integer<std::size_t, type_safe::arithmetic_policy_default>;

/// Standard index type.
///
/// It is a type-safe unsigned integer that can represent memory addresses.
/// \notes For more details consult [here](https://github.com/foonathan/type_safe).
///
/// \module Basic Types
using index_t = std::size_t; // TODO: XXX: check what we need to use: type_safe::index_t;

/// Standard signed type.
///
/// It is a type-safe signed integer that can represent difference of memory addresses.
/// \notes For more details consult [here](https://github.com/foonathan/type_safe).
///
/// \module Basic Types
using sint = type_safe::integer<std::ptrdiff_t, type_safe::arithmetic_policy_default>;

/// Standard distance type.
///
/// It is a type-safe signed integer that can represent difference of memory addresses.
/// \notes For more details consult [here](https://github.com/foonathan/type_safe).
///
/// \module Basic Types
using distance_t = std::ptrdiff_t; // TODO: XXX: check what we need to use: type_safe::distance_t;

/// Coercion rules for [numeric type](standardese://jules::numeric/).
/// \module Coercion Rules
struct numeric_rule {
  using type = numeric;
  static auto coerce_from(const string& value) -> type { return {std::stod(value)}; }
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

  /// Retrives the number of types.
  static constexpr auto type_count() { return sizeof...(Rules); }

  /// Access the `I`-th rule.
  template <std::size_t I> using rule = typename std::tuple_element<I, rules>::type;

  /// Retrives the number of rules.
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

} // namespace jules

#endif // JULES_CORE_TYPE_H
