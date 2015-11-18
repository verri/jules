#ifndef JULES_DATAFRAME_DATAFRAME_COLVIEW_DECL_H
#define JULES_DATAFRAME_DATAFRAME_COLVIEW_DECL_H

#include <jules/dataframe/column.hpp>
#include <jules/dataframe/column_view.hpp>

namespace jules
{
template <typename Coercion> class base_dataframe;

// TODO: remove parameter V
template <typename T, typename Coercion, typename V = base_column_view<T, Coercion>>
class base_dataframe_colview : public detail::ref_ndarray<V, 1>
{
    friend class base_dataframe<Coercion>;

  public:
    base_dataframe_colview() = default;
    ~base_dataframe_colview() = default;

    base_dataframe_colview(const base_dataframe_colview& source) : detail::ref_ndarray<V, 1>{source} {}
    base_dataframe_colview(base_dataframe_colview&& source) : detail::ref_ndarray<V, 1>{std::move(source)} {}

    base_dataframe_colview& operator=(const base_dataframe_colview& source) = delete;
    base_dataframe_colview& operator=(base_dataframe_colview&& source) = delete;

    using detail::ref_ndarray<V, 1>::operator=;

    V* data() { return this->data_; }
    const V* data() const { return this->data_; }

    V* data_begin() { return this->data_; }
    V* data_end() { return this->data_ + this->size(); }

    const V* data_begin() const { return this->data_; }
    const V* data_end() const { return this->data_ + this->size(); }

  private:
    base_dataframe_colview(std::vector<V>&& views);
    std::vector<V> views_;
};

template <typename T> using dataframe_colview = base_dataframe_colview<T, default_coercion_rules>;

} // namespace jules

#endif // JULES_DATAFRAME_DATAFRAME_COLVIEW_DECL_H
