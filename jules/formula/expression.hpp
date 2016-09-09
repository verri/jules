#ifndef JULES_FORMULA_EXPRESSION_H
#define JULES_FORMULA_EXPRESSION_H

#include <jules/dataframe/dataframe.hpp>
#include <jules/formula/expression_decl.hpp>

namespace jules
{
template <typename Coercion, typename T> base_expr<Coercion, T>::base_expr(const std::string& colname)
{
  f_ = [colname](const dataframe_t& data) {
    auto col = data.select(colname);
    if (col.elements_type() != typeid(T))
      col.template coerce_to<T>();
    return col;
  };
}

template <typename Coercion>
template <typename T>
base_expr<Coercion, void>::base_expr(const base_expr<Coercion, T>& source) : child_{new expr_model<T>(source)}
{
}

template <typename Coercion>
base_expr<Coercion, void>::base_expr(const base_expr& source)
  : child_{source.child_ ? source.child_->clone() : nullptr}, colname_{source.colname_}
{
}

template <typename Coercion> auto base_expr<Coercion, void>::operator=(const base_expr& source) -> base_expr&
{
  child_.release(source.child_ ? source.child_->clone() : nullptr);
  colname_ = source.colname_;

  return *this;
}

template <typename Coercion, typename T> auto base_expr<Coercion, T>::extract_from(const dataframe_t& data) const -> column_t
{
  return f_(data);
}

template <typename Coercion> auto base_expr<Coercion, void>::extract_from(const dataframe_t& data) const -> column_t
{
  return child_ ? child_->extract_from(data) : data.select(colname_);
}

template <typename Coercion, typename T> template <typename U> auto base_expr<Coercion, T>::operator+(const U& operand) const
{
  using R = decltype(T{} + U{});

  return base_expr<Coercion, R>([ parent = *this, operand ](const dataframe_t& data) {
    auto col = parent.extract_from(data);

    auto size = col.size();
    auto result = column_t(col.name() + "+" + typeid(U).name(), R{}, size);

    auto view = as_view<T>(col);
    auto result_view = as_view<R>(result);

    for (std::size_t i = 0; i < size; ++i)
      result_view[i] = view[i] + operand;

    return col;
  });
}

template <typename Coercion, typename T>
template <typename U>
auto base_expr<Coercion, T>::operator+(const base_expr<Coercion, U>& operand) const
{
  using R = decltype(T{} + U{});

  return base_expr<Coercion, R>([ parent1 = *this, parent2 = operand ](const dataframe_t& data) {
    auto lhs = parent1.extract_from(data);
    auto rhs = parent2.extract_from(data);

    auto size = data.rows_count();
    auto result = column_t(lhs.name() + "+" + rhs.name(), R{}, size);

    auto lhs_view = as_view<T>(lhs);
    auto rhs_view = as_view<U>(rhs);
    auto result_view = as_view<R>(result);

    for (std::size_t i = 0; i < size; ++i)
      result_view[i] = lhs_view[i] + rhs_view[i];

    return result;
  });
}

template <typename Coercion, typename T> template <typename U> auto base_expr<Coercion, T>::operator*(const U& operand) const
{
  using R = decltype(T{} * U{});

  return base_expr<Coercion, R>([ parent = *this, operand ](const dataframe_t& data) {
    auto col = parent.extract_from(data);

    auto size = col.size();
    auto result = column_t(col.name() + "*" + typeid(U).name(), R{}, size);

    auto view = as_view<T>(col);
    auto result_view = as_view<R>(result);

    for (std::size_t i = 0; i < size; ++i)
      result_view[i] = view[i] * operand;

    return col;
  });
}

template <typename C, typename V, typename U> auto operator*(const U& operand, const base_expr<C, V>& expr)
{
  using R = decltype(U{} * V{});

  return base_expr<C, R>([ parent = expr, operand ](const base_dataframe<C>& data) {
    auto col = parent.extract_from(data);

    auto size = col.size();
    auto result = base_column<C>(col.name() + "*" + typeid(U).name(), R{}, size);

    auto view = as_view<V>(col);
    auto result_view = as_view<R>(result);

    for (std::size_t i = 0; i < size; ++i)
      result_view[i] = operand * view[i];

    return col;
  });
}

template <typename Coercion, typename T>
template <typename U>
auto base_expr<Coercion, T>::operator*(const base_expr<Coercion, U>& operand) const
{
  using R = decltype(T{} * U{});

  return base_expr<Coercion, R>([ parent1 = *this, parent2 = operand ](const dataframe_t& data) {
    auto lhs = parent1.extract_from(data);
    auto rhs = parent2.extract_from(data);

    auto size = data.rows_count();
    auto result = column_t(lhs.name() * "*" * rhs.name(), R{}, size);

    auto lhs_view = as_view<T>(lhs);
    auto rhs_view = as_view<U>(rhs);
    auto result_view = as_view<R>(result);

    for (std::size_t i = 0; i < size; ++i)
      result_view[i] = lhs_view[i] * rhs_view[i];

    return result;
  });
}

template <typename Coercion> auto base_expr_list<Coercion>::extract_from(const dataframe_t& data) const -> dataframe_t
{
  dataframe_t df;
  for (auto&& expr : expressions_)
    df.colbind(expr.extract_from(data));
  return df;
}

} // namespace jules

#endif // JULES_FORMULA_EXPRESSION_H
