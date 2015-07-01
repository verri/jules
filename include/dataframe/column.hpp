#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include "core/type.hpp"
#include "array/array.hpp"
#include "dataframe/detail/storage.hpp"

#include <initializer_list>
#include <memory>
#include <typeindex>

// XXX resize must be private!

namespace jules
{
template <typename Coercion> class base_dataframe;

template <typename Coercion> class base_column
{
    friend class base_dataframe<Coercion>;

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

    base_column(const base_column& source)
        : name_{source.name_}, storage_{std::move(source.storage_->clone())}
    {
    }
    base_column(base_column&& source) = default;

    base_column& operator=(const base_column& source)
    {
        name_ = std::move(source.name_);
        storage_ = std::move(source.storage_);

        return *this;
    }

    base_column& operator=(base_column&& source) = default;

    template <typename T> base_column& coerce_to()
    {
        storage_->template coerce_to<T>();
        return *this;
    }

    template <typename T> bool can_coerce_to() const { return storage_->template can_coerce_to<T>(); }
    std::type_index elements_type() const { return storage_->elements_type(); }

    template <typename T> auto as_array()
    {
        auto& st = storage_->template downcast<T>();
        return detail::base_vector<T>{st.data(), st.size()};
    }

    template <typename T> auto as_array() const
    {
        auto& st = storage_->template downcast<T>();
        return detail::base_vector<const T>{st.data(), st.size()};
    }

    auto size() const { return storage_->size(); }

  private:
    std::string name_;
    std::unique_ptr<storage_eraser_t> storage_;
};

using column = base_column<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
