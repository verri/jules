// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_DATAFRAME_H
#define JULES_DATAFRAME_DATAFRAME_H

#include <jules/array/array.hpp>
#include <jules/array/numeric.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>
#include <jules/core/type.hpp>
#include <jules/dataframe/column.hpp>

#include <algorithm>
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
            typename = meta::requires<range::Sentinel<Sent, Iter>, meta::negation<range::InputIterator<Iter>>>, int = 0>
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

  base_dataframe(column_type column) : row_count_{column.size()}, elements_{{"", std::move(column)}} {}

  base_dataframe(const base_dataframe& source) = default;
  base_dataframe(base_dataframe&& source) noexcept = default;

  auto operator=(const base_dataframe& source) -> base_dataframe& = default;
  auto operator=(base_dataframe&& source) noexcept -> base_dataframe& = default;

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

private:
  index_t row_count_ = 0u;
  std::vector<named_column_type> elements_;
  std::unordered_map<string, index_t> indexes_;
};

using dataframe = base_dataframe<coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_H

#ifndef JULES_DATAFRAME_DATAFRAME_H

template <typename Coercion> auto base_dataframe<Coercion>::read(std::istream& is, dataframe_read_options opt) -> base_dataframe
{
  using namespace adaptors;
  using namespace range;

  if (!is)
    return {};

  const auto as_range = [](auto&& match) { return make_iterator_range(match.first, match.second); };

  string raw_data;
  raw_data.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

  std::vector<std::sub_match<string::iterator>> data;
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
      base_column<Coercion> col(string{data[j].first, data[j].second}, string{}, 0);
      df.colbind(std::move(col));
    }
    return df;
  }

  for (std::size_t j = 0; j < ncol; ++j) {
    auto column_data =
      make_iterator_range(data.begin() + j + (opt.header ? ncol : 0), data.end()) | strided(ncol) | transformed([](auto&& match) {
        return std::move(string{match.first, match.second});
      });
    base_column<Coercion> col(opt.header ? string{data[j].first, data[j].second} : string{}, column_data);
    df.colbind(std::move(col));
  }
  return df;
}

template <typename C> auto write(const base_dataframe<C>& df, std::ostream& os, dataframe_write_options opt) -> std::ostream&
{
  if (df.rows_count() == 0 || df.columns_count() == 0)
    return os;

  std::vector<base_column<C>> coerced;
  std::vector<column_view<const string>> data;

  for (std::size_t j = 0; j < df.columns_count(); ++j) {
    const auto& col = df.select(j);

    if (col.elements_type() == typeid(string)) {
      data.push_back(jules::as_view<string>(col));
    } else {
      coerced.push_back(std::move(jules::as_column<string>(col)));
      data.push_back(jules::as_view<string>(coerced.back()));
    }
  }

  if (opt.header) {
    opt.cell.data(os, df.select(0).name());
    for (std::size_t j = 1; j < df.columns_count(); ++j)
      opt.cell.data(opt.cell.separator(os), df.select(j).name());
    opt.line.separator(os);
  }

  for (std::size_t i = 0; i < df.rows_count(); ++i) {
    opt.cell.data(os, data[0][i]);
    for (std::size_t j = 1; j < df.columns_count(); ++j)
      opt.cell.data(opt.cell.separator(os), data[j][i]);
    opt.line.separator(os);
  }

  return os;
}

template <typename C> auto write(const base_dataframe<C>& df, std::ostream& os) -> std::ostream& { return write(df, os, {}); };

#endif // JULES_DATAFRAME_DATAFRAME_H
