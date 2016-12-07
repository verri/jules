// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_BINARY_EXPR_ARRAY_H
#define JULES_ARRAY_BINARY_EXPR_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/slice.hpp>
#include <jules/core/type.hpp>

#include <iterator>

namespace jules
{

template <typename LhsIt, typename RhsIt, typename Op, size_t N> class binary_expr_array
{
public:
  using lhs_value_type = typename std::iterator_traits<LhsIt>::value_type;
  using rhs_value_type = typename std::iterator_traits<RhsIt>::value_type;

  using value_type = decltype(std::declval<Op&>()(std::declval<lhs_value_type>(), std::declval<rhs_value_type>()));
  static constexpr auto order = N;

  using size_type = index_t;
  using difference_type = distance_t;

  class iterator
  {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = decltype(std::declval<Op&>()(std::declval<lhs_value_type>(), std::declval<rhs_value_type>()));
    using difference_type = distance_t;
    using pointer = void*;
    using reference = value_type;

    constexpr iterator() = default;
    constexpr iterator(LhsIt lhs, RhsIt rhs, const binary_expr_array* source) : lhs_{lhs}, rhs_{rhs}, source_{source} {}

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

    constexpr auto operator*() -> reference { return source_->op_(*lhs_, *rhs_); }

    constexpr auto operator-> () -> pointer { return nullptr; }

  private:
    LhsIt lhs_;
    RhsIt rhs_;
    const binary_expr_array* source_;
  };

  using const_iterator = iterator;

public:
  binary_expr_array(LhsIt lhs_first, LhsIt lhs_last, RhsIt rhs_first, RhsIt rhs_last, const Op& op,
                    typename base_slice<N>::extent_type extents)
    : lhs_first_{lhs_first}, lhs_last_{lhs_last}, rhs_first_{rhs_first}, rhs_last_{rhs_last}, op_{op}, descriptor_{0u, extents}
  {
  }

  // TODO: XXX: with guaranteed copy elision these constructors can be deleted.
  binary_expr_array(const binary_expr_array& source) = default;
  binary_expr_array(binary_expr_array&& source) noexcept = default;

  auto operator=(const binary_expr_array& source) -> binary_expr_array& = delete;
  auto operator=(binary_expr_array&& source) noexcept -> binary_expr_array& = delete;

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return {lhs_first_, rhs_first_, this}; }
  auto cend() const -> const_iterator { return {lhs_last_, rhs_last_, this}; }

  auto descriptor() const { return std::make_tuple(lhs_first_, lhs_last_, rhs_first_, rhs_last_, extents()); }
  auto extents() const { return descriptor_.dimensions(); }
  auto size() const { return descriptor_.size(); }

private:
  LhsIt lhs_first_, lhs_last_;
  RhsIt rhs_first_, rhs_last_;
  mutable Op op_; // TODO: XXX: is there a better solution?
  base_slice<N> descriptor_;
};

template <typename Lhs, typename Rhs, typename Op, std::size_t N>
auto eval(const binary_expr_array<Lhs, Rhs, Op, N>& source)
  -> base_array<typename binary_expr_array<Lhs, Rhs, Op, N>::value_type, N>
{
  return {source};
}

} // namespace jules

#endif // JULES_ARRAY_BINARY_EXPR_ARRAY_H
