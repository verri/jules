// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include <jules/core/range.hpp>
#include <jules/core/type.hpp>
#include <jules/dataframe/detail/column_model.hpp>

#include <memory>

namespace jules
{

template <typename Coercion> class base_column;
template <typename T, typename C> auto to_column(const base_column<C>& column) -> base_column<C>;
template <typename T, typename C> auto to_column(base_column<C>&& column) -> base_column<C>;

template <typename Coercion> class base_column
{
  template <typename T> using model_t = detail::column_model<T, Coercion>;
  using interface_t = detail::column_interface<Coercion>;
  using model_ptr_t = std::unique_ptr<interface_t>;

public:
  template <typename T>
  base_column(string name, std::initializer_list<T> values) : name_{std::move(name)}, model_{std::make_unique<model_t<T>>(values)}
  {
  }

  template <typename T>
  base_column(string name, const T& value, index_t size)
    : name_{std::move(name)}, model_{std::make_unique<model_t<T>>(size, value)}
  {
  }

  template <typename T> base_column(std::initializer_list<T> values) : model_{std::make_unique<model_t<T>>(values)} {}

  template <typename T> base_column(const T& value, index_t size) : model_{std::make_unique<model_t<T>>(size, value)} {}

  template <typename Rng, typename R = range::range_value_t<Rng>, CONCEPT_REQUIRES_(range::Range<Rng>())>
  base_column(string name, const Rng& rng)
    : name_{std::move(name)}, model_{std::make_unique<model_t<R>>(range::begin(rng), range::end(rng))}
  {
  }

  template <typename Rng, typename R = range::range_value_t<Rng>, CONCEPT_REQUIRES_(range::Range<Rng>())>
  base_column(const Rng& rng) : model_{std::make_unique<model_t<R>>(range::begin(rng), range::end(rng))}
  {
  }

  template <typename Iter, typename Sent, typename R = range::iterator_value_t<Iter>,
            CONCEPT_REQUIRES_(range::Sentinel<Sent, Iter>())>
  base_column(string name, Iter first, Sent last) : name_{std::move(name)}, model_{std::make_unique<model_t<R>>(first, last)}
  {
  }

  template <typename Iter, typename Sent, typename R = range::iterator_value_t<Iter>,
            CONCEPT_REQUIRES_(range::Sentinel<Sent, Iter>())>
  base_column(Iter first, Sent last) : model_{std::make_unique<model_t<R>>(first, last)}
  {
  }

  base_column(const base_column& source) : name_{source.name_}, model_{std::move(source.model_->clone())} {}

  base_column(base_column&& source) noexcept = default;

  auto operator=(const base_column& source) -> base_column&
  {
    name_ = source.name_;
    model_ = std::move(source.model_->clone());
    return *this;
  }

  auto operator=(base_column&& source) noexcept -> base_column& = default;

  template <typename T> auto coerce() & -> base_column&
  {
    model_ = model_->template coerce<T>();
    return *this;
  }

  template <typename T> auto can_coerce() const { return model_->template can_coerce<T>(); }

  auto elements_type() const { return model_->elements_type(); }

  auto size() const { return model_->size(); }
  auto extents() const { return size(); }
  auto length() const { return size(); }

  auto name() -> string& { return name_; }
  auto name() const -> const string& { return name_; }

  template <typename T> auto data()
  {
    if (elements_type() != typeid(T))
      this->coerce<T>();
    auto& model = this->model_->template downcast<T>();
    return model.data();
  }

  template <typename T> auto data() const
  {
    if (elements_type() != typeid(T))
      throw std::bad_cast{};
    const auto& model = this->model_->template downcast<T>();
    return model.data();
  }

  template <typename T, typename C> friend auto to_column(const base_column<C>& column) -> base_column<C>;
  template <typename T, typename C> friend auto to_column(base_column<C>&& column) -> base_column<C>;

private:
  base_column(string name, model_ptr_t&& model) : name_{std::move(name)}, model_{std::move(model)} {}

  string name_;
  model_ptr_t model_;
};

using column = base_column<coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
