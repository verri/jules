// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_UNARY_EXPR_ARRAY_H
/// \exclude
#define JULES_ARRAY_UNARY_EXPR_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/detail/slicing.hpp>
#include <jules/array/slice.hpp>
#include <jules/core/type.hpp>

#include <iterator>

namespace jules
{

template <typename It, typename Op, std::size_t N> class unary_expr_array
{
  /// \exclude
  using iterator_reference = const typename std::iterator_traits<It>::value_type&;

public:
  static constexpr auto order = N;

  using value_type = decltype(std::declval<const Op&>()(std::declval<iterator_reference>()));

  using size_type = index_t;

  using difference_type = distance_t;

  class iterator
  {
    friend class unary_expr_array<It, Op, N>;

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = decltype(std::declval<const Op&>()(std::declval<iterator_reference>()));
    using difference_type = distance_t;
    using reference = void;
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

    constexpr auto operator*() -> value_type { return source_->op_(const_cast<iterator_reference>(*it_)); }

  private:
    constexpr iterator(It it, const unary_expr_array* source) : it_{it}, source_{source} {}

    It it_;
    const unary_expr_array* source_;
  };

  using const_iterator = iterator;

public:
  unary_expr_array(It it_first, It it_last, Op op, typename base_slice<N>::extent_type extents)
    : it_first_{it_first}, it_last_{it_last}, op_{std::move(op)}, descriptor_{0u, extents}
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

  auto extents() const { return detail::extents(descriptor_); }

  // TODO: fix slicing and functional to comply with return type
  // auto extents() const -> std::array<size_type, order>
  // {
  //   // clang-format off
  //   if constexpr (order == 1ul)
  //     return {{descriptor_.extent}};
  //   else
  //     return descriptor_.extents;
  //   // clang-format on
  // }

  auto size() const { return descriptor_.size(); }

  template <typename _ = void>
  auto length() const -> meta::requires_t<index_t, meta::always_true<_>, std::bool_constant<(order == 1ul)>>
  {
    return detail::extents(descriptor_);
  }

  template <typename _ = void>
  auto row_count() const -> meta::requires_t<index_t, meta::always_true<_>, std::bool_constant<(order > 1ul)>>
  {
    return detail::extents(descriptor_)[0];
  }

  template <typename _ = void>
  auto column_count() const -> meta::requires_t<index_t, meta::always_true<_>, std::bool_constant<(order > 1ul)>>
  {
    return detail::extents(descriptor_)[1];
  }

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
