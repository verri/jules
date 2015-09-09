#ifndef JULES_DATAFRAME_DATAFRAME_COLVIEW_DECL_H
#define JULES_DATAFRAME_DATAFRAME_COLVIEW_DECL_H

#include "dataframe/column.hpp"
#include "dataframe/column_view.hpp"

namespace jules
{
template <typename Coercion> class base_dataframe;

template <typename T, typename Coercion> class base_const_dataframe_colview
{
    friend class base_dataframe<Coercion>;

  private:
    using column_t = base_column<Coercion>;
    using column_model_t = detail::column_model<T, Coercion>;

  protected:
    base_const_dataframe_colview(const std::vector<column_t>& columns);

    std::vector<base_column_view<T, Coercion>> columns_;
};

template <typename T, typename Coercion>
class base_dataframe_colview : public base_const_dataframe_colview<T, Coercion>
{
    friend class base_dataframe<Coercion>;

  public:
    base_dataframe_colview(const base_const_dataframe_colview<T, Coercion>& source);
};

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_COLVIEW_DECL_H
