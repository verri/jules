#ifndef JULES_DATAFRAME_COLUMN_VIEW_DECL_H
#define JULES_DATAFRAME_COLUMN_VIEW_DECL_H

#include "dataframe/detail/column.hpp"

namespace jules
{
template <typename Coercion> class base_column;

template <typename T, typename Coercion> class base_const_column_view
{
  public:
    friend class base_column<Coercion>;

  private:
    using column_model_t = detail::column_model<T, Coercion>;

  public:
    const auto& operator[](std::size_t i) const { return column_model_[i]; }

  protected:
    base_const_column_view(const column_model_t& column_model);
    column_model_t& column_model_;
};

template <typename T, typename Coercion> class base_column_view : public base_const_column_view<T, Coercion>
{
  public:
    friend class base_column<Coercion>;

  private:
    using column_model_t = detail::column_model<T, Coercion>;

  public:
    auto& operator[](std::size_t i) { return this->column_model_[i]; }

  private:
    base_column_view(column_model_t& column_model) : base_const_column_view<T, Coercion>(column_model) {}
};

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_VIEW_DECL_H