#ifndef JULES_DATAFRAME_COLUMN_VIEW_DECL_H
#define JULES_DATAFRAME_COLUMN_VIEW_DECL_H

#include "array/array.hpp"
#include "dataframe/detail/column.hpp"

namespace jules
{
template <typename Coercion> class base_column;

template <typename T, typename Coercion> class base_const_column_view
{
    friend class base_column<Coercion>;

  private:
    using column_model_t = detail::column_model<T, Coercion>;

  public:
    using value_type = T;
    using size_type = typename column_model_t::size_type;
    using difference_type = typename column_model_t::difference_type;

    using reference = typename column_model_t::reference;
    using const_reference = typename column_model_t::const_reference;
    using pointer = typename column_model_t::pointer;
    using const_pointer = typename column_model_t::const_pointer;
    using iterator = typename column_model_t::iterator;
    using const_iterator = typename column_model_t::const_iterator;

    base_const_column_view(const base_const_column_view& source) = default;
    base_const_column_view(base_const_column_view&& source) = default;

    base_const_column_view& operator=(const base_const_column_view& source) = default;
    base_const_column_view& operator=(base_const_column_view&& source) = default;

    const auto& operator[](std::size_t i) const { return column_model_[i]; }

    auto begin() const { return column_model_.begin(); }
    auto end() const { return column_model_.end(); }

    template <typename R, typename S, typename C>
    friend vector<bool> operator==(const base_const_column_view<R, C>& lhs, const base_const_column_view<S, C>& rhs);

    template <typename R, typename S, typename C>
    friend vector<bool> operator==(const R& lhs, const base_const_column_view<T, C>& rhs);

    template <typename R, typename S, typename C>
    friend vector<bool> operator==(const base_const_column_view<R, C>& lhs, const S& rhs);

    auto size() const { return column_model_.size(); }

  protected:
    base_const_column_view(const column_model_t& column_model);
    column_model_t& column_model_;
};

template <typename T, typename Coercion> class base_column_view : public base_const_column_view<T, Coercion>
{
    friend class base_column<Coercion>;

  private:
    using column_model_t = detail::column_model<T, Coercion>;

  public:
    base_column_view(const base_column_view& source) = default;
    base_column_view(base_column_view&& source) = default;

    base_column_view& operator=(const base_column_view& source) = default;
    base_column_view& operator=(base_column_view&& source) = default;

    using base_const_column_view<T, Coercion>::operator[];
    auto& operator[](std::size_t i) { return this->column_model_[i]; }

    auto begin() { return this->column_model_.begin(); }
    auto end() { return this->column_model_.end(); }

    using base_const_column_view<T, Coercion>::begin;
    using base_const_column_view<T, Coercion>::end;

  private:
    base_column_view(const column_model_t& column_model) : base_const_column_view<T, Coercion>(column_model)
    {
    }
};

template <typename T> using column_view = base_column_view<T, default_coercion_rules>;
template <typename T> using const_column_view = base_const_column_view<T, default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_VIEW_DECL_H
