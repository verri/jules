// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_NUMERIC_H
#define JULES_DATAFRAME_NUMERIC_H

#include <jules/array/array.hpp>
#include <jules/dataframe/detail/common.hpp>

namespace jules
{

template <typename T, typename C> auto to_column(const base_column<C>& column) -> base_column<C>
{
  auto new_model = column.model_->template coerce<T>();
  return new_model;
}

template <typename T, typename C> auto to_column(base_column<C>&& column) -> base_column<C>
{
  if (column.elements_type() != typeid(T))
    column.template coerce<T>();
  return {std::move(column)};
}

template <typename T, typename C> auto to_view(base_column<C>& column) -> ref_array<T, 1u>
{
  if (column.elements_type() != typeid(T))
    column.template coerce<T>();
  return {column.template data<T>(), {{{column.size()}}}};
}

template <typename T, typename C> auto to_view(const base_column<C>& column) -> ref_array<const T, 1u>
{
  return {column.template data<T>(), {{column.size()}}};
}

template <typename T, typename C> auto to_view(const base_column<C>&& column) = delete;

template <typename T, typename C> auto to_vector(const base_column<C>& column) -> array<T, 1u>
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

template <typename T, typename C> auto to_vector(base_column<C>&& column) -> array<T, 1u>
{
  auto size = column.size();

  if (column.elements_type() != typeid(T)) {
    column.template coerce<T>();
  }

  auto data = column.template data<T>();
  return {std::make_move_iterator(data), std::make_move_iterator(data + size)};
}

template <typename T, typename C> auto to_matrix(const base_dataframe<C>& df) -> array<T, 2u>
{
  const auto nrow = df.row_count();
  const auto ncol = df.column_count();

  auto temp = std::vector<base_column<C>>();
  auto data_vector = std::vector<std::pair<bool, const T*>>();

  data_vector.reserve(ncol);

  for (const auto& [name, column] : df) {
    (void)name;
    if (column.elements_type() == typeid(T)) {
      data_vector.push_back({false, column.template data<T>()});
    } else {
      temp.push_back(to_column<T>(column));
      data_vector.push_back({true, temp.back().template data<T>()});
    }
  }

  auto builder = array_builder<T, 2u>{{{nrow, ncol}}};

  for (const auto [should_move, data] : data_vector)
    for (const auto i : indices(nrow))
      if (should_move)
        builder.push_back(std::move(data[i]));
      else
        builder.push_back(data[i]);

  return array<T, 2>(std::move(builder));
}

} // namespace jules

#endif // JULES_DATAFRAME_NUMERIC_H
