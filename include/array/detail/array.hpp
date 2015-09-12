#ifndef JULES_ARRAY_DETAIL_ARRAY_H
#define JULES_ARRAY_DETAIL_ARRAY_H

#include "array/detail/array_decl.hpp"
#include "util/numeric.hpp"

namespace jules
{
namespace detail
{
template <std::size_t N>
template <typename... Dims, typename>
std::size_t base_slice::operator()(Dims... dims) const
{
    std::array<std::size_t, N> indexes{{std::size_t{dims}...}};
    return std::inner_product(std::begin(indexes), std::end(indexes), std::begin(strides_))
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_H
