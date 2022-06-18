// Copyright (c) 2016-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_ACTIONS_HPP
#define JULES_DATAFRAME_ACTIONS_HPP

#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>
#include <jules/dataframe/detail/common.hpp>

#include <stdexcept>

namespace jules
{

template <typename Op> struct base_dataframe_action
{

  base_dataframe_action(Op op) : op_{std::move(op)} {}

  template <typename Coercion>
  friend auto operator|(const base_dataframe<Coercion>& df, const base_dataframe_action& a) -> base_dataframe<Coercion>
  {
    return a.op_(df);
  }

  template <typename Coercion>
  friend auto operator|(base_dataframe<Coercion>&& df, const base_dataframe_action& a) -> base_dataframe<Coercion>
  {
    return a.op_(std::move(df));
  }

private:
  Op op_;
};

template <typename Op> auto make_dataframe_action(Op op) -> base_dataframe_action<Op> { return {std::move(op)}; }

namespace action
{

constexpr auto head = [](index_t n) {
  // TODO: composed lambdas to accept temporary dataframe
  return make_dataframe_action([n](const auto& df) {
    if (n > df.row_count())
      throw std::out_of_range{"not enough rows"};

    namespace view = ::jules::ranges::views;
    return decltype(df)(view::all(df) | view::transform([n](const auto& named_column) {
                          return decltype(named_column){named_column.name, {named_column.column, 0u, n}};
                        }));
  });
};

} // namespace action
} // namespace jules

#endif // JULES_DATAFRAME_ACTIONS_HPP
