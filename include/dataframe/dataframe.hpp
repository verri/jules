#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include "dataframe/dataframe_decl.hpp"

#include <algorithm>

namespace jules
{
template <typename Coercion> base_dataframe<Coercion>::base_dataframe(std::initializer_list<column_t> columns)
{
    auto size = columns.begin()->size();
    if (!std::all_of(columns.begin() + 1, columns.end(), [size](auto& col) { return col.size() == size; }))
        throw std::runtime_error{"columns size mismatch"};

    nrow_ = size;
    columns_.assign(columns.begin(), columns.end());

    std::size_t i = 0;
    for (auto& column : columns_) {
        auto& colname = column.name();
        if (colname.empty())
            colname = std::to_string(i);
        if (colindexes_.find(colname) != colindexes_.end())
            throw std::runtime_error{"repeated column name"};
        colindexes_[colname] = i++;
    }
}

template <typename Coercion> base_dataframe<Coercion>& base_dataframe<Coercion>::cbind(const column_t& column)
{
    auto tmp = column;

    auto& name = tmp.name();
    if (name.empty())
        name = std::to_string(columns_.size());

    auto it = colindexes_.find(name);
    if (it != colindexes_.end())
        throw std::runtime_error{"column already exists"};

    if (*this != nullptr && nrow() != tmp.size())
        throw std::runtime_error{"invalid column size"};

    if (*this == nullptr)
        nrow_ = tmp.size();

    colindexes_[name] = columns_.size();
    columns_.push_back(std::move(tmp));

    return *this;
}

template <typename Coercion> base_dataframe<Coercion>& base_dataframe<Coercion>::cbind(column_t&& column)
{
    auto& name = column.name();
    if (name.empty())
        name = std::to_string(columns_.size());

    auto it = colindexes_.find(name);
    if (it != colindexes_.end())
        throw std::runtime_error{"column already exists"};

    if (*this != nullptr && nrow() != column.size())
        throw std::runtime_error{"invalid column size"};

    if (*this == nullptr)
        nrow_ = column.size();

    colindexes_[name] = columns_.size();
    columns_.push_back(std::move(column));

    return *this;
}

template <typename Coercion>
auto base_dataframe<Coercion>::select(const std::string& name) const -> const column_t &
{
    auto it = colindexes_.find(name);
    if (it == colindexes_.end())
        throw std::out_of_range{"column does not exists"};
    return columns_.at(it->second);
}

template <typename Coercion> auto base_dataframe<Coercion>::select(const expr_t& expression) const -> column_t
{
    return expression.extract_from(*this);
}

template <typename Coercion>
auto base_dataframe<Coercion>::select(const expr_list_t& expression_list) const -> base_dataframe
{
    return expression_list.extract_from(*this);
}

template <typename Coercion> base_dataframe<Coercion> base_dataframe<Coercion>::read(std::istream& is)
{
    const auto split = [](const std::string& string, auto out, const char* sep) {
        std::size_t ini = 0, pos = 0;
        while (pos != string.npos) {
            pos = string.find_first_of(sep, ini);
            *out++ = string.substr(ini, pos - ini);
            ini = pos + 1;
        }
        return out;
    };

    std::vector<std::string> header, data;

    std::string header_line;
    std::getline(is, header_line);
    split(header_line, std::back_inserter(header), "\t");

    auto ncol = header.size();

    std::string data_line;
    while (std::getline(is, data_line)) {
        split(data_line, std::back_inserter(data), "\t");
        if (data.size() % ncol != 0)
            throw std::runtime_error{"number of columns differ"};
    }

    auto nrow = data.size() / ncol;

    base_dataframe<Coercion> df;

    for (std::size_t j = 0; j < ncol; ++j) {
        base_column<Coercion> col(header[j], std::string{}, nrow);
        auto view = make_view<std::string>(col);
        for (std::size_t i = 0; i < nrow; ++i)
            view[i] = data[i * ncol + j];
        df.cbind(std::move(col));
    }

    return df;
}

template <typename Coercion>
void base_dataframe<Coercion>::write(const base_dataframe<Coercion>& df, std::ostream& os)
{
    if (df.nrow() == 0 || df.ncol() == 0)
        return;

    std::vector<base_column<Coercion>> coerced;
    std::vector<const_column_view<std::string>> data;

    for (std::size_t j = 0; j < df.ncol(); ++j) {
        const auto& col = df.select(j);

        if (col.elements_type() == typeid(std::string)) {
            data.push_back(make_view<std::string>(col));
        } else {
            coerced.push_back(std::move(coerce_to<std::string>(col)));
            data.push_back(make_view<std::string>(coerced.back()));
        }
    }

    os << df.select(0).name();
    for (std::size_t j = 1; j < df.ncol(); ++j)
        os << "\t" << df.select(j).name();
    os << "\n";

    for (std::size_t i = 0; i < df.nrow(); ++i) {
        os << data[0][i];
        for (std::size_t j = 1; j < df.ncol(); ++j)
            os << "\t" << data[j][i];
        os << "\n";
    }
}

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
