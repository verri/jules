#ifndef JULES_DATAFRAME_COLUMN_VIEW_H
#define JULES_DATAFRAME_COLUMN_VIEW_H

#include <jules/dataframe/column_view_decl.hpp>

namespace jules
{
template <typename T> base_column_view<T>::operator base_column_view<const T>() const
{
  return {static_cast<const detail::ref_ndarray<T, 1>&>(*this)};
}
} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_VIEW_H
