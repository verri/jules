#ifndef JULES_ARRAY_DETAIL_EXPRESSION_H
#define JULES_ARRAY_DETAIL_EXPRESSION_H

#include "array/detail/expression_decl.hpp"

namespace jules
{
namespace detail
{
// Expression classes

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
    const LhsIt& lhs_begin, const LhsIt& lhs_end, const RhsIt& rhs_begin, const RhsIt& rhs_end,
    LhsContainer&& lhs, RhsContainer&& rhs, const Op& op)
    : lhs_begin_{lhs_begin}, lhs_end_{lhs_end}, rhs_begin_{rhs_begin}, rhs_end_{rhs_end},
      lhs_{std::move(lhs)}, rhs_{std::move(rhs)}, op_{op}
{
}

template <typename It, typename Container, typename Op>
unary_expr_ndarray<It, Container, Op>::unary_expr_ndarray(const It& it_begin, const It& it_end, Container&& c,
                                                          const Op& op)
    : it_begin_{it_begin}, it_end_{it_end}, c_{std::move(c)}, op_{op}
{
}

// Utilities

template <typename T, std::size_t N>
std::tuple<const T*, const T*, const base_ndarray<T, N>&> get_expr_info(const base_ndarray<T, N>& array)
{
    return {array.data_begin(), array.data_end(), array};
}

template <typename T, std::size_t N>
std::tuple<const T*, const T*, base_ndarray<T, N>&&> get_expr_info(base_ndarray<T, N>&& array)
{
    return {array.data_begin(), array.data_end(), array};
}

template <typename LhsIt, typename RhsIt, typename LhsContainer, typename RhsContainer, typename Op>
binary_expr_ndarray<LhsIt, RhsIt, LhsContainer, RhsContainer, Op>
make_expr_ndarray(const LhsIt& lhs_begin, const LhsIt& lhs_end, const RhsIt& rhs_begin, const RhsIt& rhs_end,
                  LhsContainer&& lhs, RhsContainer&& rhs, const Op& op)
{
    return {lhs_begin, lhs_end, rhs_begin, rhs_end, std::move(lhs), std::move(rhs), op};
}

// Arithmetic and other operations

template <typename T, std::size_t N, typename U>
auto operator+(const base_ndarray<T, N>& lhs, const base_ndarray<U, N>& rhs)
{
    auto lhs_info = get_expr_info(lhs);
    auto rhs_info = get_expr_info(rhs);

    return make_expr_ndarray(std::get<0>(lhs_info), std::get<1>(lhs_info), std::get<0>(rhs_info),
                             std::get<1>(rhs_info), std::get<2>(lhs_info), std::get<2>(rhs_info),
                             [](T& a, U& b) { return a + b; });
}

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
