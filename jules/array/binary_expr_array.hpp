// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_BINARY_EXPR_ARRAY_H
/// \exclude
#define JULES_ARRAY_BINARY_EXPR_ARRAY_H

#include <jules/array/expr_array.hpp>

#include <iterator>

namespace jules
{

template <typename LhsIt, typename RhsIt, typename Op, size_t N> class binary_expr_array : public expr_array<Op, N>
{
  /// \exclude
  using lhs_result = decltype(*std::declval<LhsIt&>());

  /// \exclude
  using rhs_result = decltype(*std::declval<RhsIt&>());

public:
  using expr_array<Op, N>::order;

  using value_type = decltype(std::declval<const Op&>()(std::declval<lhs_result>(), std::declval<rhs_result>()));

  using typename expr_array<Op, N>::size_type;

  using typename expr_array<Op, N>::difference_type;

  class iterator
  {
    friend class binary_expr_array<LhsIt, RhsIt, Op, N>;

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = decltype(std::declval<const Op&>()(std::declval<lhs_result>(), std::declval<rhs_result>()));
    using difference_type = distance_t;
    using reference = value_type&&;
    using pointer = void;

    constexpr iterator() = default;

    constexpr iterator(const iterator& source) = default;
    constexpr iterator(iterator&& source) noexcept = default;

    constexpr iterator& operator=(const iterator& source) = default;
    constexpr iterator& operator=(iterator&& source) noexcept = default;

    constexpr auto operator++() -> iterator&
    {
      ++lhs_;
      ++rhs_;
      return *this;
    }

    constexpr auto operator++(int) -> iterator
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr auto operator==(const iterator& other) const { return lhs_ == other.lhs_ && rhs_ == other.rhs_; }

    constexpr auto operator!=(const iterator& other) const { return !(*this == other); }

    constexpr auto operator*() -> value_type { return source_->operate(*lhs_, *rhs_); }

  private:
    constexpr iterator(LhsIt lhs, RhsIt rhs, const binary_expr_array* source) : lhs_{lhs}, rhs_{rhs}, source_{source} {}

    LhsIt lhs_;
    RhsIt rhs_;
    const binary_expr_array* source_;
  };

  using const_iterator = iterator;

public:
  binary_expr_array(LhsIt lhs_first, LhsIt lhs_last, RhsIt rhs_first, RhsIt rhs_last, const Op& op,
                    const std::array<index_t, order>& extents)
    : expr_array<Op, N>(std::move(op), extents), lhs_first_{lhs_first}, lhs_last_{lhs_last}, rhs_first_{rhs_first}, rhs_last_{
                                                                                                                      rhs_last}
  {
  }

  binary_expr_array(const binary_expr_array& source) = delete;
  binary_expr_array(binary_expr_array&& source) noexcept = delete;

  auto operator=(const binary_expr_array& source) -> binary_expr_array& = delete;
  auto operator=(binary_expr_array&& source) noexcept -> binary_expr_array& = delete;

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return {lhs_first_, rhs_first_, this}; }
  auto cend() const -> const_iterator { return {lhs_last_, rhs_last_, this}; }

private:
  LhsIt lhs_first_, lhs_last_;
  RhsIt rhs_first_, rhs_last_;
};

template <typename Lhs, typename Rhs, typename Op, std::size_t N>
auto eval(const binary_expr_array<Lhs, Rhs, Op, N>& source)
  -> base_array<typename binary_expr_array<Lhs, Rhs, Op, N>::value_type, N>
{
  return {source};
}

} // namespace jules

#endif // JULES_ARRAY_BINARY_EXPR_ARRAY_H
