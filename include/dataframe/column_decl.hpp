#ifndef JULES_DATAFRAME_COLUMN_DECL_H
#define JULES_DATAFRAME_COLUMN_DECL_H

#include "core/type.hpp"
#include "array/view.hpp"
#include "dataframe/detail/column.hpp"

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
    template <typename T> using column_model_t = detail::column_model<T, Coercion>;
    using column_concept_t = detail::column_concept<Coercion>;

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

    template <typename T> bool can_coerce_to() const { return column_model_->template can_coerce_to<T>(); }
    template <typename T, typename Coercion_> friend bool can_coerce_to(const base_column<Coercion_>& column);

    std::type_index elements_type() const { return column_model_->elements_type(); }

    template <typename T, typename Coercion_>
    friend array_view<detail::column_model<T, Coercion_>> make_view(base_column<Coercion_>& column);

    auto size() const { return column_model_->size(); }

    auto& name() { return name_; }
    const auto& name() const { return name_; }

  private:
    base_column(const std::string& name, std::unique_ptr<column_concept_t>&& column_model);

    std::string name_;
    std::unique_ptr<column_concept_t> column_model_;
};

using column = base_column<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_DECL_H
