#ifndef JULES_DATAFRAME_DETAIL_STORAGE_H
#define JULES_DATAFRAME_DETAIL_STORAGE_H

#include "core/type.hpp"
#include "util/async.hpp"

#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <vector>

namespace jules
{
namespace dataframe_detail
{
template <typename T, typename Coercion> class storage;

template <typename Eraser, typename Coercion, size_t I>
class generate_virtual_coercions : public generate_virtual_coercions<Eraser, Coercion, I - 1>
{
  private:
    using storage_eraser_ptr = std::unique_ptr<Eraser>;
    using type = typename Coercion::template type<I>;
    using base = generate_virtual_coercions<Eraser, Coercion, I - 1>;

  protected:
    using base::coerce_to_;
    using base::can_coerce_to_;

    virtual storage_eraser_ptr coerce_to_(tag<type>) const = 0;
    virtual bool can_coerce_to_(tag<type>) const = 0;

  public:
    ~generate_virtual_coercions() {}
};

template <typename Eraser, typename Coercion> class generate_virtual_coercions<Eraser, Coercion, 0>
{
  private:
    using storage_eraser_ptr = std::unique_ptr<Eraser>;
    using type = typename Coercion::template type<0>;

  protected:
    virtual storage_eraser_ptr coerce_to_(tag<type>) const = 0;
    virtual bool can_coerce_to_(tag<type>) const = 0;

  public:
    ~generate_virtual_coercions() {}
};

template <typename Coercion>
class storage_eraser
    : public generate_virtual_coercions<storage_eraser<Coercion>, Coercion, Coercion::ntypes() - 1>
{
  private:
    using storage_eraser_ptr = std::unique_ptr<storage_eraser>;
    using coercion_rules = Coercion;
    using base = generate_virtual_coercions<storage_eraser<Coercion>, Coercion, Coercion::ntypes() - 1>;

  public:
    storage_eraser() = default;
    storage_eraser(const storage_eraser&) = default;
    storage_eraser(storage_eraser&&) = default;

    virtual ~storage_eraser(){};

    storage_eraser& operator=(const storage_eraser&) = delete;
    storage_eraser& operator=(storage_eraser&&) = delete;

    virtual storage_eraser_ptr clone() const = 0;

    template <typename T> storage_eraser_ptr coerce_to() const { return this->coerce_to_(tag<T>{}); }
    template <typename T> bool can_coerce_to() const { return this->can_coerce_to_(tag<T>{}); }

    virtual std::type_index elements_type() const = 0;

    template <typename T> auto& downcast() { return dynamic_cast<storage<T, coercion_rules>&>(*this); }
    template <typename T> const auto& downcast() const
    {
        return dynamic_cast<const storage<T, coercion_rules>&>(*this);
    }

  protected:
    using base::coerce_to_;
    using base::can_coerce_to_;

    template <typename T> storage_eraser_ptr coerce_to_(T) const { return nullptr; }
    template <typename T> bool can_coerce_to_(T) const { return false; }
};

// Default coercion rules.  If it is impossible to convert from T to type, this class is
// instantiated.
template <typename T, typename U, typename Coercion, std::size_t I, typename Enabler = void>
class specific_concrete_coercion
{
  private:
    using type = typename Coercion::template type<I>;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_(Iter, Iter, tag<type>) const { return nullptr; }

    constexpr bool can_coerce_to_(tag<type>) const { return false; }
};

// User-defined coercion rules.  If it is possible to convert from T to type using ONLY a
// user-defined method, this class is instantiated.
template <typename T, typename U, typename Coercion, std::size_t I>
class specific_concrete_coercion<
    T, U, Coercion, I,
    std::enable_if_t<
        std::is_same<decltype(Coercion::template rule<I>::coerce_from(std::declval<T>())), U>::value &&
        !std::is_convertible<T, U>::value>>
{
  private:
    using type = typename Coercion::template type<I>;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_(Iter begin, Iter end, tag<type>) const
    {
        auto result = new storage<type, Coercion>;
        auto d = defer([&result] { delete result; });

        result->reserve(end - begin);
        std::transform(begin, end, std::back_inserter(*result),
                       [](const T& value) { return Coercion::template rule<I>::coerce_from(value); });

        return storage_eraser_ptr{move_ptr(result)};
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
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_(Iter begin, Iter end, tag<type>) const
    {
        auto result = new storage<type, Coercion>;
        auto d = defer([&result] { delete result; });

        result->reserve(end - begin);
        std::transform(begin, end, std::back_inserter(*result), [](const T& value) -> type { return value; });

        return storage_eraser_ptr{move_ptr(result)};
    }

    constexpr bool can_coerce_to_(tag<type>) const { return true; }
};

template <typename T, typename Coercion, std::size_t I>
class generate_concrete_coercions
    : public specific_concrete_coercion<T, typename Coercion::template type<I>, Coercion, I>,
      public generate_concrete_coercions<T, Coercion, I - 1>,
      public virtual storage_eraser<Coercion>
{
  private:
    using type = typename Coercion::template type<I>;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;
    using base = generate_concrete_coercions<T, Coercion, I - 1>;
    using specific = specific_concrete_coercion<T, typename Coercion::template type<I>, Coercion, I>;

  protected:
    using base::coerce_to_;
    using base::can_coerce_to_;
    using storage_eraser<Coercion>::coerce_to_;
    using storage_eraser<Coercion>::can_coerce_to_;

    virtual storage_eraser_ptr coerce_to_(tag<type> tag) const override final
    {
        return specific::coerce_to_(this->begin(), this->end(), tag);
    }

    virtual bool can_coerce_to_(tag<type> tag) const override final
    {
        return specific::can_coerce_to_(tag);
    }

  public:
    using generate_concrete_coercions<T, Coercion, I - 1>::generate_concrete_coercions;

    generate_concrete_coercions() = default;
    generate_concrete_coercions(const generate_concrete_coercions&) = default;
    generate_concrete_coercions(generate_concrete_coercions&&) = default;
};

template <typename T, typename Coercion>
class generate_concrete_coercions<T, Coercion, 0>
    : public specific_concrete_coercion<T, typename Coercion::template type<0>, Coercion, 0>,
      public std::vector<T>,
      public virtual storage_eraser<Coercion>
{
  private:
    using type = typename Coercion::template type<0>;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;
    using specific = specific_concrete_coercion<T, typename Coercion::template type<0>, Coercion, 0>;

  protected:
    using storage_eraser<Coercion>::coerce_to_;
    using storage_eraser<Coercion>::can_coerce_to_;

    virtual storage_eraser_ptr coerce_to_(tag<type> tag) const override final
    {
        return specific::coerce_to_(this->begin(), this->end(), tag);
    }

    virtual bool can_coerce_to_(tag<type> tag) const override final
    {
        return specific::can_coerce_to_(tag);
    }

  public:
    using std::vector<T>::vector;

    generate_concrete_coercions() = default;
    generate_concrete_coercions(const generate_concrete_coercions&) = default;
    generate_concrete_coercions(generate_concrete_coercions&&) = default;
};

template <typename T, typename Coercion>
class storage : public generate_concrete_coercions<T, Coercion, Coercion::ntypes() - 1>
{
    static_assert(!std::is_reference<T>::value, "storage type can not be a reference");

  public:
    using generate_concrete_coercions<T, Coercion, Coercion::ntypes() - 1>::generate_concrete_coercions;

    storage() = default;

    storage(const storage&) = default;
    storage(storage&&) = default;

    storage& operator=(const storage&) = delete;
    storage& operator=(storage&&) = delete;

    virtual std::type_index elements_type() const override final { return typeid(T); }
    virtual std::unique_ptr<storage_eraser<Coercion>> clone() const override final
    {
        return std::unique_ptr<storage_eraser<Coercion>>{new storage{*this}};
    }
};

} // namespace dataframe_detail
} // namespace jules

#endif // JULES_DATAFRAME_DETAIL_STORAGE_H
