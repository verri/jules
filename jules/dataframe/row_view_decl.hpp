// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#define JULES_DATAFRAME_ROW_VIEW_DECL_H
#ifndef JULES_DATAFRAME_ROW_VIEW_DECL_H

#include <tuple>

namespace jules
{
template <typename> class base_dataframe;

template <typename... T> class base_row_view : public std::tuple<T&...>
{
  template <typename> friend class base_dataframe;

private:
  using super_t = std::tuple<T&...>;

public:
  base_row_view() = delete;
  ~base_row_view() = default;

  base_row_view(const base_row_view& source) = default;
  base_row_view(base_row_view&& source) noexcept = default;

  auto operator=(const base_row_view& source) -> base_row_view& = delete;
  auto operator=(base_row_view&& source) noexcept -> base_row_view& = delete;

  using super_t::operator=;

  operator base_row_view<const T...>() const;

private:
  base_row_view(T&... args) : super_t{args...} {}
};

template <typename T> using row_view = base_row_view<T>;

} // namespace jules

#endif // JULES_DATAFRAME_ROW_VIEW_DECL_H
