// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_EXPRESSION_H
/// \exclude
#define JULES_ARRAY_META_EXPRESSION_H

#include <jules/array/meta/common.hpp>
#include <jules/core/meta.hpp>

namespace jules
{

template <typename T, typename = void> struct ExpressionArray : std::false_type {
};

template <typename T>
struct ExpressionArray<                        //
  T, meta::requires<                           //
       std::negation<DefaultConstructible<T>>, //
       std::negation<CopyConstructible<T>>,    //
       std::negation<MoveConstructible<T>>,    //
       std::negation<Copyable<T>>,             //
       std::negation<Movable<T>>,              //
       range::InputRange<T>,                   //
       CommonArray<T>                          //
       >> : std::true_type {
};

} // namespace jules

#endif // JULES_ARRAY_META_EXPRESSION_H
