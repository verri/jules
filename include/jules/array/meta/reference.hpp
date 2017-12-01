// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_REFERENCE_H
/// \exclude
#define JULES_ARRAY_META_REFERENCE_H

#include <jules/array/meta/common.hpp>
#include <jules/array/slicing/absolute.hpp>
#include <jules/base/index_view.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>

namespace jules
{
namespace meta
{
namespace result
{
template <typename T> using indexing = decltype(std::declval<T&>()[index_t()]);
template <typename T> using slice_indexing = decltype(std::declval<T&>()[std::declval<absolute_slice>()]);
template <typename T> using strided_indexing = decltype(std::declval<T&>()[std::declval<absolute_strided_slice>()]);
template <typename T> using indirect_indexing = decltype(std::declval<T&>()[std::declval<index_view>()]);
template <typename T> using public_data = decltype(std::declval<T&>().data());
} // namespace result
} // namespace meta

template <typename T, typename = void> struct ReferenceArray : std::false_type
{
};

// TODO: bidirectional in the future
template <typename T>
struct ReferenceArray<                                             //
  T, meta::requires<                                               //
       range::ForwardRange<T>,                                     //
       CommonArray<T>,                                             //
       meta::compiles<T, meta::result::indexing>,                  //
       meta::compiles<T, meta::result::slice_indexing>,            //
       meta::compiles<T, meta::result::strided_indexing>,          //
       meta::compiles<T, meta::result::indirect_indexing>,         //
       std::negation<meta::compiles<T, meta::result::public_data>> //
       >> : std::true_type
{
};

} // namespace jules

#endif // JULES_ARRAY_META_REFERENCE_H
