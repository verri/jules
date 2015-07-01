#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include "dataframe/column.hpp"

#include <initializer_list>
#include <vector>

namespace jules
{
template <typename Coercion> class base_dataframe
{
  public:
    using column_t = base_column<Coercion>;

    base_dataframe(std::initializer_list<column_t> columns) : columns_(columns) {}

  private:
    std::vector<column_t> columns_;
};

using dataframe = base_dataframe<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
