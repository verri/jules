#ifndef JULES_DATAFRAME_DETAIL_STORAGE_H
#define JULES_DATAFRAME_DETAIL_STORAGE_H

#include "core/type.hpp"

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

template <typename T> struct tag {
    using untag = T;
};

template <typename Coercion> class storage_eraser
{
  private:
    using numeric_t = typename Coercion::numeric_t;
    using string_t = typename Coercion::string_t;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser>;

  public:
    virtual ~storage_eraser(){};

    template <typename T> storage_eraser_ptr coerce_to() const { return coerce_to_(tag<T>{}); }

    template <typename T> bool can_coerce_to() const { return can_coerce_to_(tag<T>{}); }

    virtual std::type_index elements_type() const = 0;

  protected:
    template <typename T> auto coerce_to_(T) const
    {
        using Storage = storage<typename T::untag, Coercion>;
        auto_assert(can_coerce_to_(T{}));
        return storage_eraser_ptr{new Storage{reinterpret_cast<Storage&>(*this)}};
    }

    template <typename T> bool can_coerce_to_(T) const
    {
        using Storage = storage<typename T::untag, Coercion>;
        return dynamic_cast<const Storage*>(this) != nullptr;
    }

    template <typename T> const auto* cthis() const
    {
        auto_assert(can_coerce_to_(T{}));
        return reinterpret_cast<const storage<T, Coercion>*>(this);
    }

    template <typename T> auto* cthis()
    {
        auto_assert(can_coerce_to_(T{}));
        return reinterpret_cast<storage<T, Coercion>*>(this);
    }

    virtual storage_eraser_ptr coerce_to_(tag<numeric_t>) const = 0;
    virtual storage_eraser_ptr coerce_to_(tag<string_t>) const = 0;

    virtual bool can_coerce_to_(tag<numeric_t>) const = 0;
    virtual bool can_coerce_to_(tag<string_t>) const = 0;
};

template <typename T, typename Coercion, typename Enabler = void> class numeric_storage
{
  private:
    using numeric_t = typename Coercion::numeric_t;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_numeric(Iter, Iter) const { return nullptr; }
    constexpr bool can_coerce_to_numeric() const { return false; }
};

template <typename T, typename Coercion>
class numeric_storage<T, Coercion,
                      std::enable_if_t<std::is_same<decltype(Coercion::coerce_to_numeric(std::declval<T>())),
                                                    typename Coercion::numeric_t>::value>>
{
  private:
    using numeric_t = typename Coercion::numeric_t;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_numeric(Iter begin, Iter end) const
    {
        auto result = new storage<numeric_t, Coercion>;
        result->reserve(end - begin);
        std::transform(begin, end, std::back_inserter(*result),
                       [](const T& value) -> numeric_t { return Coercion::coerce_to_numeric(value); });
        return storage_eraser_ptr{result};
    }

    constexpr bool can_coerce_to_numeric() const { return true; }
};

template <typename Coercion> class numeric_storage<typename Coercion::numeric_t, Coercion, void>
{
  private:
    using numeric_t = typename Coercion::numeric_t;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_numeric(Iter begin, Iter end) const
    {
        auto result = new storage<numeric_t, Coercion>;
        result->assign(begin, end);
        return storage_eraser_ptr{result};
    }

    constexpr bool can_coerce_to_numeric() const { return true; }
};

template <typename T, typename Coercion, typename Enabler = void> class string_storage
{
  private:
    using string_t = typename Coercion::string_t;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_string(Iter, Iter) const { return nullptr; }
    constexpr bool can_coerce_to_string() const { return false; }
};

template <typename T, typename Coercion>
class string_storage<T, Coercion,
                     std::enable_if_t<std::is_same<decltype(Coercion::coerce_to_string(std::declval<T>())),
                                                   typename Coercion::string_t>::value>>
{
  private:
    using string_t = typename Coercion::string_t;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_string(Iter begin, Iter end) const
    {
        auto result = new storage<string_t, Coercion>;
        result->reserve(end - begin);
        std::transform(begin, end, std::back_inserter(*result),
                       [](const T& value) -> string_t { return Coercion::coerce_to_string(value); });
        return storage_eraser_ptr{result};
    }

    constexpr bool can_coerce_to_string() const { return true; }
};

template <typename Coercion> class string_storage<typename Coercion::string_t, Coercion, void>
{
  private:
    using string_t = typename Coercion::string_t;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

  protected:
    template <typename Iter> storage_eraser_ptr coerce_to_string(Iter begin, Iter end) const
    {
        auto result = new storage<string_t, Coercion>;
        result->assign(begin, end);
        return storage_eraser_ptr{result};
    }

    constexpr bool can_coerce_to_string() const { return true; }
};

template <typename T, typename Coercion>
class storage : public numeric_storage<T, Coercion>,
                public string_storage<T, Coercion>,
                public storage_eraser<Coercion>,
                public std::vector<T>
{
    static_assert(!std::is_reference<T>::value, "storage type can not be a reference");
    friend class storage_eraser<Coercion>;
    friend class numeric_storage<T, Coercion>;
    friend class string_storage<T, Coercion>;

  public:
    using std::vector<T>::vector;

    virtual std::type_index elements_type() const override final { return typeid(T); }

  private:
    using numeric_t = typename Coercion::numeric_t;
    using string_t = typename Coercion::string_t;
    using storage_eraser_ptr = std::unique_ptr<storage_eraser<Coercion>>;

    virtual storage_eraser_ptr coerce_to_(tag<numeric_t>) const override final
    {
        return this->coerce_to_numeric(this->begin(), this->end());
    }

    virtual storage_eraser_ptr coerce_to_(tag<string_t>) const override final
    {
        return this->coerce_to_string(this->begin(), this->end());
    }

    virtual bool can_coerce_to_(tag<numeric_t>) const override final { return this->can_coerce_to_numeric(); }

    virtual bool can_coerce_to_(tag<string_t>) const override final
    {
        return this->string_storage<T, Coercion>::can_coerce_to_string();
    }
};

} // namespace dataframe_detail
} // namespace jules

#endif // JULES_DATAFRAME_DETAIL_STORAGE_H
