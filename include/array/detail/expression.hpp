#ifndef JULES_ARRAY_DETAIL_EXPRESSION_H
#define JULES_ARRAY_DETAIL_EXPRESSION_H

#include "array/detail/expression_decl.hpp"

namespace jules
{
namespace detail
{
template <typename LhsIt, typename RhsIt, typename LhsContainer, typename RhsContainer, typename Op>
auto binary_expr_ndarray<LhsIt, RhsIt, LhsContainer, RhsContainer, Op>::iterator::operator++() -> iterator &
{
    ++lhs_;
    ++rhs_;

    return *this;
}

template <typename LhsIt, typename RhsIt, typename LhsContainer, typename RhsContainer, typename Op>
auto binary_expr_ndarray<LhsIt, RhsIt, LhsContainer, RhsContainer, Op>::iterator::operator++(int) -> iterator
{
    auto c = *this;
    ++(*this);
    return c;
}

template <typename LhsIt, typename RhsIt, typename LhsContainer, typename RhsContainer, typename Op>
binary_expr_ndarray<LhsIt, RhsIt, LhsContainer, RhsContainer, Op>::binary_expr_ndarray(
    const LhsIt& lhs_begin, const LhsIt& lhs_end, const RhsIt& rhs_begin, LhsContainer lhs, RhsContainer rhs,
    const Op& op)
    : lhs_begin_{lhs_begin}, lhs_end_{lhs_end}, rhs_begin_{rhs_begin}, lhs_{lhs}, rhs_{rhs}, op_{op}
{
}

template <typename It, typename Container, typename Op>
unary_expr_ndarray<It, Container, Op>::unary_expr_ndarray(const It& it_begin, const It& it_end, Container c,
                                                          const Op& op)
    : it_begin_{it_begin}, it_end_{it_end}, c_{c}, op_{op}
{
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
