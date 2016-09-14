#ifndef JULES_DATAFRAME_DATAFRAME_DECL_H
#define JULES_DATAFRAME_DATAFRAME_DECL_H

#include <jules/array/array.hpp>
#include <jules/dataframe/column.hpp>
#include <jules/dataframe/io_decl.hpp>
#include <jules/dataframe/row_view.hpp>
#include <jules/formula/expression_decl.hpp>

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace jules
{
template <typename Coercion> class base_dataframe
{
private:
  using expr_t = base_expr<Coercion, void>;
  using expr_list_t = base_expr_list<Coercion>;

  template <typename T> using column_view_t = base_column_view<T>;

public:
  using column_t = base_column<Coercion>;

  // Constructors

  base_dataframe() = default;
  base_dataframe(std::initializer_list<column_t> columns);
  template <typename Range, typename R = range_value_t<Range>,
            typename = std::enable_if_t<std::is_convertible<R, column_t>::value>>
  base_dataframe(const Range& rng);

  base_dataframe(const base_dataframe& source) = default;
  base_dataframe(base_dataframe&& source) noexcept = default;

  base_dataframe& operator=(const base_dataframe& source) = default;
  base_dataframe& operator=(base_dataframe&& source) noexcept = default;

  // IO

  static auto read(std::istream& is, const dataframe_read_options& opt = {}) -> base_dataframe;
  template <typename C>
  friend auto write(const base_dataframe<C>& df, std::ostream& os, const dataframe_write_options& opt) -> std::ostream&;
  template <typename C> friend auto write(const base_dataframe<C>& df, std::ostream& os) -> std::ostream&;
  template <typename C> friend auto operator<<(std::ostream& os, const base_dataframe<C>& df) -> std::ostream&;

  // Properties

  auto elements_type(std::size_t index) const { return select(index).elements_type(); }
  auto elements_type(const std::string& name) const { return select(name).elements_type(); }

  template <typename T> auto can_coerce_to(std::size_t index) const { return select(index).template can_coerce_to<T>(); }
  template <typename T> auto can_coerce_to(const std::string& name) const { return select(name).template can_coerce_to<T>(); }

  auto columns_elements_types() const -> vector<std::type_index>;
  auto columns_names() const -> vector<std::string>;

  auto dimensions() const { return std::make_tuple(rows_count(), columns_count()); }
  auto size() const { return rows_count() * columns_count(); }
  auto size(std::size_t index) -> std::size_t;

  auto rows_count() const { return nrow_; }
  auto columns_count() const { return columns_.size(); }

  auto is_empty() const { return rows_count() == 0; }
  auto is_null() const { return columns_count() == 0; }

  operator bool() const { return is_null(); }

  // Iterators

  auto begin() { return columns_.begin(); }
  auto end() { return columns_.end(); }

  auto begin() const { return columns_.begin(); }
  auto end() const { return columns_.end(); }

  auto cbegin() const { return columns_.cbegin(); }
  auto cend() const { return columns_.cend(); }

  auto rbegin() { return columns_.rbegin(); }
  auto rend() { return columns_.rend(); }

  auto rbegin() const { return columns_.rbegin(); }
  auto rend() const { return columns_.rend(); }

  auto crbegin() const { return columns_.crbegin(); }
  auto crend() const { return columns_.crend(); }

  // Ranges

  template <typename T> auto columns() -> vector<base_column_view<T>>;
  template <typename T> auto columns() const -> vector<base_column_view<const T>>;

  template <typename... T> auto rows() -> vector<base_row_view<T...>>;
  template <typename... T> auto rows() const -> vector<base_row_view<const T...>>;

  // Others

  base_dataframe& colbind(const column_t& column);
  base_dataframe& colbind(column_t&& column);

  const column_t& select(std::size_t i) const { return columns_.at(i); }
  const column_t& select(const std::string& name) const;

  column_t select(const expr_t& expression) const;
  base_dataframe select(const expr_list_t& expression_list) const;

  template <typename T> base_dataframe& coerce_to();
  template <typename T, typename C> friend auto coerce_to(const base_dataframe<C>& dataframe) -> base_dataframe<C>;

  template <typename T> bool can_coerce_to() const;
  template <typename T, typename C> friend bool can_coerce_to(const base_dataframe<C>& dataframe);

private:
  template <typename... T, std::size_t... I> auto views(std::index_sequence<I...>) -> std::tuple<base_column_view<T>...>;
  template <typename... T, std::size_t... I>
  auto views(std::index_sequence<I...>) const -> std::tuple<base_column_view<const T>...>;

  template <typename... T, std::size_t... I>
  static auto rows_helper(std::tuple<base_column_view<T>...>& columns, std::index_sequence<I...>, std::size_t rows_count)
    -> vector<base_row_view<T...>>;

  std::size_t nrow_ = 0;

  std::vector<column_t> columns_;
  std::unordered_map<std::string, std::size_t> colindexes_;
};

using dataframe = base_dataframe<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_DECL_H
