// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_FORMULA_VARIABLE_H
#define JULES_FORMULA_VARIABLE_H

#include <jules/core/type.hpp>

namespace jules
{

template <typename T = void> class var
{
public:
  using value_type = T;

  var(string name) : name_(name) {}

private:
  string name_;
};

namespace formula_literals
{
auto operator""_v(const char* name, index_t) -> var<> { return name; }
auto operator""_nv(const char* name, index_t) -> var<numeric> { return name; }
auto operator""_sv(const char* name, index_t) -> var<string> { return name; }
} // namespace formula_literals

} // namespace jules

#endif // JULES_FORMULA_VARIABLE_H
