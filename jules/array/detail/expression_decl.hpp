#ifndef JULES_ARRAY_DETAIL_EXPRESSION_DECL_H
#define JULES_ARRAY_DETAIL_EXPRESSION_DECL_H

#include <jules/array/detail/common.hpp>
#include <jules/array/detail/define_macros.hpp>

#include <iterator>

namespace jules
{
namespace detail
{
template <typename LhsIt, typename RhsIt, typename Op, size_t N> class binary_expr_ndarray
{
    template <typename, std::size_t> friend class base_ndarray;
    template <typename, std::size_t> friend class ref_ndarray;
    template <typename, std::size_t> friend class indirect_ndarray;
    template <typename, typename, typename, std::size_t> friend class binary_expr_ndarray;
    template <typename, typename, std::size_t> friend class unary_expr_ndarray;

  private:
    using extent_t = std::array<std::size_t, N>;

  public:
    class iterator : public std::iterator<std::forward_iterator_tag,
                                          decltype(std::declval<Op>()(*std::declval<LhsIt>(), *std::declval<RhsIt>()))>
    {
      public:
        iterator(const LhsIt& lhs, const RhsIt& rhs, const Op& op) : lhs_{lhs}, rhs_{rhs}, op_{op} {}

        auto operator*() const { return op_(*lhs_, *rhs_); }

        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const { return lhs_ == other.lhs_; }
        bool operator!=(const iterator& other) const { return lhs_ == other.lhs_; }

        std::ptrdiff_t operator-(const iterator& other) const { return lhs_ - other.lhs_; }

      private:
        LhsIt lhs_;
        RhsIt rhs_;
        const Op& op_;
    };

  public:
    using value_type = typename iterator::value_type;
    static constexpr auto order = N;

    using const_iterator = iterator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    binary_expr_ndarray(const LhsIt& lhs_begin, const LhsIt& lhs_end, const RhsIt& rhs_begin, const RhsIt& rhs_end, const Op& op,
                        const extent_t& extents);

    binary_expr_ndarray(const binary_expr_ndarray& source) = delete;
    binary_expr_ndarray& operator=(const binary_expr_ndarray& source) = delete;

    iterator data_begin() const { return {lhs_begin_, rhs_begin_}; }
    iterator data_end() const { return {lhs_end_, rhs_end_}; }

    const auto& extents() const { return extents_; }
    std::size_t size() const { return prod(extents_); }

    template <typename LhsI, typename RhsI, typename F, std::size_t M>
    friend binary_expr_ndarray<LhsI, RhsI, F, M> make_expr_ndarray(const LhsI& lhs_begin, const LhsI& lhs_end,
                                                                   const RhsI& rhs_begin, const RhsI& rhs_end, const F& op,
                                                                   const std::array<std::size_t, M>& extents);

    FRIEND_OPERATIONS((typename R, std::size_t M), (const base_ndarray<R, M>&))
    FRIEND_OPERATIONS((typename R, std::size_t M), (const ref_ndarray<R, M>&))

  private:
    binary_expr_ndarray(binary_expr_ndarray&& source) = default;
    binary_expr_ndarray& operator=(binary_expr_ndarray&& source) = default;

    LhsIt lhs_begin_, lhs_end_;
    RhsIt rhs_begin_, rhs_end_;

    Op op_;
    extent_t extents_;
};

template <typename It, typename Op, std::size_t N> class unary_expr_ndarray
{
    template <typename, std::size_t> friend class base_ndarray;
    template <typename, std::size_t> friend class ref_ndarray;
    template <typename, std::size_t> friend class indirect_ndarray;
    template <typename, typename, typename, std::size_t> friend class binary_expr_ndarray;
    template <typename, typename, std::size_t> friend class unary_expr_ndarray;

  private:
    using extent_t = std::array<std::size_t, N>;

  public:
    class iterator : public std::iterator<std::forward_iterator_tag, decltype(std::declval<Op>()(*std::declval<It>()))>
    {
      public:
        iterator(const It& it, const Op& op) : it_{it}, op_{op} {}

        auto operator*() const { return op_(*it_); }

        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const { return it_ == other.it_; }
        bool operator!=(const iterator& other) const { return it_ == other.it_; }

        std::ptrdiff_t operator-(const iterator& other) const { return it_ - other.it_; }

      private:
        It it_;
        const Op& op_;
    };

  public:
    using value_type = typename iterator::value_type;
    static constexpr auto order = N;

    using const_iterator = iterator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    unary_expr_ndarray(const It& it_begin, const It& it_end, const Op& op, const extent_t& extents);

    unary_expr_ndarray(const unary_expr_ndarray& source) = delete;
    unary_expr_ndarray& operator=(const unary_expr_ndarray& source) = delete;

    iterator data_begin() const { return {it_begin_, op_}; }
    iterator data_end() const { return {it_end_, op_}; }

    const auto& extents() const { return extents_; }
    std::size_t size() const { return prod(extents_); }

    FRIEND_OPERATIONS((typename R, std::size_t M), (const base_ndarray<R, M>&))
    FRIEND_OPERATIONS((typename R, std::size_t M), (const ref_ndarray<R, M>&))

  private:
    unary_expr_ndarray(unary_expr_ndarray&& source) = default;
    unary_expr_ndarray& operator=(unary_expr_ndarray&& source) = default;

    It it_begin_, it_end_;
    Op op_;
    extent_t extents_;
};

} // namespace detail
} // namespace jules

#include <jules/array/detail/undef_macros.hpp>

#endif // JULES_ARRAY_DETAIL_EXPRESSION_H
