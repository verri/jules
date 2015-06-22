#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include "core/type.hpp"
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

    template <typename T> bool can_coerce_to() const { return storage_->template can_coerce_to<T>(); }
    std::type_index elements_type() const { return storage_->elements_type(); }

  private:
    std::string name_;
    std::unique_ptr<storage_eraser_t> storage_;
};

using column = base_column<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
