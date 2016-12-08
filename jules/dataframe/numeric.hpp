#ifndef JULES_DATAFRAME_NUMERIC_H
#define JULES_DATAFRAME_NUMERIC_H

#include <jules/dataframe/column.hpp>

namespace jules
{

template <typename T, typename C> auto to_column(const base_column<C>& column) -> base_column<C>
{
  auto new_model = column.model_->template coerce<T>();
  return {column.name(), std::move(new_model)};
}

template <typename T, typename C> auto to_column(base_column<C>&& column) -> base_column<C>
{
  if (column.elements_type() != typeid(T))
    column.template coerce<T>();
  return {std::move(column)};
}

} // namespace jules

#endif // JULES_DATAFRAME_NUMERIC_H
