// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_EXPRESSION_H
/// \exclude
#define JULES_ARRAY_META_EXPRESSION_H

#include <concepts>
#include <jules/array/meta/common.hpp>
#include <jules/core/concepts.hpp>

namespace jules
{

template <typename T>
concept expression_array = !std::default_initializable<T> && !std::copy_constructible<T> && !std::move_constructible<T> &&
                           !std::copyable<T> && !std::movable<T> && ranges::input_range<T> && common_array<T>;

} // namespace jules

#endif // JULES_ARRAY_META_EXPRESSION_H
