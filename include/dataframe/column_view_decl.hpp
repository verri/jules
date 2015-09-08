#ifndef JULES_DATAFRAME_COLUMN_VIEW_DECL_H
#define JULES_DATAFRAME_COLUMN_VIEW_DECL_H

#include "dataframe/detail/column.hpp"

namespace jules
{
template <typename Coercion> class base_column;

template <typename T, typename Coercion> class base_const_column_view
{
    friend class base_column<Coercion>;

  public:
    using value_type = T;

  private:
    using column_model_t = detail::column_model<T, Coercion>;

  public:
    base_const_column_view(const base_const_column_view& source) = default;
    base_const_column_view(base_const_column_view&& source) = default;

    base_const_column_view& operator=(const base_const_column_view& source) = default;
    base_const_column_view& operator=(base_const_column_view&& source) = default;

    const auto& operator[](std::size_t i) const { return column_model_[i]; }

  protected:
    base_const_column_view(const column_model_t& column_model);
    column_model_t& column_model_;
};

template <typename T, typename Coercion> class base_column_view : public base_const_column_view<T, Coercion>
{
    friend class base_column<Coercion>;

  public:
    using value_type = T;

  private:
    using column_model_t = detail::column_model<T, Coercion>;

  public:
    base_column_view(const base_column_view& source) = default;
    base_column_view(base_column_view&& source) = default;

    base_column_view& operator=(const base_column_view& source) = default;
    base_column_view& operator=(base_column_view&& source) = default;

    auto& operator[](std::size_t i) { return this->column_model_[i]; }

  private:
    base_column_view(column_model_t& column_model) : base_const_column_view<T, Coercion>(column_model) {}
};

template <typename T> using column_view = base_column_view<T, default_coercion_rules>;
template <typename T> using const_column_view = base_const_column_view<T, default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_VIEW_DECL_H
