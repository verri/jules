// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_OVERLAP_H
/// \exclude
#define JULES_ARRAY_OVERLAP_H

#include <jules/array/detail/common.hpp>
#include <jules/array/meta/reference.hpp>

namespace jules
{

template <reference_array Array> class overlapping_array
{
public:
  explicit overlapping_array(Array self) : self_(std::move(self)) {}

  template <typename U> auto operator=(U&& rhs) && -> Array&
  {
    array evaluated = rhs;
    self_ = std::move(evaluated);
    return self_;
  }

private:
  Array self_;
};

template <reference_array Array> auto overlap(Array a) -> overlapping_array<Array>
{
  return overlapping_array<Array>{std::move(a)};
}

template <typename T, std::size_t N> auto overlap(const array<T, N>& a) { return overlap(ref(a)); }

template <typename T, std::size_t N> auto overlap(array<T, N>& a) { return overlap(ref(a)); }

} // namespace jules

#endif // JULES_ARRAY_OVERLAP_H
