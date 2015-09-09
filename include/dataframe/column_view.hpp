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

template <typename R, typename S, typename C>
vector<bool> operator==(const base_const_column_view<R, C>& lhs, const base_const_column_view<S, C>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error{"columns size differ"};

    vector<bool> result(lhs.size());
    for (std::size_t i = 0; i < lhs.size(); ++i)
        result[i] = lhs[i] == rhs[i];
    return result;
}

template <typename R, typename S, typename C>
vector<bool> operator==(const R& lhs, const base_const_column_view<T, C>& rhs)
{
    vector<bool> result(rhs.size());
    for (std::size_t i = 0; i < lhs.size(); ++i)
        result[i] = lhs == rhs[i];
    return result;
}

template <typename R, typename S, typename C>
vector<bool> operator==(const base_const_column_view<R, C>& lhs, const S& rhs)
{
    vector<bool> result(lhs.size());
    for (std::size_t i = 0; i < lhs.size(); ++i)
        result[i] = lhs[i] == rhs;
    return result;
}

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_VIEW_H
