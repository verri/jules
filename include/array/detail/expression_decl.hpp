#ifndef JULES_ARRAY_DETAIL_EXPRESSION_DECL_H
#define JULES_ARRAY_DETAIL_EXPRESSION_DECL_H

#include "array/detail/array.hpp"

namespace jules
{
namespace detail
{
template <typename T> class scalar_iterator
{
  public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::forward_iterator_tag;

    scalar_iterator(T& value, std::size_t current) : value_{value}, current_{current} {}

    T& operator*() const { return value_; }

    scalar_iterator& operator++()
    {
        ++current_;
        return *this;
    }
    scalar_iterator operator++(int)
    {
        auto c = *this;
        ++(*this);
        return c;
    }
    bool operator==(const scalar_iterator& other) const { return current_ == other.current_; }
    bool operator!=(const scalar_iterator& other) const { return current_ == other.current_; }

    std::ptrdiff_t operator-(const scalar_iterator& other) const { return current_ - other.current_; }

  private:
    T& value_;
    std::size_t current_;
};

template <typename LhsIt, typename RhsIt, typename Op, size_t N> class binary_expr_ndarray
{
  private:
    using extent_t = std::array<std::size_t, N>;

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
                        const RhsIt& rhs_end, const Op& op, const extent_t& extents);

    // XXX: how to keep private without hiding operator
    binary_expr_ndarray(const binary_expr_ndarray& source) = delete;
    binary_expr_ndarray(binary_expr_ndarray&& source) = default;

    binary_expr_ndarray& operator=(const binary_expr_ndarray& source) = delete;
    binary_expr_ndarray& operator=(binary_expr_ndarray&& source) = default;

    iterator data_begin() const { return {lhs_begin_, rhs_begin_}; }
    iterator data_end() const { return {lhs_end_, rhs_end_}; }

    const auto& extents() const { return extents_; }
    std::size_t size() const { return prod(extents_); }

    template <typename LhsI, typename RhsI, typename F, std::size_t M>
    friend binary_expr_ndarray<LhsI, RhsI, F, M>
    make_expr_ndarray(const LhsI& lhs_begin, const LhsI& lhs_end, const RhsI& rhs_begin, const RhsI& rhs_end,
                      const F& op, const std::array<std::size_t, M>& extents);

  private:
    LhsIt lhs_begin_, lhs_end_;
    RhsIt rhs_begin_, rhs_end_;

    Op op_;
    extent_t extents_;
};

template <typename It, typename Op, std::size_t N> class unary_expr_ndarray
{
  private:
    using extent_t = std::array<std::size_t, N>;

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

    unary_expr_ndarray(const It& it_begin, const It& it_end, const Op& op, const extent_t& extents);

    unary_expr_ndarray(const unary_expr_ndarray& source) = delete;
    unary_expr_ndarray& operator=(const unary_expr_ndarray& source) = delete;

    iterator data_begin() const { return {it_begin_}; }
    iterator data_end() const { return {it_end_}; }

    const auto& extents() const { return extents_; }

  private:
    unary_expr_ndarray(unary_expr_ndarray&& source) = default;
    unary_expr_ndarray& operator=(unary_expr_ndarray&& source) = default;

    It it_begin_, it_end_;
    Op op_;
    extent_t extents_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
