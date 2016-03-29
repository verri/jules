#ifndef JULES_DATAFRAME_IO_H
#define JULES_DATAFRAME_IO_H

#include <functional>
#include <iostream>
#include <regex>
#include <string>

namespace jules
{
struct dataframe_read_options {
    dataframe_read_options(const std::regex& line_regex = std::regex{R"(\n)"}, const std::regex& cell_regex = std::regex{R"(\t)"},
                           bool header = true)
        : line{line_regex, true, {}}, cell{cell_regex, true, {}}, header{header}
    {
    }

    ~dataframe_read_options() {}

    struct {
        std::regex regex;
        bool separator;
        std::regex_constants::match_flag_type flag;
    } line;

    struct {
        std::regex regex;
        bool separator;
        std::regex_constants::match_flag_type flag;
    } cell;

    bool header;
};

// clang-format off
struct dataframe_write_options {
    dataframe_write_options(const std::string& line_separator = "\n",
                            const std::string& cell_separator = "\t", bool header = true)
        : line{[sep = line_separator](auto& os)-> auto& { return os << sep; }},
          cell{[](auto& os, const auto& data) -> auto& { return os << data; },
               [sep = cell_separator](auto& os) -> auto & { return os << sep; }},
          header{header} {}

    ~dataframe_write_options() {}

    struct {
        std::function<std::ostream&(std::ostream&)> separator;
    } line;

    struct {
        std::function<std::ostream&(std::ostream&, const std::string&)> data;
        std::function<std::ostream&(std::ostream&)> separator;
    } cell;

    bool header;
};
// clang-format on

} // namespace jules

#endif // JULES_DATAFRAME_IO_H
