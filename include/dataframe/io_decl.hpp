#ifndef JULES_DATAFRAME_IO_H
#define JULES_DATAFRAME_IO_H

#include <string>

namespace jules
{
struct dataframe_storage_options {
    std::string separator = R"(\t)";
    std::string eol = R"(\n)";
    bool header = true;
};
}

#endif // JULES_DATAFRAME_IO_H
