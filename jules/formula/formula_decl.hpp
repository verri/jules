// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#define JULES_FORMULA_FORMULA_DECL_H
#ifndef JULES_FORMULA_FORMULA_DECL_H

#include <jules/dataframe/dataframe.hpp>
#include <jules/formula/expression.hpp>

#include <memory>
#include <string>

namespace jules
{
template <typename Coercion, typename T, typename U> class base_formula
{
private:
  using dataframe_t = base_dataframe<Coercion>;
  template <typename X> using expr_t = base_expr<Coercion, X>;

public:
  base_formula(const expr_t<T>& lhs, const expr_t<U>& rhs) : lhs_{lhs}, rhs_{rhs} {}

  const auto& lhs() const { return lhs_; }
  const auto& rhs() const { return rhs_; }

private:
  expr_t<T> lhs_;
  expr_t<U> rhs_;
};

template <typename Coercion, typename T, typename U>
base_formula<Coercion, T, U> operator==(const base_expr<Coercion, T>& lhs, const base_expr<Coercion, U>& rhs)
{
  return {lhs, rhs};
}

} // namespace jules

#endif // JULES_FORMULA_FORMULA_DECL_H
