#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include "dataframe/dataframe_decl.hpp"

namespace jules
{
template <typename Coercion> base_dataframe<Coercion>& base_dataframe<Coercion>::cbind(const column_t& column)
{
    const auto& name = column.name();
    auto it = colindexes_.find(name);
    if (it != colindexes_.end())
        throw std::runtime_error{"column already exists"};

    if (*this != nullptr && nrow() != column.size())
        throw std::runtime_error{"invalid column size"};

    colindexes_[name] = columns_.size();
    columns_.push_back(column);

    return *this;
}

template <typename Coercion> base_dataframe<Coercion>& base_dataframe<Coercion>::cbind(column_t&& column)
{
    const auto& name = column.name();
    auto it = colindexes_.find(name);
    if (it != colindexes_.end())
        throw std::runtime_error{"column already exists"};

    if (*this != nullptr && nrow() != column.size())
        throw std::runtime_error{"invalid column size"};

    colindexes_[name] = columns_.size();
    columns_.push_back(std::move(column));

    return *this;
}

template <typename Coercion> auto base_dataframe<Coercion>::col(const std::string& name) -> column_t &
{
    auto it = colindexes_.find(name);
    if (it == colindexes_.end())
        throw std::out_of_range{"column does not exists"};
    return columns_.at(*it);
}

template <typename Coercion>
auto base_dataframe<Coercion>::col(const std::string& name) const -> const column_t &
{
    auto it = colindexes_.find(name);
    if (it == colindexes_.end())
        throw std::out_of_range{"column does not exists"};
    return columns_.at(*it);
}

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
