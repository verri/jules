// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_EXPRESSION_H
/// \exclude
#define JULES_ARRAY_META_EXPRESSION_H

#include <jules/array/meta/common.hpp>
#include <jules/core/meta.hpp>

namespace jules
{

template <typename T>
concept expression_array = !default_constructible<T> && !copy_constructible<T> && !move_constructible<T> && !copyable<T> &&
                           !movable<T> && ranges::input_range<T> && common_array<T>;

} // namespace jules

#endif // JULES_ARRAY_META_EXPRESSION_H
