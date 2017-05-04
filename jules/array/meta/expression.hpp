// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_EXPRESSION_H
/// \exclude
#define JULES_ARRAY_META_EXPRESSION_H

#include <jules/array/meta/common.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>

namespace jules
{

template <typename T, typename> struct ArrayExpression : std::false_type {
};

template <typename T>
struct ArrayExpression<                              //
  T, meta::requires<                                 //
       std::negation<meta::DefaultConstructible<T>>, //
       std::negation<meta::CopyConstructible<T>>,    //
       std::negation<meta::MoveConstructible<T>>,    //
       std::negation<meta::Copyable>,                //
       std::negation<meta::Movable>,                 //
       meta::CommonArray<T>,                         //
       range::ForwardRange<T>                        //
       >> : std::true_type {
};

} // namespace jules

#endif // JULES_ARRAY_META_EXPRESSION_H
