#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include <jules/dataframe/dataframe_decl.hpp>
#include <jules/range/range.hpp>

#include <algorithm>
#include <iterator>
#include <regex>

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
        if (!colname.empty()) {
            if (colindexes_.find(colname) != colindexes_.end())
                throw std::runtime_error{"repeated column name"};
            colindexes_[colname] = i++;
        }
    }
}

template <typename Coercion> base_dataframe<Coercion>& base_dataframe<Coercion>::colbind(const column_t& column)
{
    const auto& name = column.name();

    if (!name.empty()) {
        auto it = colindexes_.find(name);
        if (it != colindexes_.end())
            throw std::runtime_error{"column already exists"};
    }

    if (!null() && nrow() != column.size())
        throw std::runtime_error{"invalid column size"};

    if (null())
        nrow_ = column.size();

    columns_.push_back(column);
    if (!name.empty())
        colindexes_[name] = columns_.size();

    return *this;
}

template <typename Coercion> base_dataframe<Coercion>& base_dataframe<Coercion>::colbind(column_t&& column)
{
    auto& name = column.name();
    if (!name.empty()) {
        auto it = colindexes_.find(name);
        if (it != colindexes_.end())
            throw std::runtime_error{"column already exists"};
    }

    if (!null() && nrow() != column.size())
        throw std::runtime_error{"invalid column size"};

    if (null())
        nrow_ = column.size();

    if (!name.empty())
        colindexes_[name] = columns_.size();
    columns_.push_back(std::move(column));

    return *this;
}

template <typename Coercion> vector<std::string> base_dataframe<Coercion>::colnames() const
{
    return to_vector<std::string>(columns_ | adaptors::transformed([](const column_t& col) { return col.name(); }));
}

template <typename Coercion> auto base_dataframe<Coercion>::select(const std::string& name) const -> const column_t &
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

template <typename Coercion> auto base_dataframe<Coercion>::select(const expr_list_t& expression_list) const -> base_dataframe
{
    return expression_list.extract_from(*this);
}

template <typename Coercion> template <typename T> base_dataframe<Coercion>& base_dataframe<Coercion>::coerce_to()
{
    std::vector<column_t> new_columns;

    new_columns.reserve(columns_.size());
    std::transform(columns_.begin(), columns_.end(), std::back_inserter(new_columns),
                   [](auto&& column) { return jules::coerce_to<T>(column); });

    columns_ = std::move(new_columns);

    return *this;
}

template <typename Coercion, typename T> base_dataframe<Coercion> coerce_to(const base_dataframe<Coercion>& dataframe)
{
    base_dataframe<Coercion> coerced;
    for (std::size_t i = 0; dataframe.ncol(); ++i)
        coerced.colbind(jules::coerce_to<T>(dataframe.select(i)));
    return coerced;
}

template <typename Coercion> template <typename T> bool base_dataframe<Coercion>::can_coerce_to() const
{
    for (auto&& column : columns_)
        if (!column.template can_coerce_to<T>())
            return false;
    return true;
}

template <typename T, typename Coercion> bool can_coerce_to(const base_dataframe<Coercion>& dataframe)
{
    return dataframe.template can_coerce_to<T>();
}

template <typename Coercion>
base_dataframe<Coercion> base_dataframe<Coercion>::read(std::istream& is, const dataframe_read_options& opt)
{
    using namespace adaptors;
    using namespace range;

    if (!is)
        return {};

    const auto as_range = [](auto&& match) { return make_iterator_range(match.first, match.second); };

    std::string raw_data;
    raw_data.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

    std::vector<std::sub_match<std::string::iterator>> data;
    std::size_t ncol = 0;

    auto line_range = raw_data | tokenized(opt.line.regex, opt.line.separator ? -1 : 0, opt.line.flag);

    std::size_t size = 0;
    for (auto&& line : line_range) {
        if (line.first == line.second)
            continue;

        copy(as_range(line) | tokenized(opt.cell.regex, opt.cell.separator ? -1 : 0, opt.cell.flag), std::back_inserter(data));

        if (ncol == 0)
            ncol = data.size() - size;

        if (ncol != 0 && data.size() - size != ncol)
            throw std::runtime_error{"number of columns differ"};

        size = data.size();
    }

    if (ncol == 0)
        return {};

    base_dataframe<Coercion> df;
    if (opt.header && data.size() / ncol == 1) {
        for (std::size_t j = 0; j < ncol; ++j) {
            base_column<Coercion> col(std::string{data[j].first, data[j].second}, std::string{}, 0);
            df.colbind(std::move(col));
        }
        return df;
    }

    for (std::size_t j = 0; j < ncol; ++j) {
        auto column_data = make_iterator_range(data.begin() + j + (opt.header ? ncol : 0), data.end()) | strided(ncol) |
                           transformed([](auto&& match) {
                               return std::move(std::string{match.first, match.second});
                           });
        base_column<Coercion> col(opt.header ? std::string{data[j].first, data[j].second} : std::string{}, column_data);
        df.colbind(std::move(col));
    }
    return df;
}

template <typename Coercion>
void base_dataframe<Coercion>::write(const base_dataframe<Coercion>& df, std::ostream& os, const dataframe_write_options& opt)
{
    if (df.nrow() == 0 || df.ncol() == 0)
        return;

    std::vector<base_column<Coercion>> coerced;
    std::vector<column_view<const std::string>> data;

    for (std::size_t j = 0; j < df.ncol(); ++j) {
        const auto& col = df.select(j);

        if (col.elements_type() == typeid(std::string)) {
            data.push_back(view<std::string>(col));
        } else {
            coerced.push_back(std::move(jules::coerce_to<std::string>(col)));
            data.push_back(view<std::string>(coerced.back()));
        }
    }

    if (opt.header) {
        opt.cell.data(os, df.select(0).name());
        for (std::size_t j = 1; j < df.ncol(); ++j)
            opt.cell.data(opt.cell.separator(os), df.select(j).name());
        opt.line.separator(os);
    }

    for (std::size_t i = 0; i < df.nrow(); ++i) {
        opt.cell.data(os, data[0][i]);
        for (std::size_t j = 1; j < df.ncol(); ++j)
            opt.cell.data(opt.cell.separator(os), data[j][i]);
        opt.line.separator(os);
    }
}

template <typename Coercion>
template <typename T>
base_dataframe_colview<const T, Coercion> base_dataframe<Coercion>::colview() const
{
    std::vector<base_column_view<const T, Coercion>> views;
    for (const auto& col : columns_)
        views.push_back(view<T>(col));

    return {std::move(views)};
}

template <typename Coercion> template <typename T> base_dataframe_colview<T, Coercion> base_dataframe<Coercion>::colview()
{
    std::vector<base_column_view<T, Coercion>> views;
    for (auto& col : columns_)
        views.push_back(view<T>(col));

    return {std::move(views)};
}

template <typename T, typename C> base_dataframe_colview<const T, C> colview(const base_dataframe<C>& df)
{
    return df.template colview<T>();
}

template <typename T, typename C> base_dataframe_colview<T, C> colview(base_dataframe<C>& df)
{
    return df.template colview<T>();
}

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
