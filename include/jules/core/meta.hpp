// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_CONCEPTS_H
#define JULES_CORE_CONCEPTS_H

#include <concepts/concepts.hpp>
#include <range/v3/range/concepts.hpp>

#include <type_traits>

namespace jules
{

using concepts::constructible_from;
using concepts::convertible_to;
using concepts::copy_constructible;
using concepts::copyable;
using concepts::default_constructible;
using concepts::integral;
using concepts::movable;
using concepts::move_constructible;
using concepts::same_as;
using concepts::semiregular;
using concepts::signed_integral;
using concepts::unsigned_integral;

template <typename T, typename U> concept comparable_with = requires(const T& t, const U& u)
{
  {
    t == u
  }
  ->convertible_to<bool>;
  {
    t != u
  }
  ->convertible_to<bool>;
};

template <typename T> concept adl_to_string_convertible = requires(const T& value)
{
  {
    to_string(value)
  }
  ->convertible_to<std::string>;
};

template <typename T> concept std_to_string_convertible = requires(const T& value)
{
  {
    std::to_string(value)
  }
  ->convertible_to<std::string>;
};

template <typename T> concept to_string_convertible = adl_to_string_convertible<T> || std_to_string_convertible<T>;

} // namespace jules

#endif // JULES_CORE_CONCEPTS_H
