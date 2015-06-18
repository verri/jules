#ifndef JULES_DATAFRAME_DETAIL_STORAGE_H
#define JULES_DATAFRAME_DETAIL_STORAGE_H

#include "core/type.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>

namespace jules
{
namespace dataframe_detail
{

template <typename C>
class storage_eraser
{
  private:
    using numeric_t = typename C::numeric_t;
    using string_t = typename C::string_t;

  public:
    virtual ~storage_eraser(){};

    virtual std::unique_ptr<storage_eraser> coerce_to_numeric() const = 0;
    virtual std::unique_ptr<storage_eraser> coerce_to_string() const = 0;

    virtual bool can_coerce_to_numeric() const = 0;
    virtual bool can_coerce_to_string() const = 0;
};

template <typename T, typename E = void> class storage : public storage_eraser, public vector_t<T>
{
    static_assert(!std::is_reference<T>::value, "storage type can not be a reference");

  public:
    using vector_t<T>::vector_t;

    std::unique_ptr<storage_eraser> coerce_to_numeric() const final
    {
        // TODO assert
        return std::unique_ptr<storage_eraser>{nullptr};
    }

    std::unique_ptr<storage_eraser> coerce_to_string() const final
    {
        // TODO assert
        return std::unique_ptr<storage_eraser>{nullptr};
    }

    bool can_coerce_to_numeric() const final { return false; }
    bool can_coerce_to_string() const final { return false; }
};

template <typename T>
class storage<T, std::enable_if_t<std::is_convertible<T, numeric_t>::value>> : public storage_eraser,
                                                                               public vector_t<T>
{
    static_assert(!std::is_reference<T>::value, "storage type can not be a reference");

  public:
    using vector_t<T>::vector_t;

    std::unique_ptr<storage_eraser> coerce_to_numeric() const final
    {
        auto result = new storage<numeric_t>;
        result->reserve(this->size());
        std::transform(this->begin(), this->end(), std::back_inserter(*result),
                       [](auto&& value) -> numeric_t { return value; });
        return std::unique_ptr<storage_eraser>{result};
    }

    std::unique_ptr<storage_eraser> coerce_to_string() const final
    {
        auto result = new storage<string_t>;
        result->reserve(this->size());
        std::transform(this->begin(), this->end(), std::back_inserter(*result),
                       [](auto&& value) -> string_t { return jules::coerce_to_string(value); });
        return std::unique_ptr<storage_eraser>{result};
    }

    bool can_coerce_to_numeric() const final { return true; }
    bool can_coerce_to_string() const final { return true; }
};

template <typename T>
class storage<
    T, std::enable_if_t<!std::is_convertible<T, numeric_t>::value && std::is_convertible<T, string_t>::value>>
    : public storage_eraser, public vector_t<T>
{
    static_assert(!std::is_reference<T>::value, "storage type can not be a reference");

  public:
    using vector_t<T>::vector_t;

    std::unique_ptr<storage_eraser> coerce_to_numeric() const final
    {
        // TODO assert
        return std::unique_ptr<storage_eraser>{nullptr};
    }

    std::unique_ptr<storage_eraser> coerce_to_string() const final
    {
        auto result = new storage<string_t>;
        result->reserve(this->size());
        std::transform(this->begin(), this->end(), std::back_inserter(*result),
                       [](auto&& value) -> string_t { return value; });
        return std::unique_ptr<storage_eraser>{result};
    }

    bool can_coerce_to_numeric() const final { return false; }
    bool can_coerce_to_string() const final { return true; }
};

} // namespace dataframe_detail
} // namespace jules

#endif // JULES_DATAFRAME_DETAIL_STORAGE_H
