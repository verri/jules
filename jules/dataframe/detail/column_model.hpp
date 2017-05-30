// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_DETAIL_COLUMN_MODEL_H
#define JULES_DATAFRAME_DETAIL_COLUMN_MODEL_H

#include <jules/base/async.hpp>
#include <jules/core/type.hpp>

#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <vector>

namespace jules
{
namespace detail
{
template <typename T, typename Coercion> class column_model;

template <typename Eraser, typename Coercion, std::size_t I>
class generate_virtual_coercions : public generate_virtual_coercions<Eraser, Coercion, I - 1>
{
private:
  using column_interface_ptr = std::unique_ptr<Eraser>;
  using type = typename Coercion::template type<I>;
  using base = generate_virtual_coercions<Eraser, Coercion, I - 1>;

protected:
  using base::coerce_;
  using base::can_coerce_;

  virtual auto coerce_(tag<type>) const -> column_interface_ptr = 0;
  virtual auto can_coerce_(tag<type>) const -> bool = 0;

public:
  virtual ~generate_virtual_coercions() = default;
};

template <typename Eraser, typename Coercion> class generate_virtual_coercions<Eraser, Coercion, 0>
{
private:
  using column_interface_ptr = std::unique_ptr<Eraser>;
  using type = typename Coercion::template type<0>;

protected:
  virtual auto coerce_(tag<type>) const -> column_interface_ptr = 0;
  virtual auto can_coerce_(tag<type>) const -> bool = 0;

public:
  virtual ~generate_virtual_coercions() = default;
};

template <typename Coercion>
class column_interface : public generate_virtual_coercions<column_interface<Coercion>, Coercion, Coercion::type_count() - 1>
{
private:
  using column_interface_ptr = std::unique_ptr<column_interface>;
  using coercion_rules = Coercion;
  using base = generate_virtual_coercions<column_interface<Coercion>, Coercion, Coercion::type_count() - 1>;

public:
  column_interface() = default;
  column_interface(const column_interface&) = default;
  column_interface(column_interface&&) noexcept = default;

  virtual ~column_interface() = default;

  auto operator=(const column_interface&) -> column_interface& = delete;
  auto operator=(column_interface &&) -> column_interface& = delete;

  virtual auto clone() const -> column_interface_ptr = 0;
  virtual auto partial_clone(index_t first, index_t size) const -> column_interface_ptr = 0;

  virtual auto resize(index_t size) -> void = 0;
  virtual auto shrink_to_fit() -> void = 0;
  virtual auto size() const -> index_t = 0;

  template <typename T> auto coerce() const -> column_interface_ptr { return this->coerce_(tag<T>{}); }
  template <typename T> auto can_coerce() const -> bool { return this->can_coerce_(tag<T>{}); }

  virtual auto elements_type() const -> std::type_index = 0;

  template <typename T> decltype(auto) downcast() { return dynamic_cast<column_model<T, coercion_rules>&>(*this); }
  template <typename T> decltype(auto) downcast() const { return dynamic_cast<const column_model<T, coercion_rules>&>(*this); }

protected:
  using base::coerce_;
  using base::can_coerce_;

  template <typename T>[[noreturn]] auto coerce_(T) const -> column_interface_ptr { throw std::bad_cast{}; }
  template <typename T> auto can_coerce_(T) const -> bool { return false; }
};

// No known coercion rule.
// If it is impossible to convert from T to type using a user-defined method, this class is instantiated.
template <typename T, typename U, typename Coercion, std::size_t I, typename Enabler = void> class specific_concrete_coercion
{
private:
  using type = typename Coercion::template type<I>;
  using column_interface_ptr = std::unique_ptr<column_interface<Coercion>>;

protected:
  template <typename Iter>[[noreturn]] auto coerce_(Iter, Iter, tag<type>) const -> column_interface_ptr
  {
    throw std::bad_cast{};
  }

  constexpr auto can_coerce_(tag<type>) const -> bool { return false; }
};

// User-defined coercion rules.
// If it is possible to convert from T to type using a user-defined method, this class is instantiated.
template <typename T, typename U, typename Coercion, std::size_t I>
class specific_concrete_coercion<
  T, U, Coercion, I,
  std::enable_if_t<std::is_same<decltype(Coercion::template rule<I>::coerce_from(std::declval<T>())), U>::value>>
{
private:
  using type = typename Coercion::template type<I>;
  using column_interface_ptr = std::unique_ptr<column_interface<Coercion>>;

protected:
  template <typename Iter> auto coerce_(Iter begin, Iter end, tag<type>) const -> column_interface_ptr
  {
    auto result = new column_model<type, Coercion>;
    JULES_DEFER(delete result);

    result->reserve(end - begin);
    std::transform(begin, end, std::back_inserter(*result),
                   [](const T& value) { return Coercion::template rule<I>::coerce_from(value); });

    return column_interface_ptr{std::exchange(result, nullptr)};
  }

  constexpr auto can_coerce_(tag<type>) const -> bool { return true; }
};

template <typename T, typename Coercion, std::size_t I>
class generate_concrete_coercions : public specific_concrete_coercion<T, typename Coercion::template type<I>, Coercion, I>,
                                    public generate_concrete_coercions<T, Coercion, I - 1>,
                                    public virtual column_interface<Coercion>
{
private:
  using type = typename Coercion::template type<I>;
  using column_interface_ptr = std::unique_ptr<column_interface<Coercion>>;
  using base = generate_concrete_coercions<T, Coercion, I - 1>;
  using specific = specific_concrete_coercion<T, typename Coercion::template type<I>, Coercion, I>;

protected:
  using base::coerce_;
  using base::can_coerce_;
  using column_interface<Coercion>::coerce_;
  using column_interface<Coercion>::can_coerce_;

  auto coerce_(tag<type> tag) const -> column_interface_ptr final { return specific::coerce_(this->begin(), this->end(), tag); }

  auto can_coerce_(tag<type> tag) const -> bool final { return specific::can_coerce_(tag); }

public:
  using generate_concrete_coercions<T, Coercion, I - 1>::generate_concrete_coercions;

  generate_concrete_coercions() = default;
  generate_concrete_coercions(const generate_concrete_coercions&) = default;
  generate_concrete_coercions(generate_concrete_coercions&&) noexcept = default;
};

template <typename T, typename Coercion>
class generate_concrete_coercions<T, Coercion, 0>
  : public specific_concrete_coercion<T, typename Coercion::template type<0>, Coercion, 0>,
    public std::vector<T>,
    public virtual column_interface<Coercion>
{
private:
  using type = typename Coercion::template type<0>;
  using column_interface_ptr = std::unique_ptr<column_interface<Coercion>>;
  using specific = specific_concrete_coercion<T, typename Coercion::template type<0>, Coercion, 0>;

protected:
  using column_interface<Coercion>::coerce_;
  using column_interface<Coercion>::can_coerce_;

  auto coerce_(tag<type> tag) const -> column_interface_ptr final { return specific::coerce_(this->begin(), this->end(), tag); }

  auto can_coerce_(tag<type> tag) const -> bool final { return specific::can_coerce_(tag); }

public:
  using std::vector<T>::vector;

  generate_concrete_coercions() = default;
  generate_concrete_coercions(const generate_concrete_coercions&) = default;
  generate_concrete_coercions(generate_concrete_coercions&&) noexcept = default;
};

template <typename T, typename Coercion>
class column_model : public generate_concrete_coercions<T, Coercion, Coercion::type_count() - 1>
{
  static_assert(!std::is_reference<T>::value, "column_model type can not be a reference");

public:
  using generate_concrete_coercions<T, Coercion, Coercion::type_count() - 1>::generate_concrete_coercions;

  column_model() = default;

  column_model(const column_model&) = default;
  column_model(column_model&&) noexcept = default;

  auto operator=(const column_model&) -> column_model& = delete;
  auto operator=(column_model &&) -> column_model& = delete;

  auto elements_type() const -> std::type_index final { return typeid(T); }

  auto clone() const -> std::unique_ptr<column_interface<Coercion>> final
  {
    return std::unique_ptr<column_interface<Coercion>>{new column_model{*this}};
  }

  auto partial_clone(index_t first, index_t size) const -> std::unique_ptr<column_interface<Coercion>> final
  {
    if (first >= this->size() || first + size > this->size())
      throw std::out_of_range{"invalid partial clone extents"};
    return std::unique_ptr<column_interface<Coercion>>{new column_model{this->begin() + first, this->begin() + first + size}};
  }

  auto resize(index_t size) -> void final { std::vector<T>::resize(size); }

  auto shrink_to_fit() -> void final { std::vector<T>::shrink_to_fit(); }

  auto size() const -> index_t final { return std::vector<T>::size(); }
};

} // namespace detail
} // namespace jules

#endif // JULES_DATAFRAME_DETAIL_COLUMN_MODEL_H
