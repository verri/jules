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

  expr_array(Op op, const std::array<index_t, order>& extents) : op_{std::move(op)}, descriptor_{extents} {}

  expr_array(const expr_array& source) = delete;
  expr_array(expr_array&& source) noexcept = delete;

  auto operator=(const expr_array& source) -> expr_array& = delete;
  auto operator=(expr_array&& source) noexcept -> expr_array& = delete;

  [[deprecated]] auto extents() const
  {
    // clang-format off
    if constexpr (order == 1ul)
      return descriptor_.extents[0];
    else
      return descriptor_.extents;
    // clang-format on
  }

  auto dimensions() const { return descriptor_.extents; }

  auto size() const { return descriptor_.size(); }

  auto length() const { return descriptor_.length(); }

  auto row_count() const { return descriptor_.row_count(); }

  auto column_count() const { return descriptor_.column_count(); }

protected:
  template <typename... Args> decltype(auto) operate(Args&&... args) const { return op_(std::forward<Args>(args)...); }

private:
  Op op_;
  descriptor<order> descriptor_;
};

} // namespace jules

#endif // JULES_ARRAY_EXPR_ARRAY_H
