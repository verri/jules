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
  using base::coerce_to_;
  using base::can_coerce_to_;

  virtual column_interface_ptr coerce_to_(tag<type>) const = 0;
  virtual bool can_coerce_to_(tag<type>) const = 0;

public:
  ~generate_virtual_coercions() = default;
};

template <typename Eraser, typename Coercion> class generate_virtual_coercions<Eraser, Coercion, 0>
{
private:
  using column_interface_ptr = std::unique_ptr<Eraser>;
  using type = typename Coercion::template type<0>;

protected:
  virtual column_interface_ptr coerce_to_(tag<type>) const = 0;
  virtual bool can_coerce_to_(tag<type>) const = 0;

public:
  ~generate_virtual_coercions() = default;
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
  ;

  column_interface& operator=(const column_interface&) = delete;
  column_interface& operator=(column_interface&&) = delete;

  virtual column_interface_ptr clone() const = 0;
  virtual std::size_t size() const = 0;

  template <typename T> column_interface_ptr coerce_to() const { return this->coerce_to_(tag<T>{}); }
  template <typename T> bool can_coerce_to() const { return this->can_coerce_to_(tag<T>{}); }

  virtual std::type_index elements_type() const = 0;

  template <typename T> auto& downcast() { return dynamic_cast<column_model<T, coercion_rules>&>(*this); }
  template <typename T> const auto& downcast() const { return dynamic_cast<const column_model<T, coercion_rules>&>(*this); }

protected:
  using base::coerce_to_;
  using base::can_coerce_to_;

  template <typename T>[[noreturn]] column_interface_ptr coerce_to_(T) const { throw std::bad_cast{}; }
  template <typename T> bool can_coerce_to_(T) const { return false; }
};

// Default coercion rules.  If it is impossible to convert from T to type, this class is
// instantiated.
template <typename T, typename U, typename Coercion, std::size_t I, typename Enabler = void> class specific_concrete_coercion
{
private:
  using type = typename Coercion::template type<I>;
  using column_interface_ptr = std::unique_ptr<column_interface<Coercion>>;

protected:
  template <typename Iter>[[noreturn]] column_interface_ptr coerce_to_(Iter, Iter, tag<type>) const { throw std::bad_cast{}; }

  constexpr bool can_coerce_to_(tag<type>) const { return false; }
};

// User-defined coercion rules.  If it is possible to convert from T to type using ONLY a
// user-defined method, this class is instantiated.
template <typename T, typename U, typename Coercion, std::size_t I>
class specific_concrete_coercion<
  T, U, Coercion, I,
  std::enable_if_t<std::is_same<decltype(Coercion::template rule<I>::coerce_from(std::declval<T>())), U>::value &&
                   !std::is_convertible<T, U>::value>>
{
private:
  using type = typename Coercion::template type<I>;
  using column_interface_ptr = std::unique_ptr<column_interface<Coercion>>;

protected:
  template <typename Iter> column_interface_ptr coerce_to_(Iter begin, Iter end, tag<type>) const
  {
    auto result = new column_model<type, Coercion>;
    auto d = defer([&result] { delete result; });

    result->reserve(end - begin);
    std::transform(begin, end, std::back_inserter(*result),
                   [](const T& value) { return Coercion::template rule<I>::coerce_from(value); });

    return column_interface_ptr{move_ptr(result)};
  }

  constexpr bool can_coerce_to_(tag<type>) const { return true; }
};

// Automatic coercion rules.  If it is possible to convert from T to type using a
// implicit method, this class is instantiated.
template <typename T, typename U, typename Coercion, std::size_t I>
class specific_concrete_coercion<T, U, Coercion, I, std::enable_if_t<std::is_convertible<T, U>::value>>
{
private:
  using type = typename Coercion::template type<I>;
  using column_interface_ptr = std::unique_ptr<column_interface<Coercion>>;

protected:
  template <typename Iter> column_interface_ptr coerce_to_(Iter begin, Iter end, tag<type>) const
  {
    auto result = new column_model<type, Coercion>;
    auto d = defer([&result] { delete result; });

    result->reserve(end - begin);
    std::transform(begin, end, std::back_inserter(*result), [](const T& value) -> type { return value; });

    return column_interface_ptr{move_ptr(result)};
  }

  constexpr bool can_coerce_to_(tag<type>) const { return true; }
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
  using base::coerce_to_;
  using base::can_coerce_to_;
  using column_interface<Coercion>::coerce_to_;
  using column_interface<Coercion>::can_coerce_to_;

  column_interface_ptr coerce_to_(tag<type> tag) const final { return specific::coerce_to_(this->begin(), this->end(), tag); }

  bool can_coerce_to_(tag<type> tag) const final { return specific::can_coerce_to_(tag); }

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
  using column_interface<Coercion>::coerce_to_;
  using column_interface<Coercion>::can_coerce_to_;

  column_interface_ptr coerce_to_(tag<type> tag) const final { return specific::coerce_to_(this->begin(), this->end(), tag); }

  bool can_coerce_to_(tag<type> tag) const final { return specific::can_coerce_to_(tag); }

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

  column_model& operator=(const column_model&) = delete;
  column_model& operator=(column_model&&) = delete;

  std::type_index elements_type() const final { return typeid(T); }
  std::unique_ptr<column_interface<Coercion>> clone() const final
  {
    return std::unique_ptr<column_interface<Coercion>>{new column_model{*this}};
  }

  std::size_t size() const final { return std::vector<T>::size(); }
};

} // namespace detail
} // namespace jules

#endif // JULES_DATAFRAME_DETAIL_COLUMN_MODEL_H
