// Copyright (c) 2016-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_DATAFRAME_DETAIL_COLUMN_MODEL_H
#define JULES_DATAFRAME_DETAIL_COLUMN_MODEL_H

#include <jules/base/async.hpp>
#include <jules/dataframe/type.hpp>

#include <any>
#include <optional>
#include <typeindex>

namespace jules::detail
{

template <typename, typename, typename> class column_model;

template <typename Erased, typename T> class virtual_coercion
{
private:
  using column_interface_ptr = std::unique_ptr<Erased>;

protected:
  [[nodiscard]] virtual auto coerce_(tag<T>) const -> column_interface_ptr = 0;
  [[nodiscard]] virtual auto can_coerce_(tag<T>) const noexcept -> bool = 0;

public:
  virtual ~virtual_coercion() = default;
};

template <typename, typename NotTuple> class column_interface
{
  static_assert(always_false<NotTuple>);
};

template <typename Rules, typename... Ts>
class column_interface<Rules, std::tuple<Ts...>>
  : public virtual virtual_coercion<column_interface<Rules, std::tuple<Ts...>>, Ts>...
{
private:
  using column_interface_ptr = std::unique_ptr<column_interface>;
  using coercion_rules = Rules;
  template <typename T> using concrete_type = column_model<T, coercion_rules, std::tuple<Ts...>>;

public:
  column_interface() = default;
  column_interface(const column_interface&) = default;
  column_interface(column_interface&&) noexcept = default;

  virtual ~column_interface() = default;

  auto operator=(const column_interface&) -> column_interface& = delete;
  auto operator=(column_interface &&) -> column_interface& = delete;

  [[nodiscard]] virtual auto clone() const -> column_interface_ptr = 0;
  [[nodiscard]] virtual auto partial_clone(index_t first, index_t size) const -> column_interface_ptr = 0;

  virtual auto resize(index_t, const std::any& = {}) -> void = 0;
  virtual auto shrink_to_fit() -> void = 0;

  [[nodiscard]] virtual auto size() const noexcept -> index_t = 0;

  template <typename T>[[nodiscard]] auto coerce() const -> column_interface_ptr { return this->coerce_(tag<T>{}); }
  template <typename T>[[nodiscard]] auto can_coerce() const noexcept -> bool { return this->can_coerce_(tag<T>{}); }

  [[nodiscard]] virtual auto coerce(std::type_index) const -> column_interface_ptr = 0;
  [[nodiscard]] virtual auto can_coerce(std::type_index) const noexcept -> bool = 0;

  [[nodiscard]] virtual auto elements_type() const noexcept -> std::type_index = 0;

  template <typename T>[[nodiscard]] decltype(auto) downcast() { return dynamic_cast<concrete_type<T>&>(*this); }

  template <typename T>[[nodiscard]] decltype(auto) downcast() const { return dynamic_cast<const concrete_type<T>&>(*this); }

protected:
  using virtual_coercion<column_interface<Rules, std::tuple<Ts...>>, Ts>::coerce_...;
  using virtual_coercion<column_interface<Rules, std::tuple<Ts...>>, Ts>::can_coerce_...;

  template <typename T> requires(!any_args(same_as<T, Ts>...)) [[noreturn]] auto coerce_(tag<T>) const -> column_interface_ptr
  {
    throw std::bad_cast{};
  }
  template <typename T> requires(!any_args(same_as<T, Ts>...)) [[nodiscard]] auto can_coerce_(tag<T>) const noexcept -> bool
  {
    return false;
  }
};

template <typename T, typename Rules, typename U>
class concrete_coercion : protected virtual virtual_coercion<column_interface<Rules, typename Rules::types>, U>,
                          public virtual container<std::optional<T>>
{
private:
  using column_interface_ptr = std::unique_ptr<column_interface<Rules, typename Rules::types>>;

protected:
  [[nodiscard]] auto coerce_(tag<U>) const -> column_interface_ptr final
  {
    if constexpr (coercible_to<T, Rules, U>) {
      auto result = new column_model<U, Rules, typename Rules::types>;
      JULES_DEFER(delete result);

      result->reserve(this->size());
      std::transform(this->begin(), this->end(), std::back_inserter(*result), [](const std::optional<T>& x) -> std::optional<U> {
        constexpr auto rules = Rules{};
        return x.has_value() ? std::make_optional(rules.coerce(tag<T>{}, tag<U>{}, x.value())) : std::nullopt;
      });

      return column_interface_ptr{std::exchange(result, nullptr)};
    } else {
      throw std::bad_cast{};
    }
  }

  [[nodiscard]] auto can_coerce_(tag<U>) const noexcept -> bool final { return coercible_to<T, Rules, U>; }

  [[nodiscard]] auto coerce_(std::type_index id, column_interface_ptr& interface) const -> bool
  {
    if (id != typeid(U))
      return false;
    interface = coerce_(tag<U>{});
    return true;
  }

  [[nodiscard]] auto can_coerce_(std::type_index id) const noexcept -> bool
  {
    return id == typeid(U) ? can_coerce_(tag<U>{}) : false;
  }

public:
  virtual ~concrete_coercion() = default;
};

template <typename, typename, typename NotTuple> class column_model
{
  static_assert(always_false<NotTuple>);
};

template <typename T, typename Rules, typename... Ts>
class column_model<T, Rules, std::tuple<Ts...>> : public concrete_coercion<T, Rules, Ts>...,
                                                  public virtual column_interface<Rules, typename Rules::types>,
                                                  private virtual container<std::optional<T>>
{
  static_assert(std::is_same_v<T, std::decay_t<T>>);

private:
  using column_interface_ptr = std::unique_ptr<column_interface<Rules, typename Rules::types>>;

public:
  using value_type = typename container<std::optional<T>>::value_type;
  using iterator = typename container<std::optional<T>>::iterator;
  using const_iterator = typename container<std::optional<T>>::const_iterator;

  column_model() = default;

  column_model(const column_model&) = default;
  column_model(column_model&&) noexcept = default;

  auto operator=(const column_model&) -> column_model& = delete;
  auto operator=(column_model&&) noexcept -> column_model& = delete;

  using container<std::optional<T>>::container;
  using container<std::optional<T>>::operator=;

  [[nodiscard]] auto elements_type() const noexcept -> std::type_index final { return typeid(T); }

  [[nodiscard]] auto clone() const -> column_interface_ptr final { return column_interface_ptr{new column_model{*this}}; }

  [[nodiscard]] auto partial_clone(index_t first, index_t size) const -> column_interface_ptr final
  {
    if (first >= this->size() || first + size > this->size())
      throw std::out_of_range{"invalid partial clone extents"};
    return column_interface_ptr{new column_model{this->begin() + first, this->begin() + first + size}};
  }

  auto resize(index_t size, const std::any& value = {}) -> void final
  {
    if (!value.has_value())
      container<std::optional<T>>::resize(size);
    else if (value.type() == typeid(T))
      container<std::optional<T>>::resize(size, *std::any_cast<T>(&value));
    else if (value.type() == typeid(std::optional<T>))
      container<std::optional<T>>::resize(size, *std::any_cast<std::optional<T>>(&value));
    else
      throw std::bad_cast{};
  }

  auto shrink_to_fit() -> void final { container<std::optional<T>>::shrink_to_fit(); }

  [[nodiscard]] auto size() const noexcept -> index_t final { return container<std::optional<T>>::size(); }

  [[nodiscard]] auto coerce(std::type_index id) const -> column_interface_ptr final
  {
    column_interface_ptr interface;
    if (!(concrete_coercion<T, Rules, Ts>::coerce_(id, interface) || ...) || !interface)
      throw std::bad_cast{};
    return interface;
  }

  [[nodiscard]] auto can_coerce(std::type_index id) const noexcept -> bool final
  {
    return (concrete_coercion<T, Rules, Ts>::can_coerce_(id) || ...);
  }

private:
};

} // namespace jules::detail

namespace jules
{
template <typename T, typename Rules> using column_model = detail::column_model<T, Rules, typename Rules::types>;
template <typename Rules> using column_interface = detail::column_interface<Rules, typename Rules::types>;
} // namespace jules

#endif // JULES_DATAFRAME_DETAIL_COLUMN_MODEL_H
