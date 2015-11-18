#ifndef JULES_DATAFRAME_COLUMN_VIEW_DECL_H
#define JULES_DATAFRAME_COLUMN_VIEW_DECL_H

#include <jules/array/array.hpp>
#include <jules/dataframe/detail/column.hpp>

namespace jules
{
template <typename Coercion> class base_column;

template <typename T, typename Coercion> class base_column_view : public detail::ref_ndarray<T, 1>
{
    friend class base_column<Coercion>;
    template <typename, typename> friend class base_column_view;

  public:
    base_column_view() {}
    ~base_column_view() = default;

    base_column_view(const base_column_view& source) : detail::ref_ndarray<T, 1>{source} {}
    base_column_view(base_column_view&& source) : detail::ref_ndarray<T, 1>{std::move(source)} {}

    base_column_view& operator=(const base_column_view& source) = delete;
    base_column_view& operator=(base_column_view&& source) = delete;

    // TODO: idem no colview do dataframe
    operator base_column_view<const T, Coercion>() const;

    using detail::ref_ndarray<T, 1>::operator=;

    T* data() { return this->data_; }
    const T* data() const { return this->data_; }

    T* data_begin() { return this->data_; }
    T* data_end() { return this->data_ + this->size(); }

    const T* data_begin() const { return this->data_; }
    const T* data_end() const { return this->data_ + this->size(); }

  private:
    base_column_view(T* data, std::size_t size) : detail::ref_ndarray<T, 1>{data, {0, {size}}} {}
    base_column_view(const detail::ref_ndarray<T, 1>& source) : detail::ref_ndarray<T, 1>{source} {}
};

template <typename T> using column_view = base_column_view<T, default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_VIEW_DECL_H
