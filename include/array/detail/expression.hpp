#ifndef JULES_ARRAY_DETAIL_EXPRESSION_H
#define JULES_ARRAY_DETAIL_EXPRESSION_H

#include "array/detail/expression_decl.hpp"

// TODO: debug extents check

namespace jules
{
namespace detail
{
// Expression classes

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
binary_expr_ndarray<LhsIt, RhsIt, Op, N>::binary_expr_ndarray(
    const LhsIt& lhs_begin, const LhsIt& lhs_end, const RhsIt& rhs_begin, const RhsIt& rhs_end,
    const Op& op, const extent_t& extents)
    : lhs_begin_{lhs_begin}, lhs_end_{lhs_end}, rhs_begin_{rhs_begin}, rhs_end_{rhs_end},
      op_{op}, extents_(extents)
{
}

template <typename It, typename Op, std::size_t N>
unary_expr_ndarray<It, Op, N>::unary_expr_ndarray(const It& it_begin, const It& it_end,
                                                             const Op& op, const extent_t& extents)
    : it_begin_{it_begin}, it_end_{it_end}, op_{op}, extents_(extents)
{
}

// Utilities

template <typename LhsIt, typename RhsIt, typename Op, std::size_t N>
binary_expr_ndarray<LhsIt, RhsIt, Op, N>
make_expr_ndarray(const LhsIt& lhs_begin, const LhsIt& lhs_end, const RhsIt& rhs_begin, const RhsIt& rhs_end,
                  const Op& op, const std::array<std::size_t, N>& extents)
{
    return {lhs_begin, lhs_end, rhs_begin, rhs_end, op, extents};
}

// Arithmetic and other operations

#define BASE_NDARRAY(X__) const base_ndarray<X__, N>&
#define REF_NDARRAY(X__) const ref_ndarray<X__, N>&

#define UNPACK(...) __VA_ARGS__

#define BINARY_OPERATION(TX__, X__, TY__, Y__, OP__) \
    template <UNPACK TX__, UNPACK TY__, std::size_t N> \
    auto operator OP__ (UNPACK X__ lhs, UNPACK Y__ rhs) \
    { \
        return make_expr_ndarray(lhs.data_begin(), lhs.data_end(), rhs.data_begin(), rhs.data_end(), \
                                 [](T& a, U& b) { return a + b; }, lhs.extents());\
    }

#define EXPAND_BINARY_COMBINATIONS(TX__, X__, OP__) \
    BINARY_OPERATION(TX__, X__, (typename U), (BASE_NDARRAY(U)), OP__) \
    BINARY_OPERATION(TX__, X__, (typename U), (REF_NDARRAY(U)), OP__)

#define BINARY_POSSIBILITIES(OP__) \
    EXPAND_BINARY_COMBINATIONS((typename T), (BASE_NDARRAY(T)), OP__) \
    EXPAND_BINARY_COMBINATIONS((typename T), (REF_NDARRAY(T)), OP__)

BINARY_POSSIBILITIES(+)
BINARY_POSSIBILITIES(-)
BINARY_POSSIBILITIES(*)
BINARY_POSSIBILITIES(/)
BINARY_POSSIBILITIES(%)
BINARY_POSSIBILITIES(&)
BINARY_POSSIBILITIES(|)
BINARY_POSSIBILITIES(^)
BINARY_POSSIBILITIES(<<)
BINARY_POSSIBILITIES(>>)
BINARY_POSSIBILITIES(&&)
BINARY_POSSIBILITIES(||)

BINARY_POSSIBILITIES(==)
BINARY_POSSIBILITIES(!=)
BINARY_POSSIBILITIES(<)
BINARY_POSSIBILITIES(<=)
BINARY_POSSIBILITIES(>)
BINARY_POSSIBILITIES(>=)

#undef BINARY_POSSIBILITIES
#undef EXPAND_BINARY_COMBINATIONS
#undef BINARY_OPERATION
#undef UNPACK
#undef BASE_NDARRAY
#undef TMP_NDARRAY
#undef REF_NDARRAY

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
