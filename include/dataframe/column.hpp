#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include "core/type.hpp"
#include "array/array.hpp"
#include "dataframe/detail/storage.hpp"

#include <initializer_list>
#include <memory>
#include <typeindex>

namespace jules
{
template <typename Coercion> class base_column
{
  private:
    template <typename T> using storage_t = dataframe_detail::storage<T, Coercion>;
    using storage_eraser_t = dataframe_detail::storage_eraser<Coercion>;

  public:
    template <typename T>
    base_column(const std::string& name, std::initializer_list<T> values)
        : name_{name}, storage_{new storage_t<T>(values)}
    {
    }

    template <typename T> base_column(std::initializer_list<T> values) : storage_{new storage_t<T>(values)} {}

    template <typename T> base_column& coerce_to() {
        storage_->template coerce_to<T>();
        return *this;
    }

    template <typename T> bool can_coerce_to() const { return storage_->template can_coerce_to<T>(); }
    std::type_index elements_type() const { return storage_->elements_type(); }

    template <typename T> auto as_array()
    {
        auto& st = storage_->template downcast<T>();
        return detail::base_array<T>{st.data(), st.size()};
    }

    template <typename T> detail::base_array<const T> as_array() const
    {
        auto& st = storage_->template downcast<T>();
        return detail::base_array<const T>{st.data(), st.size()};
    }

  private:
    std::string name_;
    std::unique_ptr<storage_eraser_t> storage_;
};

using column = base_column<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
