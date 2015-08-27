#ifndef JULES_DATAFRAME_COLUMN_VIEW_H
#define JULES_DATAFRAME_COLUMN_VIEW_H

#include "dataframe/column_view_decl.hpp"

namespace jules
{
template <typename T, typename Coercion>
base_const_column_view<T, Coercion>::base_const_column_view(const column_model_t& column_model)
    : column_model_(const_cast<column_model_t&>(column_model))
{
}

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_VIEW_H
