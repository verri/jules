#ifndef JULES_ARRAY_DETAIL_EXPRESSION_DECL_H
#define JULES_ARRAY_DETAIL_EXPRESSION_DECL_H

#include "array/detail/array.hpp"

namespace jules
{
namespace detail
{

template <typename IndexesLeft, typename IndexesRight, typename TypeLeft, typename TypeRight, typename F>
class binary_expr_ndarray
{

  private:
    IndexesLeft lhs_begin_, lhs_end_;
    IndexesRight rhs_begin_, rhs_end_;

    TypeLeft *lhs_;
    TypeRight *rhs_;

    F f_;
};

template <typename Indexes, typename T, typename F>
class unary_expr_ndarray
{

  private:
    Indexes begin_, end_;
    T *data_;
    F f_;
};


} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
