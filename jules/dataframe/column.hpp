#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include <jules/dataframe/column_decl.hpp>

namespace jules
{
template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(const std::string& name, std::initializer_list<T> values)
    : name_{name}, column_model_{std::make_unique<column_model_t<T>>(values)}
{
}

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(const std::string& name, const T& value, std::size_t size)
    : name_{name}, column_model_{std::make_unique<column_model_t<T>>(size, value)}
{
}

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(std::initializer_list<T> values)
    : column_model_{std::make_unique<column_model_t<T>>(values)}
{
}

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(const T& value, std::size_t size)
    : column_model_{std::make_unique<column_model_t<T>>(size, value)}
{
}

template <typename Coercion>
base_column<Coercion>::base_column(const std::string& name, std::unique_ptr<column_interface_t>&& column_model)
    : name_{name}, column_model_{std::move(column_model)}
{
}

template <typename Coercion>
template <typename Range, typename R>
base_column<Coercion>::base_column(const std::string& name, const Range& rng)
    : name_{name}, column_model_{std::make_unique<column_model_t<R>>(range::begin(rng), range::end(rng))}
{
}

template <typename Coercion>
template <typename Range, typename R>
base_column<Coercion>::base_column(const Range& rng)
    : column_model_{std::make_unique<column_model_t<R>>(range::begin(rng), range::end(rng))}
{
}

template <typename Coercion>
base_column<Coercion>::base_column(const base_column& source)
    : name_{source.name_}, column_model_{std::move(source.column_model_->clone())}
{
}

template <typename Coercion> auto base_column<Coercion>::operator=(const base_column& source) -> base_column &
{
    name_ = source.name_;
    column_model_ = std::move(source.column_model_->clone());

    return *this;
}

template <typename Coercion> template <typename T> auto base_column<Coercion>::coerce_to() & -> base_column &
{
    column_model_ = column_model_->template coerce_to<T>();
    return *this;
}

template <typename T, typename Coercion> auto coerce_to(const base_column<Coercion>& source) -> base_column<Coercion>
{
    return {source.name(), source.column_model_->template coerce_to<T>()};
}

template <typename T, typename Coercion> auto can_coerce_to(const base_column<Coercion>& column) -> bool
{
    return column.column_model_->template can_coerce_to<T>();
}

template <typename Coercion> template <typename T> auto base_column<Coercion>::view() -> view_t<T>
{
    auto& model = this->column_model_.template downcast<T>();
    return {model.data(), model.size()};
}

template <typename Coercion> template <typename T> auto base_column<Coercion>::view() const -> view_t<const T>
{
    const auto& model = this->column_model_.template downcast<T>();
    return {model.data(), model.size()};
}

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
