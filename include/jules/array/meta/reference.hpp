// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_REFERENCE_H
/// \exclude
#define JULES_ARRAY_META_REFERENCE_H

#include <jules/array/meta/common.hpp>
#include <jules/array/slicing/absolute.hpp>
#include <jules/base/index_span.hpp>
#include <jules/core/concepts.hpp>
#include <jules/core/ranges.hpp>

namespace jules
{

// clang-format off
template <typename T> concept exposes_data = requires(T t)
{
  { t.data() } -> same_as<typename T::value_type*>;
};

template <typename T>
concept reference_array = requires(T t, index_t i, absolute_slice ai, absolute_strided_slice asi, index_span ix)
{
  ranges::forward_range<T>;
  common_array<T>;

  !exposes_data<T>;

  { t[i] };
  { t[ai] };
  { t[asi] };
  { t[ix] };
};
// clang-format on

} // namespace jules

#endif // JULES_ARRAY_META_REFERENCE_H
