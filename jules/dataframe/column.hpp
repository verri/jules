#ifndef JULES_DATAFRAME_COLUMN_H
#define JULES_DATAFRAME_COLUMN_H

#include <jules/dataframe/column_decl.hpp>

namespace jules
{

// Member functions.

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(std::string name, std::initializer_list<T> values)
  : name_{std::move(name)}, column_model_{std::make_unique<column_model_t<T>>(values)}
{
}

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(std::string name, const T& value, std::size_t size)
  : name_{std::move(name)}, column_model_{std::make_unique<column_model_t<T>>(size, value)}
{
}

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(std::initializer_list<T> values) : column_model_{std::make_unique<column_model_t<T>>(values)}
{
}

template <typename Coercion>
template <typename T>
base_column<Coercion>::base_column(const T& value, std::size_t size)
  : column_model_{std::make_unique<column_model_t<T>>(size, value)}
{
}

template <typename Coercion>
base_column<Coercion>::base_column(std::string name, std::unique_ptr<column_interface_t>&& column_model)
  : name_{std::move(name)}, column_model_{std::move(column_model)}
{
}

template <typename Coercion>
template <typename Range, typename R>
base_column<Coercion>::base_column(std::string name, const Range& rng)
  : name_{std::move(name)}, column_model_{std::make_unique<column_model_t<R>>(range::begin(rng), range::end(rng))}
{
}

template <typename Coercion>
template <typename Range, typename R>
base_column<Coercion>::base_column(const Range& rng)
  : column_model_{std::make_unique<column_model_t<R>>(range::begin(rng), range::end(rng))}
{
}

template <typename Coercion>
base_column<Coercion>::base_column(const base_column& source)
  : name_{source.name_}, column_model_{std::move(source.column_model_->clone())}
{
}

template <typename Coercion> auto base_column<Coercion>::operator=(const base_column& source) -> base_column&
{
  name_ = source.name_;
  column_model_ = std::move(source.column_model_->clone());

  return *this;
}

template <typename Coercion> template <typename T> auto base_column<Coercion>::coerce_to()& -> base_column&
{
  column_model_ = column_model_->template coerce_to<T>();
  return *this;
}

template <typename Coercion> template <typename T> auto base_column<Coercion>::view() -> view_t<T>
{
  auto& model = this->column_model_->template downcast<T>();
  return {model.data(), model.size()};
}

template <typename Coercion> template <typename T> auto base_column<Coercion>::view() const -> view_t<const T>
{
  const auto& model = this->column_model_->template downcast<T>();
  return {model.data(), model.size()};
}

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
