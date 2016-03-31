#ifndef JULES_DATAFRAME_COLUMN_DECL_H
#define JULES_DATAFRAME_COLUMN_DECL_H

#include <jules/core/type.hpp>
#include <jules/dataframe/column_view.hpp>
#include <jules/dataframe/detail/column.hpp>
#include <jules/range/range.hpp>

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
    template <typename> friend class base_column_view;

  private:
    template <typename T> using column_model_t = detail::column_model<T, Coercion>;
    using column_interface_t = detail::column_interface<Coercion>;

  public:
    template <typename T> using view_t = base_column_view<T>;

    template <typename T> base_column(const std::string& name, std::initializer_list<T> values);
    template <typename T> base_column(const std::string& name, const T& value, std::size_t size);
    template <typename T> base_column(std::initializer_list<T> values);
    template <typename T> base_column(const T& value, std::size_t size);
    template <typename Range, typename R = range_value_t<Range>> base_column(const std::string& name, const Range& rng);
    template <typename Range, typename R = range_value_t<Range>> base_column(const Range& rng);

    base_column(const base_column& source);
    base_column(base_column&& source) = default;

    auto operator=(const base_column& source) -> base_column&;
    auto operator=(base_column&& source) -> base_column& = default;

    template <typename T> auto coerce_to() & -> base_column&;
    template <typename T> auto can_coerce_to() const { return column_model_->template can_coerce_to<T>(); }

    auto elements_type() const { return column_model_->elements_type(); }

    auto size() const { return column_model_->size(); }
    auto is_empty() const { return size() == 0; }

    auto name() -> std::string & { return name_; }
    auto name() const -> const std::string & { return name_; }

    template <typename T, typename C> friend auto as_view(base_column<C>& column) -> base_column_view<T>;
    template <typename T, typename C> friend auto as_view(const base_column<C>& column) -> base_column_view<T>;
    template <typename T, typename C> friend auto as_column(const base_column<C>& column) -> base_column<C>;

  private:
    base_column(const std::string& name, std::unique_ptr<column_interface_t>&& column_model);

    template <typename T> auto view() -> view_t<T>;
    template <typename T> auto view() const -> view_t<const T>;

    template <typename T> auto clone() const -> base_column;

    std::string name_;
    std::unique_ptr<column_interface_t> column_model_;
};

using column = base_column<default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_DECL_H
