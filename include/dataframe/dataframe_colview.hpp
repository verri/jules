#ifndef JULES_DATAFRAME_DATAFRAME_COLVIEW_H
#define JULES_DATAFRAME_DATAFRAME_COLVIEW_H

#include "dataframe/dataframe_colview_decl.hpp"

namespace jules
{

template <typename T, typename Coercion, typename V>
    base_dataframe_colview<T, Coercion,V>::
    base_dataframe_colview(std::vector<V>&& views) :
detail::ref_ndarray<V, 1>{views.data(), {0, {views.size()}}} ,
views_{views}
{ }


} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_COLVIEW_H
