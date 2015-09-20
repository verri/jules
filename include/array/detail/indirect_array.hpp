#ifndef JULES_ARRAY_DETAIL_INDIRECT_ARRAY_H
#define JULES_ARRAY_DETAIL_INDIRECT_ARRAY_H

#include "array/detail/indirect_array_decl.hpp"

namespace jules
{
namespace detail
{
template <typename T, std::size_t N>
template <std::size_t... I>
std::array<std::size_t, N> indirect_ndarray<T, N>::extents_helper(std::index_sequence<I...>) const
{
    return {{indexes_[I].size()...}};
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_INDIRECT_ARRAY_H
