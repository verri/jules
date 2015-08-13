#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include "array/array.hpp"
#include "dataframe/column.hpp"

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <unordered_map>
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
        auto name = column.name();
        auto it = colindexes_.find(name);
        if (it != colindexes_.end())
            throw int{}; // TODO

        if (*this != nullptr && nrow() != column.size())
            throw int{}; // TODO

        colindexes_[name] = columns_.size();
        columns_.push_back(column);

        return *this;
    }

    base_dataframe& cbind(column_t&& column)
    {
        auto name = column.name();
        auto it = colindexes_.find(name);
        if (it != colindexes_.end())
            throw int{}; // TODO

        if (*this != nullptr && nrow() != column.size())
            throw int{}; // TODO

        colindexes_[name] = columns_.size();
        columns_.push_back(std::move(column));

        return *this;
    }

    column_t& col(std::size_t i) { return columns_.at(i); }
    const column_t& col(std::size_t i) const { return columns_.at(i); }

    column_t& col(const std::string& name) {
        auto it = colindexes_.find(name);
        if (it == colindexes_.end())
            throw std::out_of_range{"column does not exists"};
        return columns_.at(*it);
    }

    const column_t& col(const std::string& name) const {
        auto it = colindexes_.find(name);
        if (it == colindexes_.end())
            throw std::out_of_range{"column does not exists"};
        return columns_.at(*it);
    }

    bool operator==(std::nullptr_t) { return ncol() == 0; }
    bool operator!=(std::nullptr_t) { return ncol() != 0; }

    std::size_t nrow() const { return nrow_; }
    std::size_t ncol() const { return columns_.size(); }

    template <typename T> matrix<T> as_array() const {
        matrix<T> result({nrow(), ncol()});

        for (std::size_t i{}; i < ncol(); ++i)
        {
            auto& column = columns<double>(i);
        }

        for (auto&& column : columns<T>())
            result[all][i] = column;

        return result;
    }

  private:
    std::size_t nrow_;

    std::vector<column_t> columns_;
    std::unordered_map<std::string, std::size_t> colindexes_;

};

using dataframe = base_dataframe<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
