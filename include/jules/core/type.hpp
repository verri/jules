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

using string = sso_string<24>;

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

template <typename T> concept always_false = false;

} // namespace jules

#endif // JULES_CORE_TYPE_H
