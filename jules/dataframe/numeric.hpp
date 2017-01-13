#ifndef JULES_DATAFRAME_NUMERIC_H
#define JULES_DATAFRAME_NUMERIC_H

#include <jules/array/array.hpp>
#include <jules/array/contiguous_array.hpp>
#include <jules/dataframe/column.hpp>

namespace jules
{

template <typename T, typename C> auto to_column(const base_column<C>& column) -> base_column<C>
{
  auto new_model = column.model_->template coerce<T>();
  return std::move(new_model);
}

template <typename T, typename C> auto to_column(base_column<C>&& column) -> base_column<C>
{
  if (column.elements_type() != typeid(T))
    column.template coerce<T>();
  return {std::move(column)};
}

template <typename T, typename C> auto to_view(base_column<C>& column) -> contiguous_array<T, 1>
{
  return {column.template data<T>(), column.size()};
}

template <typename T, typename C> auto to_view(const base_column<C>& column) -> contiguous_array<const T, 1>
{
  return {column.template data<T>(), column.size()};
}

template <typename T, typename C> auto to_vector(const base_column<C>& column) -> vector<T>
{
  auto size = column.size();

  if (column.elements_type() != typeid(T)) {
    auto converted = to_column<T>(column);
    auto data = converted.template data<T>();
    return {data, data + size};
  }

  auto data = column.template data<T>();
  return {data, data + size};
}

} // namespace jules

#endif // JULES_DATAFRAME_NUMERIC_H
