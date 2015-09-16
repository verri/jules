#ifndef JULES_ARRAY_DETAIL_REF_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_REF_ARRAY_DECL_H

#include "array/detail/slice.hpp"

#include <array>
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
    using difference_type = std::ptrdiff_t;

    ref_ndarray() = delete;
    ~ref_ndarray() = default;

    ref_ndarray& operator=(const ref_ndarray& source);

    // TODO(optimization): template <typename U> ref_ndarray& operator=(const base_ndarray<U, N>& source);
    // TODO(optimization): template <typename U> ref_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> ref_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(const indirect_ndarray<U, N>& source);
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

    std::size_t size() const { return descriptor_.size(); }
    std::size_t size(std::size_t i) const { return descriptor_.extents(i); }

    std::size_t nrow() const { return size(0); }
    std::size_t ncol() const { return size(1); }

  protected:
    ref_ndarray(T* data, const base_slice<N>& descriptor) : data_{data}, descriptor_{descriptor} {}

    ref_ndarray(const ref_ndarray& source) = default;
    ref_ndarray(ref_ndarray&& source) = default;

    T* data_;
    base_slice<N> descriptor_;

  private:
    void check_assignment(const T* data, const base_slice<N> descriptor);
};

template <typename T> class ref_ndarray<T, 1>
{
    template <typename U, std::size_t M> friend class ref_ndarray;
    template <typename U, std::size_t M> friend class ref_ndarray_iterator;

  public:
    using value_type = T;
    static constexpr auto order = 1;

    using iterator = ref_ndarray_iterator<T, 1>;
    using const_iterator = ref_ndarray_iterator<const T, 1>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    ref_ndarray() = delete;
    ~ref_ndarray() = default;

    ref_ndarray& operator=(const ref_ndarray& source);

    // TODO(optimization): template <typename U> ref_ndarray& operator=(const base_ndarray<U, 1>& source);
    // TODO(optimization): template <typename U> ref_ndarray& operator=(base_ndarray<U, 1>&& source);

    template <typename U> ref_ndarray& operator=(const ref_ndarray<U, 1>& source);
    template <typename U> ref_ndarray& operator=(const indirect_ndarray<U, 1>& source);
    template <typename U> ref_ndarray& operator=(const U& source);

    operator ref_ndarray<const T, 1>() const { return {this->data_, descriptor_}; }

    T& operator[](std::size_t i) { return data_[descriptor_(i)]; }
    const T& operator[](std::size_t i) const { return data_[descriptor_(i)]; }

    template <typename... Args> indirect_request<indirect_ndarray<T, 1>, Args...> operator()(Args&&... args);
    template <typename... Args> slice_request<ref_ndarray<T, 1>, Args...> operator()(Args&&... args);
    template <typename... Args> element_request<T&, Args...> operator()(Args&&... args);

    template <typename... Args>
    indirect_request<indirect_ndarray<const T, 1>, Args...> operator()(Args&&... args) const;
    template <typename... Args>
    slice_request<ref_ndarray<const T, 1>, Args...> operator()(Args&&... args) const;
    template <typename... Args> element_request<const T&, Args...> operator()(Args&&... args) const;

    ref_ndarray_iterator<T, 1> begin() { return {*this, 0}; }
    ref_ndarray_iterator<T, 1> end() { return {*this, descriptor_.extents(0)}; }

    ref_ndarray_iterator<const T, 1> begin() const { return {*this, 0}; }
    ref_ndarray_iterator<const T, 1> end() const { return {*this, descriptor_.extents(0)}; }

    ref_ndarray_iterator<const T, 1> cbegin() const { return {*this, 0}; }
    ref_ndarray_iterator<const T, 1> cend() const { return {*this, descriptor_.extents(0)}; }

    std::size_t size() const { return descriptor_.size(); }

  protected:
    ref_ndarray(T* data, const base_slice<1>& descriptor) : data_{data}, descriptor_{descriptor} {}

    ref_ndarray(const ref_ndarray& source) = default;
    ref_ndarray(ref_ndarray&& source) = default;

    T* data_;
    base_slice<1> descriptor_;

  private:
    void check_assignment(const T* data, const base_slice<1> descriptor);
};

template <typename T, std::size_t N> class ref_ndarray_iterator
{
  public:
    using value_type = ref_ndarray<T, N - 1>;
    using difference_type = std::ptrdiff_t;
    using reference = ref_ndarray<T, N - 1>&;
    using pointer = ref_ndarray<T, N - 1>*;
    using iterator_category = std::random_access_iterator_tag;

    ref_ndarray_iterator(const ref_ndarray<T, N>& array, std::size_t index) : array_{array}, index_{index} {}

    ref_ndarray<T, N - 1> operator*() const { return array_[index_]; }

    ref_ndarray_iterator& operator++();
    ref_ndarray_iterator operator++(int);
    bool operator==(const ref_ndarray_iterator& other) const { return index_ == other.index_; }
    bool operator!=(const ref_ndarray_iterator& other) const { return index_ != other.index_; }

    // TODO other random access operations

    std::ptrdiff_t operator-(const ref_ndarray_iterator& other) const { return index_ - other.index_; }

  private:
    mutable ref_ndarray<T, N> array_;
    std::size_t index_;
};

template <typename T> class ref_ndarray_iterator<T, 1>
{
  public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using pointer = T*;
    using iterator_category = std::random_access_iterator_tag;

    ref_ndarray_iterator(const ref_ndarray<T, 1>& array, std::size_t index) : array_{array}, index_{index} {}

    T& operator*() const { return array_[index_]; }

    ref_ndarray_iterator& operator++();
    ref_ndarray_iterator operator++(int);
    bool operator==(const ref_ndarray_iterator& other) const { return index_ == other.index_; }
    bool operator!=(const ref_ndarray_iterator& other) const { return index_ != other.index_; }

    // TODO other random access operations

    std::ptrdiff_t operator-(const ref_ndarray_iterator& other) const { return index_ - other.index_; }

  private:
    mutable ref_ndarray<T, 1> array_;
    std::size_t index_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_REF_ARRAY_DECL_H
