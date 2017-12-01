// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_OVERLAP_H
/// \exclude
#define JULES_ARRAY_OVERLAP_H

#include <jules/array/detail/common.hpp>

namespace jules
{

template <typename Array> class overlapping_array
{
public:
  explicit overlapping_array(Array self) : self_(static_cast<Array>(self)) {}

  template <typename U> auto operator=(U&& rhs) -> Array
  {
    array evaluated = rhs;
    static_cast<Array>(self_) = std::move(evaluated);
    return static_cast<Array>(self_);
  }

private:
  Array self_;
};

template <typename T> auto overlap(T&& value) -> overlapping_array<T&&> { return overlapping_array<T&&>{std::forward<T>(value)}; }

} // namespace jules

#endif // JULES_ARRAY_OVERLAP_H
