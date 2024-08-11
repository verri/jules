// Copyright (c) 2016-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_ACTIONS_HPP
#define JULES_DATAFRAME_ACTIONS_HPP

#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>
#include <jules/data/detail/common.hpp>

#include <stdexcept>

namespace jules
{

template <typename Op> struct base_data_action
{

  base_data_action(Op op) : op_{std::move(op)} {}

  template <typename Coercion>
  friend auto operator|(const base_data<Coercion>& df, const base_data_action& a) -> base_data<Coercion>
  {
    return a.op_(df);
  }

  template <typename Coercion> friend auto operator|(base_data<Coercion>&& df, const base_data_action& a) -> base_data<Coercion>
  {
    return a.op_(std::move(df));
  }

private:
  Op op_;
};

template <typename Op> auto make_data_action(Op op) -> base_data_action<Op> { return {std::move(op)}; }

namespace action
{

constexpr auto head = [](index_t n) {
  return make_data_action(overloaded{[n](const auto& df) {
                                       if (n > df.row_count())
                                         throw std::out_of_range{"not enough rows"};

                                       namespace view = ::jules::ranges::views;
                                       return decltype(df)(
                                         view::all(df) | view::transform([n](const auto& named_column) {
                                           return decltype(named_column){named_column.name, {named_column.column, 0u, n}};
                                         }));
                                     },
                                     [n]<typename C>(base_data<C>&& df) -> base_data<C> {
                                       if (n > df.row_count())
                                         throw std::out_of_range{"not enough rows"};

                                       df.resize_columns(n);
                                       return std::move(df);
                                     }});
};

// TODO: move-version
constexpr auto tail = [](index_t n) {
  return make_data_action([n](const auto& df) {
    const auto nrow = df.row_count();
    if (n > nrow)
      throw std::out_of_range{"not enough rows"};

    namespace view = ::jules::ranges::views;
    return decltype(df)(view::all(df) | view::transform([&](const auto& named_column) {
                          return decltype(named_column){named_column.name, {named_column.column, nrow - n, n}};
                        }));
  });
};

} // namespace action
} // namespace jules

#endif // JULES_DATAFRAME_ACTIONS_HPP
