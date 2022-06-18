// Copyright (c) 2016-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include <jules/array/array.hpp>
#include <jules/base/string.hpp>
#include <jules/core/concepts.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>
#include <jules/data/action.hpp>
#include <jules/data/column.hpp>
#include <jules/data/numeric.hpp>

#include <algorithm>
#include <iosfwd>
#include <iterator>
#include <regex>
#include <unordered_map>

namespace jules
{

template <typename Rules> class base_data
{
public:
  using column_type = base_column<Rules>;

  struct named_column_type
  {
    string name;
    column_type column;
  };

  struct read_options
  {
    struct
    {
      std::regex regex;
      bool separator;
      std::regex_constants::match_flag_type flag;
    } line = {std::regex{R"(\n)"}, true, {}};

    struct
    {
      std::regex regex;
      bool separator;
      std::regex_constants::match_flag_type flag;
    } cell = {std::regex{R"(\t)"}, true, {}};

    string na = "NA";

    bool header = true;
  };

  struct write_options
  {
    string eol = "\n";
    string delimiter = "\t";
    string na = "NA";
    bool header = true;
  };

  base_data() = default;

  base_data(std::initializer_list<named_column_type> elements) : base_data(elements.begin(), elements.end(), elements.size()) {}

  template <ranges::range Rng, typename R = ranges::range_value_t<Rng>>
  requires convertible_to<R, named_column_type> base_data(const Rng& rng)
    : base_data(ranges::begin(rng), ranges::end(rng), ranges::size(rng))
  {}

  template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, typename R = ranges::iter_value_t<Iter>>
  requires convertible_to<R, named_column_type> &&(!ranges::forward_iterator<Iter>)base_data(Iter first, Sent last)
    : base_data(first, last, 0u)
  {}

  template <ranges::forward_iterator Iter, ranges::sentinel_for<Iter> Sent, typename R = ranges::iter_value_t<Iter>>
  requires convertible_to<R, named_column_type> base_data(Iter first, Sent last)
    : base_data(first, last, ranges::distance(first, last))
  {}

  template <ranges::input_iterator Iter, ranges::sentinel_for<Iter> Sent, typename R = ranges::iter_value_t<Iter>>
  requires convertible_to<R, named_column_type> base_data(Iter first, Sent last, index_t size_hint)
  {
    if (first == last)
      return;

    elements_.reserve(size_hint);

    elements_.push_back(*first);
    row_count_ = elements_.back().column.size();

    auto ok = std::all_of(++first, last, [this](auto&& column) {
      elements_.push_back(std::forward<decltype(column)>(column));
      return elements_.back().column.size() == row_count_;
    });

    DEBUG_ASSERT(ok, debug::throwing_module, debug::level::invalid_argument, "columns size mismatch");

    auto i = index_t{0u};
    for (auto& element : elements_) {
      auto& name = element.name;
      if (!name.view().empty()) {
        DEBUG_ASSERT(indexes_.find(name) == indexes_.end(), debug::throwing_module, debug::level::invalid_argument,
                     "repeated column name");
        indexes_[name] = i++;
      }
    }
  }

  base_data(const base_data& source) = default;

  base_data(base_data&& source) noexcept
    : row_count_{std::exchange(source.row_count_, 0)},
      elements_{std::exchange(source.elements_, {})},
      indexes_{std::exchange(source.indexes_, {})}
  {}

  auto operator=(const base_data& source) -> base_data& = default;

  auto operator=(base_data&& source) noexcept -> base_data&
  {
    row_count_ = std::exchange(source.row_count_, 0);
    elements_ = std::exchange(source.elements_, {});
    indexes_ = std::exchange(source.indexes_, {});

    return *this;
  }

  static auto read(std::istream& is, read_options opt = {}) -> base_data
  {
    namespace view = ::jules::ranges::views;

    if (!is)
      return {};

    const auto as_range = [](auto&& match) { return ranges::make_subrange(match.first, match.second); };

    auto raw_data = std::string();
    raw_data.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

    auto data = container<string>();
    auto ncol = index_t{0u};

    auto line_range = raw_data | view::tokenize(opt.line.regex, opt.line.separator ? -1 : 0, opt.line.flag);

    auto last_size = index_t{0u};
    for (auto&& line : line_range) {
      if (line.first == line.second)
        continue;

      const auto cells = as_range(line) | view::tokenize(opt.cell.regex, opt.cell.separator ? -1 : 0, opt.cell.flag);
      ranges::transform(cells, ranges::back_inserter(data), [](const auto& match) -> string {
        std::string_view sv(&*match.first, match.length());
        return trim(string(sv));
      });

      if (ncol == 0u)
        ncol = data.size() - last_size;

      DEBUG_ASSERT(ncol == 0u || data.size() - last_size == ncol, debug::throwing_module, debug::level::invalid_argument,
                   "number of columns differ");

      last_size = data.size();
    }

    if (ncol == 0)
      return {};

    auto df = base_data();

    // only header
    if (opt.header && data.size() / ncol == 1) {
      for (auto j : indices(ncol))
        df.bind(named_column_type{string(data[j]), {}});
      return df;
    }

    // optional header and data
    for (auto j : indices(ncol)) {
      auto col_data = ranges::make_subrange(data.begin() + j + (opt.header ? ncol : 0), data.end()) | view::stride(ncol) |
                      view::transform([&opt](const std::string_view& match) -> std::optional<string> {
                        if (match.empty() || match == opt.na.view())
                          return std::nullopt;
                        return string(match);
                      });
      auto col = column_type(col_data | view::move);
      df.bind(named_column_type{opt.header ? string(data[j]) : string(), std::move(col)});
    }

    return df;
  }

  auto write(std::ostream& os, write_options opt = {}) const -> std::ostream&
  {
    namespace view = ::jules::ranges::views;

    if (!(*this))
      return os;

    container<column_type> coerced; // hold temporary coerced values
    container<const std::optional<string>*> data;

    coerced.reserve(column_count());
    data.reserve(column_count());

    const auto columns = elements_ | view::transform([](auto&& elem) -> const column_type& { return elem.column; });

    for (const auto& col : columns) {
      if (col.elements_type() == typeid(string)) {
        data.push_back(col.template data<string>());
      } else {
        coerced.push_back(to_column<string>(col));
        data.push_back(coerced.back().template data<string>());
      }
    }

    if (opt.header) {
      os << elements_[0].name;
      for (auto j = index_t{1u}; j < column_count(); ++j)
        os << opt.delimiter << elements_[j].name;
      os << opt.eol;
    }

    for (auto i = index_t{0u}; i < row_count(); ++i) {
      os << data[0][i].value_or(opt.na);
      for (auto j = index_t{1u}; j < column_count(); ++j)
        os << opt.delimiter << data[j][i].value_or(opt.na);
      os << opt.eol;
    }

    return os;
  }

  // other options are `bind_left`, `bind_right`, `bind_up`, `bind_down`, or something similar.
  auto bind(named_column_type elem) -> base_data&
  {
    DEBUG_ASSERT(elem.name.view().empty() || indexes_.find(elem.name) == indexes_.end(), debug::throwing_module,
                 debug::level::invalid_argument, "repeated column name");

    if (*this)
      DEBUG_ASSERT(row_count() == elem.column.size(), debug::throwing_module, debug::level::invalid_argument,
                   "invalid column size");
    else
      row_count_ = elem.column.size();

    if (!elem.name.view().empty())
      indexes_[elem.name] = elements_.size();
    elements_.push_back(std::move(elem));

    return *this;
  }

  auto bind(column elem) -> base_data& { return bind(named_column_type{"", std::move(elem)}); }

  auto bind(base_data other) -> base_data&
  {
    for (auto& elem : other.elements_)
      bind(std::move(elem));
    return *this;
  }

  // `select` will be part of dplyr-like operations.
  auto at(index_t pos) const -> const named_column_type&
  {
    DEBUG_ASSERT(pos < elements_.size(), debug::throwing_module, debug::level::boundary_check, "invalid column index");
    return elements_[pos];
  }

  auto at(const string& name) const -> const named_column_type&
  {
    auto it = indexes_.find(name);
    DEBUG_ASSERT(it != indexes_.end(), debug::throwing_module, debug::level::invalid_argument, "column does not exist");
    return elements_[it->second];
  }

  operator bool() const { return column_count() > 0u; }

  auto row_count() const noexcept { return row_count_; }
  auto column_count() const noexcept { return elements_.size(); }

  auto dimensions() const noexcept -> std::array<index_t, 2> { return {row_count(), column_count()}; }

  auto column_types() const -> vector<std::type_index>
  {
    namespace view = ::jules::ranges::views;
    return to_vector<std::type_index>(elements_ |
                                      view::transform([](const auto& element) { return element.column.elements_type(); }));
  }

  auto names() const -> vector<string>
  {
    namespace view = ::jules::ranges::views;
    return to_vector<string>(elements_ | view::transform([](const auto& element) { return element.name; }));
  }

  auto begin() const { return cbegin(); }
  auto end() const { return cend(); }

  auto cbegin() const { return elements_.cbegin(); }
  auto cend() const { return elements_.cend(); }

  auto resize_columns(std::size_t nrows)
  {
    for (auto& [_, col] : elements_)
      col.model_->resize(nrows);
    row_count_ = nrows;
  }

private:
  index_t row_count_ = 0u;
  container<named_column_type> elements_;
  std::unordered_map<string, index_t> indexes_;
};

using data = base_data<coercion_rules>;

template <typename Rules> auto operator<<(std::ostream& os, const base_data<Rules>& df) -> std::ostream& { return df.write(os); }

template <typename Rules> auto operator>>(std::istream& is, base_data<Rules>& df) -> std::istream&
{
  df = base_data<Rules>::read(is);
  return is;
}

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
