// Copyright (c) 2016-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include <jules/core/concepts.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>
#include <jules/data/column_model.hpp>

#include <memory>

namespace jules
{

namespace detail
{

template <typename T> struct remove_optional
{
  using type = T;
};

template <typename T> struct remove_optional<std::optional<T>>
{
  using type = T;
};

template <typename T> using remove_optional_t = typename remove_optional<T>::type;
} // namespace detail

template <typename Rules> class base_column;
template <typename T, typename C> auto to_column(const base_column<C>& column) -> base_column<C>;
template <typename T, typename C> auto to_column(base_column<C>&& column) -> base_column<C>;

template <typename Rules> class base_column
{
  template <typename T> using model_t = column_model<T, Rules>;
  using interface_type = column_interface<Rules>;
  using model_type = std::unique_ptr<interface_type>;

  template <typename T, typename C> friend auto to_column(const base_column<C>& column) -> base_column<C>;
  template <typename T, typename C> friend auto to_column(base_column<C>&& column) -> base_column<C>;

public:
  base_column() = default;

  template <typename T>
  base_column(std::initializer_list<T> values) : model_{std::make_unique<model_t<T>>(values.begin(), values.end())}
  {}

  template <typename T> base_column(std::initializer_list<std::optional<T>> values) : model_{std::make_unique<model_t<T>>(values)}
  {}

  template <typename T>
  base_column(const T& value, index_t size) : model_{std::make_unique<model_t<T>>(size, std::make_optional(value))}
  {}

  template <typename T>
  base_column(const std::optional<T>& value, index_t size) : model_{std::make_unique<model_t<T>>(size, value)}
  {}

  template <ranges::range Rng, typename R = ranges::range_value_t<Rng>>
  base_column(const Rng& rng)
    : model_{std::make_unique<model_t<detail::remove_optional_t<R>>>(ranges::begin(rng), ranges::end(rng))}
  {}

  template <ranges::forward_iterator Iter, ranges::sentinel_for<Iter> Sent, typename R = ranges::iter_value_t<Iter>>
  base_column(Iter first, Sent last) : model_{std::make_unique<model_t<detail::remove_optional_t<R>>>(first, last)}
  {}

  base_column(const base_column& source, index_t first, index_t size)
    : model_{std::move(source.model_->partial_clone(first, size))}
  {}

  base_column(const base_column& source) : model_{std::move(source.model_->clone())} {}

  base_column(base_column&& source) noexcept = default;

  auto operator=(const base_column& source) -> base_column&
  {
    if (this == &source)
      return *this;

    model_ = source.model_ ? source.model_->clone() : nullptr;
    return *this;
  }

  auto operator=(base_column&& source) noexcept -> base_column& = default;

  template <typename T> auto coerce() & -> base_column&
  {
    DEBUG_ASSERT(model_ != nullptr, debug::throwing_module, debug::level::invalid_state, "column has no data");
    model_ = model_->template coerce<T>();
    return *this;
  }

  template <typename T> auto can_coerce() const noexcept { return model_ ? model_->template can_coerce<T>() : false; }

  auto elements_type() const
  {
    DEBUG_ASSERT(model_ != nullptr, debug::throwing_module, debug::level::invalid_state,
                 "column has no type because it has no data");
    return model_->elements_type();
  }

  auto size() const { return model_ ? model_->size() : 0u; }

  template <typename T> auto data() -> std::optional<T>*
  {
    DEBUG_ASSERT(model_ != nullptr, debug::throwing_module, debug::level::invalid_state, "column has no data");
    DEBUG_ASSERT(elements_type() == typeid(T), debug::throwing_module, debug::level::invalid_state, "invalid data type");
    auto& model = this->model_->template downcast<T>();
    return model.data();
  }

  template <typename T> auto data() const -> const std::optional<T>*
  {
    DEBUG_ASSERT(model_ != nullptr, debug::throwing_module, debug::level::invalid_state, "column has no data");
    DEBUG_ASSERT(elements_type() == typeid(T), debug::throwing_module, debug::level::invalid_state, "invalid data type");
    const auto& model = this->model_->template downcast<T>();
    return model.data();
  }

private:
  base_column(model_type&& model) : model_{std::move(model)} {}

  model_type model_ = nullptr;
};

using column = base_column<coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H