#ifndef JULES_DATAFRAME_COLUMN_VIEW_DECL_H
#define JULES_DATAFRAME_COLUMN_VIEW_DECL_H

#include <jules/array/array.hpp>

namespace jules
{
template <typename> class base_column;
template <typename> class base_dataframe;

template <typename T> class base_column_view : public detail::ref_ndarray<T, 1>
{
  template <typename> friend class base_column;
  template <typename> friend class base_column_view;
  template <typename> friend class base_dataframe;

private:
  using super_t = detail::ref_ndarray<T, 1>;

public:
  base_column_view() = delete;
  ~base_column_view() = default;

  base_column_view(const base_column_view& source) = default;
  base_column_view(base_column_view&& source) = default;

  base_column_view& operator=(const base_column_view& source) = delete;
  base_column_view& operator=(base_column_view&& source) = delete;

  using detail::ref_ndarray<T, 1>::operator=;

  operator base_column_view<const T>() const;

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

template <typename T> using column_view = base_column_view<T>;

} // namespace jules

#endif // JULES_DATAFRAME_COLUMN_VIEW_DECL_H
