// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_CONCEPTS_H
/// \exclude
#define JULES_CORE_CONCEPTS_H

#include <concepts/concepts.hpp>
#include <range/v3/range/concepts.hpp>

#include <type_traits>

namespace jules
{

using namespace concepts;

// clang-format off
template <typename T> concept adl_convertible_to_string = requires(const T& value)
{
  { to_string(value) } -> convertible_to<std::string>;
};

template <typename T> concept std_convertible_to_string = requires(const T& value)
{
  { std::to_string(value) } -> convertible_to<std::string>;
};

template <typename T> concept convertible_to_string = adl_convertible_to_string<T> || std_convertible_to_string<T>;

template <typename C, typename T> concept container_for = requires(C& c, const T& value, std::size_t n) {
  typename C::value_type;
  same_as<typename C::value_type, T>;

  ranges::range<C>;

  { std::as_const(c).data() } -> same_as<const T*>;
  { std::as_const(c).size() } -> convertible_to<std::size_t>;
  { c.push_back(value) };
  { c.reserve(n) };
};
// clang-format on

} // namespace jules

#endif // JULES_CORE_CONCEPTS_H
