#ifndef JULES_DATAFRAME_DATAFRAME_COLVIEW_H
#define JULES_DATAFRAME_DATAFRAME_COLVIEW_H

#include "dataframe/dataframe_colview_decl.hpp"
#include "range/range.hpp"

namespace jules
{
template <typename T, typename Coercion>
base_const_dataframe_colview<T, Coercion>::base_const_dataframe_colview(const std::vector<column_t>& columns)
{
    columns_.reserve(columns.size());
    range::copy(columns | adaptors::transformed(
                              [](const column_t& col) { return make_view<T>(const_cast<column_t&>(col)); }),
                std::back_inserter(columns_));
}

template <typename T, typename Coercion>
base_dataframe_colview<T, Coercion>::base_dataframe_colview(
    const base_const_dataframe_colview<T, Coercion>& source)
    : base_const_dataframe_colview<T, Coercion>(source)
{
}
} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_COLVIEW_H
