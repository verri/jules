// Copyright (c) 2016-2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_NUMERIC_H
#define JULES_DATAFRAME_NUMERIC_H

#include <jules/array/array.hpp>
#include <jules/array/contiguous_array.hpp>
#include <jules/dataframe/detail/common.hpp>

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
  if (column.elements_type() != typeid(T))
    column.template coerce<T>();
  return {column.template data<T>(), column.size()};
}

template <typename T, typename C> auto to_view(const base_column<C>& column) -> contiguous_array<const T, 1>
{
  return {column.template data<T>(), column.size()};
}

template <typename T, typename C> auto to_view(const base_column<C>&& column) = delete;

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

template <typename T, typename C> auto to_matrix(const base_dataframe<C>& df) -> base_array<T, 2>
{
  auto temp = std::vector<base_column<C>>();
  auto data_vector = std::vector<contiguous_array<const T, 1>>();

  for (auto i = 0u; i < df.column_count(); ++i) {
    const auto& column = df.at(i).column;
    if (column.elements_type() == typeid(T)) {
      data_vector.push_back(to_view<T>(column));
    } else {
      temp.push_back(to_column<T>(column));
      data_vector.push_back(to_view<T>(temp.back()));
    }
  }

  auto result = base_array<T, 2>(uninitialized, df.row_count(), df.column_count());
  auto it = result.begin();
  for (const auto& data : data_vector)
    for (const T& value : data)
      ::new (static_cast<void*>(it++)) T(value);

  return result;
}

} // namespace jules

#endif // JULES_DATAFRAME_NUMERIC_H
