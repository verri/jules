// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_UNARY_EXPR_ARRAY_H
/// \exclude
#define JULES_ARRAY_UNARY_EXPR_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/expr_array.hpp>
#include <jules/array/meta/common.hpp>
#include <jules/array/meta/expression.hpp>

#include <iterator>

namespace jules
{

template <typename It, typename Op, std::size_t N>
class unary_expr_array : public expr_array<Op, N>, public common_array_base<unary_expr_array<It, Op, N>>
{
  /// \exclude
  using iterator_result = decltype(*std::declval<It&>());

public:
  using expr_array<Op, N>::order;

  using value_type = decltype(std::declval<const Op&>()(std::declval<iterator_result>()));

  using typename expr_array<Op, N>::size_type;

  using typename expr_array<Op, N>::difference_type;

  class iterator
  {
    friend class unary_expr_array<It, Op, N>;

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = decltype(std::declval<const Op&>()(std::declval<iterator_result>()));
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
      ++it_;
      return *this;
    }

    constexpr auto operator++(int) -> iterator
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr auto operator==(const iterator& other) const { return it_ == other.it_; }

    constexpr auto operator!=(const iterator& other) const { return !(*this == other); }

    constexpr auto operator*() -> value_type { return source_->operate(*it_); }

  private:
    constexpr iterator(It it, const unary_expr_array* source) : it_{it}, source_{source} {}

    It it_;
    const unary_expr_array* source_;
  };

  using const_iterator = iterator;

public:
  unary_expr_array(It it_first, It it_last, Op op, const std::array<index_t, order>& extents)
    : expr_array<Op, order>(std::move(op), extents), it_first_{it_first}, it_last_{it_last}
  {
  }

  unary_expr_array(const unary_expr_array& source) = delete;
  unary_expr_array(unary_expr_array&& source) noexcept = delete;

  auto operator=(const unary_expr_array& source) -> unary_expr_array& = delete;
  auto operator=(unary_expr_array&& source) noexcept -> unary_expr_array& = delete;

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> iterator { return {it_first_, this}; }
  auto cend() const -> iterator { return {it_last_, this}; }

  using expr_array<Op, order>::size;
  using expr_array<Op, order>::dimensions;
  using expr_array<Op, order>::length;
  using expr_array<Op, order>::row_count;
  using expr_array<Op, order>::column_count;

private:
  It it_first_, it_last_;
};

template <typename It, typename Op, std::size_t N>
unary_expr_array(It, It, Op, const std::array<index_t, N>&)->unary_expr_array<It, Op, N>;

template <typename It, typename Op, std::size_t N>
auto eval(const unary_expr_array<It, Op, N>& source) -> array<typename unary_expr_array<It, Op, N>::value_type, N>
{
  return {source};
}

} // namespace jules

#endif // JULES_ARRAY_UNARY_EXPR_ARRAY_H
