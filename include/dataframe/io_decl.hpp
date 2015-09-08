#ifndef JULES_DATAFRAME_IO_H
#define JULES_DATAFRAME_IO_H

#include <string>

namespace jules
{
struct dataframe_storage_options {
    std::string separator;
    std::string eol;
    bool header;
};
}

#endif // JULES_DATAFRAME_IO_H
