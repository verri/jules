#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include "dataframe/detail/storage.hpp"

#include <initializer_list>
#include <memory>
#include <string>

namespace jules {

class column {
  private:
    template <typename T> using storage_t = dataframe_detail::storage<T>;
    using storage_eraser_t = dataframe_detail::storage_eraser;

  public:
    template <typename T>
    column(const std::string& name, std::initializer_list<T> values)
        : name_{name}, storage_{new storage_t<T>(values)}
    {
    }

  private:
    std::string name_;
    std::unique_ptr<storage_eraser_t> storage_;

    template <typename T> auto& storage()
    {
        return dynamic_cast<storage_t<T>&>(*storage_);
    }
};

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
