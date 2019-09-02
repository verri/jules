// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_EXPRESSION_H
/// \exclude
#define JULES_ARRAY_META_EXPRESSION_H

#include <jules/array/meta/common.hpp>
#include <jules/core/meta.hpp>

namespace jules
{

template <typename T, typename = void> struct ExpressionArray : std::false_type
{};

template <typename T>
struct ExpressionArray<                               //
  T, meta::requires<                                  //
       std::bool_constant<!default_constructible<T>>, //
       std::bool_constant<!copy_constructible<T>>,    //
       std::bool_constant<!move_constructible<T>>,    //
       std::bool_constant<!copyable<T>>,              //
       std::bool_constant<!movable<T>>,               //
       std::bool_constant<ranges::input_range<T>>,    //
       CommonArray<T>                                 //
       >> : std::true_type
{};

} // namespace jules

#endif // JULES_ARRAY_META_EXPRESSION_H
