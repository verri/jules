#ifndef JULES_ARRAY_DETAIL_EXPRESSION_DECL_H
#define JULES_ARRAY_DETAIL_EXPRESSION_DECL_H

#include "array/detail/array.hpp"

namespace jules
{
namespace detail
{
template <typename LhsIt, typename RhsIt, typename LhsContainer, typename RhsContainer, typename Op>
class binary_expr_ndarray
{
  public:
    class iterator
    {
        using value_type = decltype(std::declval<Op>()(*std::declval<LhsIt>(), *std::declval<RhsIt>()));
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::forward_iterator_tag;

        iterator(const LhsIt& lhs, const RhsIt& rhs) : lhs_{lhs}, rhs_{rhs} {}

        auto operator*() const { return op_(*lhs_, *rhs_); }

        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const { return lhs_ == other.lhs_; }
        bool operator!=(const iterator& other) const { return lhs_ == other.lhs_; }

        std::ptrdiff_t operator-(const iterator& other) const { return lhs_ - other.lhs_; }

      private:
        LhsIt lhs_;
        RhsIt rhs_;
        Op op_;
    };

    binary_expr_ndarray(const LhsIt& lhs_begin, const LhsIt& lhs_end, const RhsIt& rhs_begin,
                        LhsContainer lhs, RhsContainer rhs, Op op);

    iterator data_begin() const { return {lhs_begin_, rhs_begin_}; }
    iterator data_end() const { return {lhs_end_, rhs_end_}; }

  private:
    LhsIt lhs_begin_, lhs_end_;
    RhsIt rhs_begin_, rhs_end_;

    LhsContainer lhs_; // to properly destroy object if it was moved
    RhsContainer rhs_;

    Op op_;
};

template <typename Indexes, typename T, typename F> class unary_expr_ndarray
{
  private:
    Indexes begin_, end_;
    T* data_;
    F f_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
