#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include "dataframe/column.hpp"

#include <cstddef>
#include <initializer_list>
#include <vector>

namespace jules
{
template <typename Coercion> class base_dataframe
{
  public:
    using column_t = base_column<Coercion>;

    base_dataframe() = default;
    base_dataframe(std::nullptr_t) : base_dataframe() {}
    base_dataframe(std::initializer_list<column_t> columns) : columns_(columns) {}

    base_dataframe& cbind(const column_t& column)
    {
        // TODO !!!
        return *this;
    }

    base_dataframe& cbind(column_t&& column)
    {
        // TODO !!!
        return *this;
    }

    column_t& col(std::size_t i) { return columns_[i]; }
    const column_t& col(std::size_t i) const { return columns_[i]; }
    column_t& col(const std::string& name) { return columns_[0]; /* TODO */ }
    const column_t& col(const std::string& name) const { return columns_[0]; /* TODO */ }

  private:
    std::size_t nrow_;
    std::vector<column_t> columns_;
};

using dataframe = base_dataframe<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
