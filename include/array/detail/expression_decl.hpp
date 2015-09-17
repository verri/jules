#ifndef JULES_ARRAY_DETAIL_EXPRESSION_DECL_H
#define JULES_ARRAY_DETAIL_EXPRESSION_DECL_H

#include "array/detail/array.hpp"

#include <tuple>

namespace jules
{
namespace detail
{
struct empty {
    empty(empty&&) = default;
};

template <typename LhsIt, typename RhsIt, typename LhsContainer, typename RhsContainer, typename Op>
class binary_expr_ndarray
{
    static_assert(!std::is_reference<LhsContainer>::value && !std::is_reference<RhsContainer>::value, "?");

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
                        const RhsIt& rhs_end, LhsContainer&& lhs, RhsContainer&& rhs, const Op& op);

    iterator data_begin() const { return {lhs_begin_, rhs_begin_}; }
    iterator data_end() const { return {lhs_end_, rhs_end_}; }

  private:
    LhsIt lhs_begin_, lhs_end_;
    RhsIt rhs_begin_, rhs_end_;

    LhsContainer lhs_; // to properly destroy object if it was moved
    RhsContainer rhs_;

    Op op_;
};

template <typename It, typename Container, typename Op> class unary_expr_ndarray
{
    static_assert(!std::is_reference<Container>::value, "?");

  public:
    class iterator
    {
        using value_type = decltype(std::declval<Op>()(*std::declval<It>()));
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::forward_iterator_tag;

        iterator(const It& it) : it_{it} {}

        auto operator*() const { return op_(*it_); }

        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const { return it_ == other.it_; }
        bool operator!=(const iterator& other) const { return it_ == other.it_; }

        std::ptrdiff_t operator-(const iterator& other) const { return it_ - other.it_; }

      private:
        It it_;
        Op op_;
    };

    unary_expr_ndarray(const It& it_begin, const It& it_end, Container&& c, const Op& op);

    unary_expr_ndarray(const unary_expr_ndarray& source) = delete;
    unary_expr_ndarray(unary_expr_ndarray&& source) = default;

    unary_expr_ndarray& operator=(const unary_expr_ndarray& source) = delete;
    unary_expr_ndarray& operator=(unary_expr_ndarray&& source) = default;

    iterator data_begin() const { return {it_begin_}; }
    iterator data_end() const { return {it_end_}; }

  private:
    It it_begin_, it_end_;
    Container c_; // to properly destroy object if it was moved
    Op op_;
};

template <typename T, std::size_t N>
std::tuple<const T*, const T*, empty&&> get_expr_info(const base_ndarray<T, N>& array);

template <typename T, std::size_t N>
std::tuple<const T*, const T*, base_ndarray<T, N>&&> get_expr_info(base_ndarray<T, N>&& array);

template <typename LhsIt, typename RhsIt, typename LhsContainer, typename RhsContainer, typename Op>
binary_expr_ndarray<LhsIt, RhsIt, std::decay_t<LhsContainer>, std::decay_t<RhsContainer>, Op>
make_expr_ndarray(const LhsIt& lhs_begin, const LhsIt& lhs_end, const RhsIt& rhs_begin, const RhsIt& rhs_end,
                  LhsContainer&& lhs, RhsContainer&& rhs, const Op& op);

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
