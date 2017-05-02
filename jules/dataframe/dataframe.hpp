// Copyright (c) 2016-2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include <jules/array/array.hpp>
#include <jules/array/numeric.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>
#include <jules/core/type.hpp>
#include <jules/dataframe/action.hpp>
#include <jules/dataframe/column.hpp>
#include <jules/dataframe/numeric.hpp>

#include <algorithm>
#include <iosfwd>
#include <iterator>
#include <regex>
#include <unordered_map>

namespace jules
{

template <typename Coercion> class base_dataframe
{
public:
  using column_type = base_column<Coercion>;

  struct named_column_type {
    string name;
    column_type column;
  };

  struct read_options {
    read_options(std::regex line_regex = std::regex{R"(\n)"}, std::regex cell_regex = std::regex{R"(\t)"}, bool header = true)
      : line{std::move(line_regex), true, {}}, cell{std::move(cell_regex), true, {}}, header{header}
    {
    }

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

  struct write_options {
    write_options(string line_separator = "\n", string cell_separator = "\t", bool header = true)
      : line{{std::move(line_separator)}}, cell{{std::move(cell_separator)}}, header{header}
    {
    }

    struct {
      string separator;
    } line;

    struct {
      string separator;
    } cell;

    bool header;
  };

  base_dataframe() = default;

  base_dataframe(std::initializer_list<named_column_type> elements)
    : base_dataframe(elements.begin(), elements.end(), elements.size())
  {
  }

  template <typename Rng, typename R = range::range_value_t<Rng>,
            typename = std::enable_if_t<std::is_convertible<R, named_column_type>::value>,
            typename = meta::requires<range::Range<Rng>>>
  base_dataframe(const Rng& rng) : base_dataframe(range::begin(rng), range::end(rng), range::size(rng))
  {
  }

  template <typename Iter, typename Sent, typename R = range::iterator_value_t<Iter>,
            typename = std::enable_if_t<std::is_convertible<R, named_column_type>::value>,
            typename = meta::requires<range::Sentinel<Sent, Iter>, std::negation<range::InputIterator<Iter>>>, int = 0>
  base_dataframe(Iter first, Sent last) : base_dataframe(first, last, 0u)
  {
  }

  template <typename Iter, typename Sent, typename R = range::iterator_value_t<Iter>,
            typename = std::enable_if_t<std::is_convertible<R, named_column_type>::value>,
            typename = meta::requires<range::Sentinel<Sent, Iter>, range::InputIterator<Iter>>>
  base_dataframe(Iter first, Sent last) : base_dataframe(first, last, range::distance(first, last))
  {
  }

  template <typename Iter, typename Sent, typename R = range::iterator_value_t<Iter>,
            typename = std::enable_if_t<std::is_convertible<R, named_column_type>::value>,
            typename = meta::requires<range::Sentinel<Sent, Iter>>>
  base_dataframe(Iter first, Sent last, index_t size_hint)
  {
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
      if (!name.empty()) {
        DEBUG_ASSERT(indexes_.find(name) == indexes_.end(), debug::throwing_module, debug::level::invalid_argument,
                     "repeated column name");
        indexes_[name] = i++;
      }
    }
  }

  base_dataframe(named_column_type elem) : row_count_{elem.column.size()} { elements_.push_back(std::move(elem)); }

  base_dataframe(column_type column) : row_count_{column.size()}, elements_{{"", std::move(column)}} {}

  base_dataframe(const base_dataframe& source) = default;
  base_dataframe(base_dataframe&& source) noexcept = default;

  auto operator=(const base_dataframe& source) -> base_dataframe& = default;
  auto operator=(base_dataframe&& source) noexcept -> base_dataframe& = default;

  static auto read(std::istream& is, read_options opt = {}) -> base_dataframe
  {
    namespace view = ::jules::range::view;

    if (!is)
      return {};

    const auto as_range = [](auto&& match) { return range::make_iterator_range(match.first, match.second); };

    auto raw_data = string();
    raw_data.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

    auto data = std::vector<std::sub_match<string::iterator>>();
    auto ncol = index_t{0u};

    auto line_range = raw_data | view::tokenize(opt.line.regex, opt.line.separator ? -1 : 0, opt.line.flag);

    auto last_size = index_t{0u};
    for (auto&& line : line_range) {
      if (line.first == line.second)
        continue;

      const auto cells = as_range(line) | view::tokenize(opt.cell.regex, opt.cell.separator ? -1 : 0, opt.cell.flag);
      for (auto&& cell : cells)
        data.push_back(cell);

      // TODO: For some reason, range::copy doesn't work here.
      // range::copy(cells, std::back_inserter(data));

      if (ncol == 0u)
        ncol = data.size() - last_size;

      DEBUG_ASSERT(ncol == 0u || data.size() - last_size == ncol, debug::throwing_module, debug::level::invalid_argument,
                   "number of columns differ");

      last_size = data.size();
    }

    if (ncol == 0)
      return {};

    auto df = base_dataframe();

    // only header
    if (opt.header && data.size() / ncol == 1) {
      for (auto j : slice(0u, ncol))
        df.bind(named_column_type{string(data[j].first, data[j].second), {}});
      return df;
    }

    // optional header and data
    for (auto j : slice(0u, ncol)) {
      auto col_data = range::make_iterator_range(data.begin() + j + (opt.header ? ncol : 0), data.end()) | view::stride(ncol) |
                      view::transform([](auto&& match) -> string {
                        return {match.first, match.second};
                      });
      auto col = column_type(col_data | view::move);
      df.bind(named_column_type{opt.header ? string{data[j].first, data[j].second} : string{}, std::move(col)});
    }

    return df;
  }

  auto write(std::ostream& os, write_options opt = {}) const -> std::ostream&
  {
    namespace view = ::jules::range::view;

    if (!(*this))
      return os;

    std::vector<column_type> coerced; // hold temporary coerced values
    std::vector<contiguous_array<const string, 1>> data;

    coerced.reserve(column_count());
    data.reserve(column_count());

    const auto columns = elements_ | view::transform([](auto&& elem) -> const column_type& { return elem.column; });

    for (const auto& col : columns) {
      if (col.elements_type() == typeid(string)) {
        data.push_back(to_view<string>(col));
      } else {
        coerced.push_back(to_column<string>(col));
        data.push_back(to_view<string>(coerced.back()));
      }
    }

    if (opt.header) {
      os << elements_[0].name;
      for (auto j = index_t{1u}; j < column_count(); ++j)
        os << opt.cell.separator << elements_[j].name;
      os << opt.line.separator;
    }

    for (auto i = index_t{0u}; i < row_count(); ++i) {
      os << data[0][i];
      for (auto j = index_t{1u}; j < column_count(); ++j)
        os << opt.cell.separator << data[j][i];
      os << opt.line.separator;
    }

    return os;
  }

  // other options are `bind_left`, `bind_right`, `bind_up`, `bind_down`, or something similar.
  auto bind(base_dataframe other) -> base_dataframe&
  {
    for (const auto& name : other.names())
      DEBUG_ASSERT(name.empty() || indexes_.find(name) == indexes_.end(), debug::throwing_module, debug::level::invalid_argument,
                   "repeated column name");

    if (*this)
      DEBUG_ASSERT(row_count() == other.row_count(), debug::throwing_module, debug::level::invalid_argument,
                   "invalid column size");
    else
      row_count_ = other.row_count();

    for (auto& elem : other.elements_) {
      if (!elem.name.empty())
        indexes_[elem.name] = elements_.size();
      elements_.push_back(std::move(elem));
    }

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

  auto row_count() const { return row_count_; }
  auto column_count() const { return elements_.size(); }

  auto column_types() const -> vector<std::type_index>
  {
    namespace view = ::jules::range::view;
    return to_vector<std::type_index>(elements_ |
                                      view::transform([](const auto& element) { return element.column.elements_type(); }));
  }

  auto names() const -> vector<string>
  {
    namespace view = ::jules::range::view;
    return to_vector<string>(elements_ | view::transform([](const auto& element) { return element.name; }));
  }

  auto begin() const { return cbegin(); }
  auto end() const { return cend(); }

  auto cbegin() const { return elements_.cbegin(); }
  auto cend() const { return elements_.cend(); }

private:
  index_t row_count_ = 0u;
  std::vector<named_column_type> elements_;
  std::unordered_map<string, index_t> indexes_;
};

using dataframe = base_dataframe<coercion_rules>;

template <typename Coercion> auto operator<<(std::ostream& os, const base_dataframe<Coercion>& df) -> std::ostream&
{
  return df.write(os);
}

template <typename Coercion> auto operator>>(std::istream& is, base_dataframe<Coercion>& df) -> std::istream&
{
  df = base_dataframe<Coercion>::read(is);
  return is;
}

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H
