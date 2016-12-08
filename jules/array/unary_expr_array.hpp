// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

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
  using iterator_reference = typename std::iterator_traits<It>::reference;

  using value_type = decltype(std::declval<Op&>()(std::declval<iterator_reference>()));
  static constexpr auto order = N;

  using size_type = index_t;
  using difference_type = distance_t;

  class iterator
  {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = decltype(std::declval<Op&>()(std::declval<iterator_reference>()));
    using difference_type = distance_t;
    using pointer = void*;
    using reference = value_type;

    constexpr iterator() = default;
    constexpr iterator(It it, const unary_expr_array* source) : it_{it}, source_{source} {}

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

    constexpr auto operator*() -> reference { return source_->op_(*it_); }

    constexpr auto operator-> () -> iterator { return nullptr; }

  private:
    It it_;
    const unary_expr_array* source_;
  };

  using const_iterator = iterator;

public:
  unary_expr_array(It it_first, It it_last, Op op, typename base_slice<N>::extent_type extents)
    : it_first_{it_first}, it_last_{it_last}, op_{std::move(op)}, descriptor_{0u, extents}
  {
  }

  // TODO: XXX: with C++17 theses constructors can be deleted.
  unary_expr_array(const unary_expr_array& source) = default;
  unary_expr_array(unary_expr_array&& source) noexcept = default;

  auto operator=(const unary_expr_array& source) -> unary_expr_array& = delete;
  auto operator=(unary_expr_array&& source) noexcept -> unary_expr_array& = delete;

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> iterator { return {it_first_, this}; }
  auto cend() const -> iterator { return {it_last_, this}; }

  auto descriptor() const { return std::make_tuple(it_first_, it_last_, extents()); }
  auto extents() const { return detail::extents(descriptor_); }
  auto size() const { return descriptor_.size(); }

private:
  It it_first_, it_last_;
  mutable Op op_; // TODO: XXX: is there a better solution?
  base_slice<N> descriptor_;
};

template <typename It, typename Op, std::size_t N>
auto eval(const unary_expr_array<It, Op, N>& source) -> base_array<typename unary_expr_array<It, Op, N>::value_type, N>
{
  return {source};
}

} // namespace jules

#endif // JULES_ARRAY_UNARY_EXPR_ARRAY_H
