#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include "core/type.hpp"
#include "dataframe/detail/storage.hpp"

#include <initializer_list>
#include <memory>

namespace jules
{
class column
{
  private:
    template <typename T> using storage_t = dataframe_detail::storage<T>;
    using storage_eraser_t = dataframe_detail::storage_eraser;

  public:
    template <typename T>
    column(const string_t& name, std::initializer_list<T> values)
        : name_{name}, storage_{new storage_t<T>(values)}
    {
    }

    template <typename T> column(std::initializer_list<T> values) : storage_{new storage_t<T>(values)} {}

    template <typename T> bool are_elements() const { return storage_ptr<T>() != nullptr; }

    bool can_coerce_to_numeric() const { return storage_->can_coerce_to_numeric(); }

    bool can_coerce_to_string() const { return storage_->can_coerce_to_string(); }

  private:
    string_t name_;
    std::unique_ptr<storage_eraser_t> storage_;

    template <typename T> auto& storage()
    {
        return dynamic_cast<storage_t<std::remove_reference_t<T>>&>(*storage_);
    }

    template <typename T> auto* storage_ptr()
    {
        return dynamic_cast<storage_t<std::remove_reference_t<T>>*>(storage_.get());
    }

    template <typename T> const auto* storage_ptr() const
    {
        return dynamic_cast<storage_t<std::remove_reference_t<T>>*>(storage_.get());
    }
};

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
