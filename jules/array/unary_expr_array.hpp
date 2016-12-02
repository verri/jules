#ifndef JULES_ARRAY_UNARY_EXPR_ARRAY_H
#define JULES_ARRAY_UNARY_EXPR_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/slice.hpp>
#include <jules/core/type.hpp>

#include <iterator>

namespace jules
{

template <typename It, typename Op, std::size_t N> class unary_expr_array
{
public:
  using value_type = decltype(std::declval<Op>()(std::declval<typename It::value_type>()));
  static constexpr auto order = N;

  using size_type = index_t;
  using difference_type = distance_t;

  class iterator : public std::iterator<std::input_iterator_tag, value_type, distance_t>
  {
    template <typename, typename, std::size_t> friend class unary_expr_array;

  public:
    constexpr iterator() = delete;

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

    constexpr auto operator*() -> value_type { return op_(*it_); }

    constexpr auto operator-> () -> value_type* { return nullptr; }

  private:
    iterator(It it, const Op& op) : it_{it}, op_{op} {}

    It it_;
    Op op_;
  };

  using const_iterator = iterator;

public:
  unary_expr_array(It it_first, It it_last, const Op& op, base_slice<N> descriptor)
    : it_first_{it_first}, it_last_{it_last}, op_{op}, descriptor_{descriptor}
  {
  }

  unary_expr_array(const unary_expr_array& source) = delete;
  unary_expr_array(unary_expr_array&& source) noexcept = delete;

  auto operator=(const unary_expr_array& source) -> unary_expr_array& = delete;
  auto operator=(unary_expr_array&& source) noexcept -> unary_expr_array& = delete;

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() -> const_iterator { return {it_first_, op_}; }
  auto cend() -> const_iterator { return {it_last_, op_}; }

  auto descriptor() const { return std::make_tuple(it_first_, it_last_, descriptor_); }
  auto extents() const { return descriptor_.extents; }
  auto size() const { return descriptor_.size(); }

private:
  It it_first_, it_last_;
  Op op_;
  base_slice<N> descriptor_;
};

template <typename It, typename Op, std::size_t N>
auto eval(const unary_expr_array<It, Op, N>& source) -> base_array<typename unary_expr_array<It, Op, N>::value_type, N>
{
  return {source};
}

} // namespace jules

#endif // JULES_ARRAY_UNARY_EXPR_ARRAY_H
