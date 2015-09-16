#ifndef JULES_ARRAY_DETAIL_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_ARRAY_DECL_H

#include "array/detail/slice.hpp"

#include <array>
#include <utility>
#include <vector>

namespace jules
{
namespace detail
{
template <typename T, std::size_t N> class base_ndarray;
template <typename T, std::size_t N> class ref_ndarray;
template <typename T, std::size_t N> class ref_ndarray_iterator;

template <typename T, std::size_t N> class indirect_ndarray
{
    template <typename U, std::size_t M> friend class base_ndarray;
    template <typename U, std::size_t M> friend class ref_ndarray;

  public:
  private:
    base_ndarray<T, N>* array_;
    std::vector<std::size_t> indexes[N];
};

template <typename T, std::size_t N> class ref_ndarray
{
    template <typename U, std::size_t M> friend class ref_ndarray;
    template <typename U, std::size_t M> friend class ref_ndarray_iterator;

  public:
    using value_type = T;
    static constexpr auto order = N;

    using iterator = ref_ndarray_iterator<T, N>;
    using const_iterator = ref_ndarray_iterator<const T, N>;

    using size_type = std::size_t;
    using difference_type = std::size_t;

    ref_ndarray() = delete;
    ~ref_ndarray() = default;

    template <typename U> ref_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> ref_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(ref_ndarray<U, N>&& source);

    template <typename U> ref_ndarray& operator=(const indirect_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(indirect_ndarray<U, N>&& source);

    template <typename U> ref_ndarray& operator=(const U& source);

    operator ref_ndarray<const T, N>() const { return {this->data_, descriptor_}; }

    ref_ndarray<T, N - 1> operator[](std::size_t i);
    ref_ndarray<const T, N - 1> operator[](std::size_t i) const;

    template <typename... Args> indirect_request<indirect_ndarray<T, N>, Args...> operator()(Args&&... args);
    template <typename... Args> slice_request<ref_ndarray<T, N>, Args...> operator()(Args&&... args);
    template <typename... Args> element_request<T&, Args...> operator()(Args&&... args);

    template <typename... Args>
    indirect_request<indirect_ndarray<const T, N>, Args...> operator()(Args&&... args) const;
    template <typename... Args>
    slice_request<ref_ndarray<const T, N>, Args...> operator()(Args&&... args) const;
    template <typename... Args> element_request<const T&, Args...> operator()(Args&&... args) const;

    ref_ndarray_iterator<T, N> begin() { return {*this, 0}; }
    ref_ndarray_iterator<T, N> end() { return {*this, descriptor_.extents(0)}; }

    ref_ndarray_iterator<const T, N> begin() const { return {*this, 0}; }
    ref_ndarray_iterator<const T, N> end() const { return {*this, descriptor_.extents(0)}; }

    ref_ndarray_iterator<const T, N> cbegin() const { return {*this, 0}; }
    ref_ndarray_iterator<const T, N> cend() const { return {*this, descriptor_.extents(0)}; }

  protected:
    ref_ndarray(T* data, const base_slice<N>& descriptor) : data_{data}, descriptor_{descriptor} {}

    ref_ndarray(const ref_ndarray& source) = default;
    ref_ndarray(ref_ndarray&& source) = default;

    T* data_;
    base_slice<N> descriptor_;
};

template <typename T, std::size_t N> class ref_ndarray_iterator
{
  public:
    ref_ndarray_iterator(const ref_ndarray<T, N>& array, std::size_t index) : array_{array}, index_{index} {}

    ref_ndarray<T, N - 1> operator*() { return array_[index_]; }

    ref_ndarray_iterator& operator++();
    ref_ndarray_iterator operator++(int);
    bool operator==(const ref_ndarray_iterator& other) const { return index_ == other.index_; }
    bool operator!=(const ref_ndarray_iterator& other) const { return index_ != other.index_; }

    std::size_t operator-(const ref_ndarray_iterator& other) const { return index_ - other.index_; }

  private:
    ref_ndarray<T, N> array_;
    std::size_t index_;
};

template <typename T, std::size_t N> class base_ndarray : public ref_ndarray<T, N>
{
  private:
    using storage_t = std::aligned_storage_t<sizeof(T), alignof(T)>;

  public:
    base_ndarray();
    ~base_ndarray();

    template <typename... Dims, typename = all_size_enabler<N, Dims...>> explicit base_ndarray(Dims... dims);

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    base_ndarray(const T& value, Dims... dims);

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    base_ndarray(const T* data, Dims... dims);

    base_ndarray(const base_ndarray& source) = default;
    base_ndarray(base_ndarray&& source) = default;

    template <typename U> base_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> base_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(ref_ndarray<U, N>&& source);

    template <typename U> base_ndarray& operator=(const indirect_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(indirect_ndarray<U, N>&& source);

    template <typename U> base_ndarray& operator=(const U& source);
    template <typename U> base_ndarray& operator=(U&& source);

    std::size_t size() const { return this->descriptor_.size(); }
    std::size_t size(std::size_t i) const { return this->descriptor_.extents(i); }

    T* data() { return this->data_; }
    const T* data() const { return this->data_; }
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_DECL_H
