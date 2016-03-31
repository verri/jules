#ifndef JULES_DATAFRAME_DATAFRAME_DECL_H
#define JULES_DATAFRAME_DATAFRAME_DECL_H

#include <jules/array/array.hpp>
#include <jules/dataframe/column.hpp>
#include <jules/dataframe/dataframe_colview.hpp>
#include <jules/dataframe/io_decl.hpp>
#include <jules/formula/expression_decl.hpp>

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>

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

    base_dataframe() = default;
    base_dataframe(std::initializer_list<column_t> columns);

    base_dataframe(const base_dataframe& source) = default;
    base_dataframe(base_dataframe&& source) = default;

    base_dataframe& operator=(const base_dataframe& source) = default;
    base_dataframe& operator=(base_dataframe&& source) = default;

    base_dataframe& colbind(const column_t& column);
    base_dataframe& colbind(column_t&& column);

    vector<std::string> colnames() const;

    const column_t& select(std::size_t i) const { return columns_.at(i); }
    const column_t& select(const std::string& name) const;

    column_t select(const expr_t& expression) const;
    base_dataframe select(const expr_list_t& expression_list) const;

    template <typename T> base_dataframe& coerce_to();
    template <typename T, typename C> friend bool can_coerce_to(const base_dataframe<C>& dataframe);

    template <typename T> bool can_coerce_to() const;
    template <typename T, typename C> friend bool can_coerce_to(const base_dataframe<C>& dataframe);

    bool empty() const { return nrow() == 0; }
    bool null() const { return ncol() == 0; }

    std::size_t nrow() const { return nrow_; }
    std::size_t ncol() const { return columns_.size(); }

    static base_dataframe read(std::istream& is, const dataframe_read_options& opt = {});
    static void write(const base_dataframe& df, std::ostream& os, const dataframe_write_options& opt = {});

    template <typename T> base_dataframe_colview<T, Coercion> colview();
    template <typename T> base_dataframe_colview<const T, Coercion> colview() const;

    template <typename T, typename C> friend base_dataframe_colview<T, C> colview(base_dataframe<C>& df);
    template <typename T, typename C> friend base_dataframe_colview<const T, C> colview(const base_dataframe<C>& df);

  private:
    std::size_t nrow_ = 0;

    std::vector<column_t> columns_;
    std::unordered_map<std::string, std::size_t> colindexes_;
};

using dataframe = base_dataframe<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_DECL_H
