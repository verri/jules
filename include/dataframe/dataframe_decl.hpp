#ifndef JULES_DATAFRAME_DATAFRAME_DECL_H
#define JULES_DATAFRAME_DATAFRAME_DECL_H

#include "array/array.hpp"
#include "dataframe/column.hpp"

#include <cstddef>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace jules
{
template <typename Coercion> class base_dataframe
{
  public:
    using column_t = base_column<Coercion>;

    base_dataframe() = default;
    base_dataframe(std::nullptr_t) : base_dataframe() {}
    base_dataframe(std::initializer_list<column_t> columns) : columns_(columns) {}

    base_dataframe& cbind(const column_t& column);
    base_dataframe& cbind(column_t&& column);

    column_t& col(std::size_t i) { return columns_.at(i); }
    const column_t& col(std::size_t i) const { return columns_.at(i); }

    column_t& col(const std::string& name);
    const column_t& col(const std::string& name) const;

    bool operator==(std::nullptr_t) { return ncol() == 0; }
    bool operator!=(std::nullptr_t) { return ncol() != 0; }

    std::size_t nrow() const { return nrow_; }
    std::size_t ncol() const { return columns_.size(); }

  private:
    std::size_t nrow_;

    std::vector<column_t> columns_;
    std::unordered_map<std::string, std::size_t> colindexes_;
};

using dataframe = base_dataframe<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_DECL_H
