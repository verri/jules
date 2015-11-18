#ifndef JULES_ARRAY_DETAIL_EXPRESSION_H
#define JULES_ARRAY_DETAIL_EXPRESSION_H

#include <jules/array/detail/expression_decl.hpp>

// TODO: debug extents check

namespace jules
{
namespace detail
{
// Expression Classes

template <typename LhsIt, typename RhsIt, typename Op, std::size_t N>
auto binary_expr_ndarray<LhsIt, RhsIt, Op, N>::iterator::operator++() -> iterator &
{
    ++lhs_;
    ++rhs_;

    return *this;
}

template <typename LhsIt, typename RhsIt, typename Op, std::size_t N>
auto binary_expr_ndarray<LhsIt, RhsIt, Op, N>::iterator::operator++(int) -> iterator
{
    auto c = *this;
    ++(*this);
    return c;
}

template <typename LhsIt, typename RhsIt, typename Op, std::size_t N>
binary_expr_ndarray<LhsIt, RhsIt, Op, N>::binary_expr_ndarray(const LhsIt& lhs_begin, const LhsIt& lhs_end,
                                                              const RhsIt& rhs_begin, const RhsIt& rhs_end,
                                                              const Op& op, const extent_t& extents)
    : lhs_begin_{lhs_begin}, lhs_end_{lhs_end}, rhs_begin_{rhs_begin}, rhs_end_{rhs_end}, op_{op},
      extents_(extents)
{
}

template <typename It, typename Op, std::size_t N>
auto unary_expr_ndarray<It, Op, N>::iterator::operator++() -> iterator &
{
    ++it_;
    return *this;
}

template <typename It, typename Op, std::size_t N>
auto unary_expr_ndarray<It, Op, N>::iterator::operator++(int) -> iterator
{
    auto c = *this;
    ++(*this);
    return c;
}

template <typename It, typename Op, std::size_t N>
unary_expr_ndarray<It, Op, N>::unary_expr_ndarray(const It& it_begin, const It& it_end, const Op& op,
                                                  const extent_t& extents)
    : it_begin_{it_begin}, it_end_{it_end}, op_{op}, extents_(extents)
{
}

// Utilities

template <typename LhsI, typename RhsI, typename F, std::size_t M>
binary_expr_ndarray<LhsI, RhsI, F, M>
make_expr_ndarray(const LhsI& lhs_begin, const LhsI& lhs_end, const RhsI& rhs_begin, const RhsI& rhs_end,
                  const F& op, const std::array<std::size_t, M>& extents)
{
    return {lhs_begin, lhs_end, rhs_begin, rhs_end, op, extents};
}

template <typename It, typename F, std::size_t M>
unary_expr_ndarray<It, F, M> make_expr_ndarray(const It& begin, const It& end, const F& op,
                                               const std::array<std::size_t, M>& extents)
{
    return {begin, end, op, extents};
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
