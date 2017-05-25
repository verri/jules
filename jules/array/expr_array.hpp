// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_EXPR_ARRAY_H
/// \exclude
#define JULES_ARRAY_EXPR_ARRAY_H

#include <jules/array/descriptor.hpp>
#include <jules/core/type.hpp>

namespace jules
{

template <typename Op, std::size_t N> class expr_array
{
public:
  static constexpr auto order = N;

  using size_type = index_t;

  using difference_type = distance_t;

  constexpr expr_array(Op op, const std::array<index_t, order>& extents) : op_{std::move(op)}, descriptor_{extents} {}

  constexpr expr_array(const expr_array& source) = delete;
  constexpr expr_array(expr_array&& source) noexcept = delete;

  constexpr auto operator=(const expr_array& source) -> expr_array& = delete;
  constexpr auto operator=(expr_array&& source) noexcept -> expr_array& = delete;

  constexpr auto dimensions() const noexcept { return descriptor_.extents; }

  constexpr auto size() const noexcept { return descriptor_.size(); }

  constexpr auto length() const noexcept { return descriptor_.length(); }

  constexpr auto row_count() const noexcept { return descriptor_.row_count(); }

  constexpr auto column_count() const noexcept { return descriptor_.column_count(); }

protected:
  template <typename... Args> decltype(auto) operate(Args&&... args) const { return op_(std::forward<Args>(args)...); }

private:
  Op op_;
  descriptor<order> descriptor_;
};

} // namespace jules

#endif // JULES_ARRAY_EXPR_ARRAY_H
