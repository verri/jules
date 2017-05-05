// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_EXPR_ARRAY_H
/// \exclude
#define JULES_ARRAY_EXPR_ARRAY_H

#include <jules/core/type.hpp>

namespace jules
{

template <typename Op, std::size_t N> class expr_array
{
public:
  static constexpr auto order = N;

  using size_type = index_t;

  using difference_type = distance_t;

  expr_array(Op op, const std::array<index_t, order>& extents) : op_{std::move(op)}, dimensions_{extents} {}

  expr_array(const expr_array& source) = delete;
  expr_array(expr_array&& source) noexcept = delete;

  auto operator=(const expr_array& source) -> expr_array& = delete;
  auto operator=(expr_array&& source) noexcept -> expr_array& = delete;

  [[deprecated]] auto extents() const
  {
    // clang-format off
    if constexpr (order == 1ul)
      return dimensions_[0];
    else
      return dimensions_;
    // clang-format on
  }

  auto dimensions() const { return dimensions_; }

  auto size() const
  {
    // clang-format off
    if constexpr (order == 1ul)
      return dimensions_[0];
    else if constexpr (order == 2ul)
      return dimensions_[0] * dimensions_[1];
    else
      return prod(dimensions_);
    // clang-format on
  }

  template <typename _ = void>
  auto length() const -> meta::requires_t<size_type, meta::always_true<_>, std::bool_constant<(order == 1ul)>>
  {
    return dimensions_[0];
  }

  template <typename _ = void>
  auto row_count() const -> meta::requires_t<size_type, meta::always_true<_>, std::bool_constant<(order > 1ul)>>
  {
    return dimensions_[0];
  }

  template <typename _ = void>
  auto column_count() const -> meta::requires_t<size_type, meta::always_true<_>, std::bool_constant<(order > 1ul)>>
  {
    return dimensions_[1];
  }

protected:
  template <typename... Args> decltype(auto) operate(Args&&... args) const { return op_(std::forward<Args>(args)...); }

private:
  Op op_;
  std::array<size_type, order> dimensions_;
};

} // namespace jules

#endif // JULES_ARRAY_EXPR_ARRAY_H
