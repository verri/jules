// Copyright (c) 2018 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_AXIS_H
#define JULES_ARRAY_AXIS_H

#include <jules/array/meta/reference.hpp>
#include <jules/array/slicing/bounded.hpp>
#include <jules/core/type.hpp>

namespace jules
{

namespace detail
{

template <size_t Order, size_t Axis, size_t I, typename ArrayLike>
constexpr decltype(auto) axis_at_impl(ArrayLike&& arr, [[maybe_unused]] index_t i)
{
  if constexpr (I == Order)
    return std::forward<ArrayLike>(arr);
  else if constexpr (I == Axis)
    return axis_at_impl<Order, Axis, I+1>(std::forward<ArrayLike>(arr)[i]);
  else
    return axis_at_impl<Order, Axis, I+1>(std::forward<ArrayLike>(arr)[every]);
}

} // detail

template <size_t Axis, typename RefArray, typename = meta::requires<ReferenceArray<RefArray>>>
constexpr decltype(auto) axis_at(RefArray arr, index_t i)
{
  static_assert(Axis < RefArray::order);
  return detail::axis_at_impl<RefArray::order, Axis, 0>(arr, i);
}

template <size_t Axis, typename RefArray> class axis_iterator
{
  static_assert(ReferenceArray<RefArray>::value);
};

template <size_t Axis, typename RefArray> class axis_array : public common_array_base<axis_array<Axis, RefArray>>
{};

template <size_t Axis, typename RefArray, typename meta::requires<ReferenceArray<RefArray>>>
auto axis(RefArray arr) -> axis_array<Axis, RefArray>;

} // namespace jules

#endif // JULES_ARRAY_AXIS_H
