// Copyright (c) 2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_STRING_H
/// \exclude
#define JULES_BASE_STRING_H

#include <jules/base/numeric.hpp>
#include <jules/core/string.hpp>
#include <utility>

namespace jules
{

constexpr auto ltrim = unary_operator(optional_operator([](string s) -> string {
  const auto start = s.view().find_first_not_of(" \n\r\t\f\v");
  if (start == std::string_view::npos)
    return {};
  return string{std::move(s = s.view().substr(start))};
}));

constexpr auto rtrim = unary_operator(optional_operator([](string s) -> string {
  const auto end = s.view().find_last_not_of(" \n\r\t\f\v");
  if (end == std::string_view::npos)
    return {};
  return string{std::move(s = s.view().substr(0, end + 1))};
}));

constexpr auto trim = []<typename T>(T&& x) { return ltrim(rtrim(std::forward<T>(x))); };

} // namespace jules

#endif // JULES_BASE_STRING_H
