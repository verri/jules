#ifndef JULES_DATAFRAME_DATAFRAME_COLVIEW_DECL_H
#define JULES_DATAFRAME_DATAFRAME_COLVIEW_DECL_H

#include "dataframe/column.hpp"
#include "dataframe/column_view.hpp"

namespace jules
{
template <typename Coercion> class base_dataframe;

// TODO: remove parameter V
template <typename T, typename Coercion, typename V = base_column_view<T, Coercion>>
    class base_dataframe_colview :
    public detail::ref_ndarray<V, 1>
{
    friend class base_dataframe<Coercion>;

  public:
    // TODO criar um construtor protected do ref_ndarray equivalente
    base_dataframe_colview() : detail::ref_ndarray<V, 1>{nullptr, {0, {0}}} {}
    ~base_dataframe_colview() = default;

    base_dataframe_colview(const base_dataframe_colview& source) : base_dataframe_colview() { this->assign(source); }
    base_dataframe_colview(base_dataframe_colview&& source) : base_dataframe_colview()  { this->assign(std::move(source)); }

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
