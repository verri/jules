// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include <jules/core/range.hpp>
#include <jules/core/type.hpp>
#include <jules/dataframe/detail/column_model.hpp>

#include <memory>

namespace jules
{

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
    auto& model = this->column_model_->template downcast<T>();
    return model.data();
  }

  template <typename T> auto data() const
  {
    if (elements_type() != typeid(T))
      throw std::bad_cast{};
    const auto& model = this->column_model_->template downcast<T>();
    return model.data();
  }

private:
  string name_;
  model_ptr_t model_;
};

using column = base_column<coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H

// TODO: XXX: delete everything below
#ifndef JULES_DATAFRAME_COLUMN_H

template <typename Coercion> template <typename T> auto base_column<Coercion>::clone() const -> base_column
{
  return {this->name(), this->column_model_->template coerce_to<T>()};
}

// Friend functions.

template <typename T, typename C> auto as_view(base_column<C>& column) -> base_column_view<T>
{
  return column.template view<T>();
}

template <typename T, typename C> auto as_view(const base_column<C>& column) -> base_column_view<const T>
{
  return column.template view<T>();
};

template <typename T, typename C> auto as_column(const base_column<C>& column) -> base_column<C>
{
  return column.template clone<T>();
}

template <typename T, typename C> auto as_vector(const base_column<C>& column) -> vector<T> { return {as_view<T>(column)}; }

template <typename C, std::size_t D> auto concatenate(const base_column<C>& a, const base_column<C>& b) -> base_dataframe<C>
{
  static_assert(D == 1, "invalid dimension: only horizontal column concatenation is implemented");
  auto result = base_dataframe<C>();
  result.concatenate<1>(a).concatenate<1>(b);
  return result;
}

template <typename C, std::size_t D> auto concatenate(const base_column<C>& a, base_column<C>&& b) -> base_dataframe<C>
{
  static_assert(D == 1, "invalid dimension: only horizontal column concatenation is implemented");
  auto result = base_dataframe<C>();
  result.concatenate<1>(a).concatenate<1>(std::move(b));
  return result;
}

template <typename C, std::size_t D> auto concatenate(base_column<C>&& a, const base_column<C>& b) -> base_dataframe<C>
{
  static_assert(D == 1, "invalid dimension: only horizontal column concatenation is implemented");
  auto result = base_dataframe<C>();
  result.concatenate<1>(std::move(a)).concatenate<1>(b);
  return result;
}

template <typename C, std::size_t D> auto concatenate(base_column<C>&& a, base_column<C>&& b) -> base_dataframe<C>
{
  static_assert(D == 1, "invalid dimension: only horizontal column concatenation is implemented");
  auto result = base_dataframe<C>();
  result.concatenate<1>(std::move(a)).concatenate<1>(std::move(b));
  return result;
}

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_H
