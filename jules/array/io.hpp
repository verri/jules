// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_IO_H
#define JULES_ARRAY_IO_H

#include <jules/array/fwd.hpp>
#include <jules/array/meta/reference.hpp>
#include <jules/core/type.hpp>

#include <ostream>
#include <string>
#include <vector>

namespace jules
{

template <typename CharT, typename Traits = std::char_traits<CharT>> struct basic_dim_fmt {
  std::basic_string<CharT, Traits> before = {CharT('{')}, separator = {CharT('\t')}, after = {CharT('}')};
};

template <typename CharT, typename Traits = std::char_traits<CharT>>
class array_ostream : public std::basic_ostream<CharT, Traits>
{
  using dim_fmt_type = basic_dim_fmt<CharT, Traits>;

public:
  array_ostream(std::basic_ostream<CharT, Traits>& ostr, std::vector<dim_fmt_type> fmts = {})
    : std::basic_ostream<CharT, Traits>(ostr.rdbuf()), fmts_{std::move(fmts)}
  {
    if (fmts_.size() == 0)
      fmts_.emplace_back();
  }

  const auto& dimensions_format() const { return fmts_; }

  auto dimensions_format(std::vector<dim_fmt_type> fmts) { return std::exchange(fmts_, std::move(fmts)); }

  using std::basic_ostream<CharT, Traits>::operator<<;

  template <typename T, std::size_t N> auto operator<<(const array<T, N>& a) -> array_ostream& { return (*this) << ref(a); }

  template <typename RefArray> auto operator<<(RefArray a) -> meta::requires_t<array_ostream&, ReferenceArray<RefArray>>
  {
    constexpr auto N = RefArray::order;

    if (fmts_.size() == 0)
      fmts_.emplace_back();
    auto& fmt = fmts_[N <= fmts_.size() ? fmts_.size() - N : 0];

    (*this) << fmt.before;
    const auto dim_size = a.dimensions()[0];
    if (dim_size == 0) {
      (*this) << fmt.after;
      return *this;
    }

    (*this) << a[0];
    for (auto i = index_t{1}; i < dim_size; ++i) {
      (*this) << fmt.separator;
      (*this) << a[i];
    }
    (*this) << fmt.after;
    return *this;
  }

private:
  std::vector<dim_fmt_type> fmts_;
  std::size_t max_dims_ = std::numeric_limits<std::size_t>::max();
};

namespace manip
{
struct comma_separated_t {
};
struct space_separated_t {
};
struct tab_separated_t {
};
} // namespace manip

constexpr auto comma_separated = manip::comma_separated_t{};
constexpr auto space_separated = manip::space_separated_t{};
constexpr auto tab_separated = manip::tab_separated_t{};

template <typename CharT, typename Traits> struct dimensions_format {
  dimensions_format(std::size_t dim, std::basic_string<CharT, Traits> before, std::basic_string<CharT, Traits> separator,
                    std::basic_string<CharT, Traits> after)
    : dim{dim}, fmt{std::move(before), std::move(separator), std::move(after)}
  {
  }
  std::size_t dim;
  basic_dim_fmt<CharT, Traits> fmt;
};

template <typename CharT, typename Traits>
auto& operator<<(array_ostream<CharT, Traits>& os, dimensions_format<CharT, Traits> setter)
{
  auto fmts = os.dimensions_format();

  if (fmts.size() < setter.dim)
    fmts.resize(setter.dim);

  if (fmts.size() == setter.dim) {
    fmts.push_back(std::move(setter.fmt));
    os.dimensions_format(std::move(fmts));
    return os;
  }

  fmts[setter.dim] = std::move(setter.fmt);
  os.dimensions_format(std::move(fmts));
  return os;
}

template <typename CharT, typename Traits> auto& operator<<(array_ostream<CharT, Traits>& os, manip::comma_separated_t)
{
  os.dimensions_format({{{}, {CharT('\n')}, {}}, {{}, {CharT(',')}, {}}});
  return os;
}

template <typename CharT, typename Traits> auto& operator<<(array_ostream<CharT, Traits>& os, manip::space_separated_t)
{
  os.dimensions_format({{{}, {CharT('\n')}, {}}, {{}, {CharT(' ')}, {}}});
  return os;
}

template <typename CharT, typename Traits> auto& operator<<(array_ostream<CharT, Traits>& os, manip::tab_separated_t)
{
  os.dimensions_format({{{}, {CharT('\n')}, {}}, {{}, {CharT('\t')}, {}}});
  return os;
}

template <typename CharT, typename Traits, typename T, std::size_t N>
auto operator<<(std::basic_ostream<CharT, Traits>& os, const array<T, N>& a) -> std::basic_ostream<CharT, Traits>&
{
  return os << ref(a);
}

template <typename CharT, typename Traits, typename RefArray>
auto operator<<(std::basic_ostream<CharT, Traits>& os, RefArray a)
  -> meta::requires_t<std::basic_ostream<CharT, Traits>&, ReferenceArray<RefArray>>
{
  if (auto os_ptr = dynamic_cast<array_ostream<CharT, Traits>*>(&os)) {
    return (*os_ptr) << a;
  }

  os << CharT('{');
  const auto dim_size = a.dimensions()[0];
  if (dim_size == 0)
    return os << CharT('}');

  os << a[0];
  for (auto i = index_t{1}; i < dim_size; ++i)
    os << CharT('\t') << a[i];
  return os << CharT('}');
}

} // namespace jules

#endif // JULES_ARRAY_IO_H
