// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_REFERENCE_H
/// \exclude
#define JULES_ARRAY_META_REFERENCE_H

#include <jules/array/meta/common.hpp>
#include <jules/array/slice.hpp>
#include <jules/base/const_vector.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>

namespace jules
{
namespace meta
{
namespace result
{
template <typename T> using indexing = decltype(std::declval<T&>()[index_t()]);
template <typename T> using slicing = decltype(std::declval<T&>()[base_slice<1u>()]);
template <typename T> using indirect_slicing = decltype(std::declval<T&>()[const_vector<index_t>()]);
// TODO: other kinds of slicing...
} // namespace result
} // namespace meta

template <typename T, typename = void> struct ReferenceArray : std::false_type {
};

// TODO: bidirectional in the future
template <typename T>
struct ReferenceArray<                                                                             //
  T, meta::requires<                                                                               //
       range::ForwardRange<T>,                                                                     //
       CommonArray<T>,                                                                             //
       std::conditional_t<(T::order == 1ul),                                                       //
                          meta::compiles_same<T, typename T::value_type&, meta::result::indexing>, //
                          meta::compiles<T, meta::result::indexing>>,                              //
       meta::compiles<T, meta::result::slicing>,                                                   //
       meta::compiles<T, meta::result::indirect_slicing>                                           //
       >> : std::true_type {
};

} // namespace jules

#endif // JULES_ARRAY_META_REFERENCE_H
