#ifndef JULES_DATAFRAME_COLUMN_DECL_H
#define JULES_DATAFRAME_COLUMN_DECL_H

#include "core/type.hpp"
#include "array/view.hpp"
#include "dataframe/detail/storage.hpp"

#include <initializer_list>
#include <memory>
#include <typeindex>

namespace jules
{
template <typename Coercion> class base_dataframe;

template <typename Coercion> class base_column
{
    friend class base_dataframe<Coercion>;

  private:
    template <typename T> using storage_t = dataframe_detail::storage<T, Coercion>;
    using storage_eraser_t = dataframe_detail::storage_eraser<Coercion>;

  public:
    template <typename T> base_column(const std::string& name, std::initializer_list<T> values);
    template <typename T> base_column(const std::string& name, const T& value, std::size_t size = 0);
    template <typename T> base_column(std::initializer_list<T> values);
    template <typename T> base_column(const T& value, std::size_t size = 0);

    base_column(const base_column& source);
    base_column(base_column&& source) = default;

    base_column& operator=(const base_column& source);
    base_column& operator=(base_column&& source) = default;

    template <typename T> base_column& coerce_to();
    template <typename T, typename Coercion_>
    friend base_column<Coercion_> coerce_to(const base_column<Coercion_>& source);

    template <typename T> bool can_coerce_to() const { return storage_->template can_coerce_to<T>(); }
    template <typename T, typename Coercion_> friend bool can_coerce_to(const base_column<Coercion_>& column);

    std::type_index elements_type() const { return storage_->elements_type(); }

    template <typename T, typename Coercion_>
    friend array_view<dataframe_detail::storage<T, Coercion_>> make_view(base_column<Coercion_>& column);

    auto size() const { return storage_->size(); }

    auto& name() { return name_; }
    const auto& name() const { return name_; }

  private:
    base_column(const std::string& name, std::unique_ptr<storage_eraser_t>&& storage);

    std::string name_;
    std::unique_ptr<storage_eraser_t> storage_;
};

using column = base_column<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_DECL_H
