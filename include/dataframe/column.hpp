#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include "dataframe/column_decl.hpp"

namespace jules
{

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(const std::string& name, std::initializer_list<T> values)
    : name_{name}, storage_{new storage_t<T>(values)}
{
}

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(std::initializer_list<T> values)
    : storage_{new storage_t<T>(values)}
{
}

template <typename Coercion>
base_column<Coercion>::base_column(const std::string& name, std::unique_ptr<storage_eraser_t>&& storage) :
    name_{name}, storage_{std::move(storage)}
{
}

template <typename Coercion>
base_column<Coercion>::base_column(const base_column& source)
    : name_{source.name_}, storage_{std::move(source.storage_->clone())}
{
}

template <typename Coercion>
auto base_column<Coercion>::operator=(const base_column& source) -> base_column&
{
    name_ = source.name_;
    storage_ = std::move(source.storage_->clone());

    return *this;
}

template <typename Coercion>
template <typename T> auto base_column<Coercion>::coerce_to() -> base_column&
{
    storage_ = storage_->template coerce_to<T>();
    return *this;
}

template <typename T, typename Coercion>
base_column<Coercion> coerce_to(const base_column<Coercion>& source)
{
    return base_column<Coercion>{source.name(), source.storage_->template coerce_to<T>()};
}

template <typename T, typename Coercion>
bool can_coerce_to(const base_column<Coercion>& column) {
    return column.storage_->template can_coerce_to<T>();
}

template <typename T, typename Coercion>
array_view<dataframe_detail::storage<T, Coercion>> make_view(base_column<Coercion>& column)
{
    return array_view<dataframe_detail::storage<T, Coercion>>{column.storage_->template downcast<T>()};
}

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
