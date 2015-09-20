#ifndef JULES_DATAFRAME_COLUMN_DECL_H
#define JULES_DATAFRAME_COLUMN_DECL_H

#include "core/type.hpp"
#include "dataframe/column_view.hpp"
#include "dataframe/detail/column.hpp"
#include "range/range.hpp"

#include <initializer_list>
#include <memory>
#include <typeindex>

namespace jules
{
template <typename Coercion> class base_dataframe;
template <typename Coercion> class base_column;

template <typename Coercion> class base_column
{
    friend class base_dataframe<Coercion>;

  private:
    template <typename T> using column_model_t = detail::column_model<T, Coercion>;
    using column_interface_t = detail::column_interface<Coercion>;

    template <typename T> using view_t = base_column_view<T, Coercion>;

  public:
    template <typename T> base_column(const std::string& name, std::initializer_list<T> values);
    template <typename T> base_column(const std::string& name, const T& value, std::size_t size);
    template <typename T> base_column(std::initializer_list<T> values);
    template <typename T> base_column(const T& value, std::size_t size);

    template <typename Range, typename R = typename std::remove_reference<Range>::type::value_type>
    base_column(const std::string& name, Range&& rng);

    template <typename Range, typename R = typename std::remove_reference<Range>::type::value_type>
    base_column(Range&& rng);

    base_column(const base_column& source);
    base_column(base_column&& source) = default;

    base_column& operator=(const base_column& source);
    base_column& operator=(base_column&& source) = default;

    template <typename T> base_column& coerce_to();
    template <typename T, typename C> friend base_column<C> coerce_to(const base_column<C>& source);

    template <typename T> bool can_coerce_to() const { return column_model_->template can_coerce_to<T>(); }
    template <typename T, typename C> friend bool can_coerce_to(const base_column<C>& column);

    std::type_index elements_type() const { return column_model_->elements_type(); }

    template <typename T> view_t<T> view();
    template <typename T> view_t<const T> view() const;

    template <typename T, typename C> friend base_column_view<T, C> make_view(base_column<C>& column);
    template <typename T, typename C>
    friend base_column_view<const T, C> make_view(const base_column<C>& column);

    auto size() const { return column_model_->size(); }

    std::string& name() { return name_; }
    const std::string& name() const { return name_; }

  private:
    base_column(const std::string& name, std::unique_ptr<column_interface_t>&& column_model);

    std::string name_;
    std::unique_ptr<column_interface_t> column_model_;
};

using column = base_column<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_DECL_H
