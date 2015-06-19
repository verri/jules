#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include "dataframe/column.hpp"

namespace jules
{
class data_frame
{
  public:
    dataframe(column&&... columns) { columns_.reserve(sizeof...(columns)); }

  private:
    vector_t<column> columns_;
};

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
