#ifndef JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H

#include "array/detail/common.hpp"

#include "array/detail/define_macros.hpp"

namespace jules
{
namespace detail
{
FRIEND_OPERATIONS_DECLARATION((typename R, std::size_t M), (const indirect_ndarray<R, M>&))

template <typename T, std::size_t N> class indirect_ndarray
{
    template <typename, std::size_t> friend class indirect_ndarray;
    template <typename, std::size_t> friend class indirect_ndarray_iterator;
    template <typename, std::size_t> friend class ref_ndarray;
    template <typename, typename, typename, std::size_t> friend class binary_expr_ndarray;
    template <typename, typename, std::size_t> friend class unary_expr_ndarray;

  public:
    using value_type = T;
    static constexpr auto order = N;

    using iterator = indirect_ndarray_iterator<T, N>;
    using const_iterator = indirect_ndarray_iterator<const T, N>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    ~indirect_ndarray() = default;

    indirect_ndarray& operator=(const indirect_ndarray& source);

    template <typename U> indirect_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> indirect_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> indirect_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> indirect_ndarray& operator=(const indirect_ndarray<U, N>& source);

    template <typename LhsIt, typename RhsIt, typename F>
    indirect_ndarray& operator=(const binary_expr_ndarray<LhsIt, RhsIt, F, N>& source);
    template <typename It, typename F>
    indirect_ndarray& operator=(const unary_expr_ndarray<It, F, N>& source);

    template <typename U> indirect_ndarray& operator=(const U& source);

    operator indirect_ndarray<const T, N>() const { return {data_, descriptor_, indexes_}; }

    indirect_ndarray<T, N - 1> operator[](std::size_t i);
    indirect_ndarray<const T, N - 1> operator[](std::size_t i) const;

    template <typename... Args> indirect_request<indirect_ndarray<T, N>, Args...> operator()(Args&&... args);
    template <typename... Args> slice_request<indirect_ndarray<T, N>, Args...> operator()(Args&&... args);
    template <typename... Args> element_request<T&, Args...> operator()(Args&&... args);

    template <typename... Args>
    indirect_request<indirect_ndarray<const T, N>, Args...> operator()(Args&&... args) const;
    template <typename... Args>
    slice_request<indirect_ndarray<const T, N>, Args...> operator()(Args&&... args) const;
    template <typename... Args> element_request<const T&, Args...> operator()(Args&&... args) const;

    indirect_ndarray_iterator<T, N> begin() { return {*this, 0}; }
    indirect_ndarray_iterator<T, N> end() { return {*this, this->size(0)}; }

    indirect_ndarray_iterator<const T, N> begin() const { return {*this, 0}; }
    indirect_ndarray_iterator<const T, N> end() const { return {*this, this->size(0)}; }

    indirect_ndarray_iterator<const T, N> cbegin() const { return {*this, 0}; }
    indirect_ndarray_iterator<const T, N> cend() const { return {*this, this->size(0)}; }

    std::size_t size() const { return descriptor_.size(); }
    std::size_t size(std::size_t i) const { return descriptor_.extents(i); }

    const auto& extents() const { return descriptor_.extents(); }

    std::size_t nrow() const { return size(0); }
    std::size_t ncol() const { return size(1); }

    indirect_ndarray_data_iterator<T, N> data_begin() { return {data_, indexes_, 0}; }
    indirect_ndarray_data_iterator<T, N> data_end() { return {data_, indexes_, indexes_.size()}; }

    indirect_ndarray_data_iterator<const T, N> data_begin() const { return {data_, indexes_, 0}; }
    indirect_ndarray_data_iterator<const T, N> data_end() const { return {data_, indexes_, indexes_.size()}; }

    OPERATIONS_LIST((typename R, std::size_t M), (const indirect_ndarray<R, M>&), N)

  private:
    indirect_ndarray() = default;
    indirect_ndarray(T* data, const std::array<std::size_t, N>& extents, std::vector<std::size_t>&& indexes);
    indirect_ndarray(T* data, const std::array<std::size_t, N>& extents,
                     const std::vector<std::size_t>& indexes);

    indirect_ndarray(const indirect_ndarray& source) = default;
    indirect_ndarray(indirect_ndarray&& source) = default;

    void clone_from(const indirect_ndarray& source);
    void clone_from(indirect_ndarray&& source);

    T* data_;
    base_slice<N> descriptor_;
    std::vector<std::size_t> indexes_;
};

template <typename T, std::size_t N> class indirect_ndarray_iterator
{
  public:
    using value_type = indirect_ndarray<T, N - 1>;
    using difference_type = std::ptrdiff_t;
    using indirecterence = indirect_ndarray<T, N - 1>&;
    using pointer = indirect_ndarray<T, N - 1>*;
    using iterator_category = std::random_access_iterator_tag;

    indirect_ndarray_iterator() = default;
    indirect_ndarray_iterator(const indirect_ndarray<T, N>& array, std::size_t index)
        : array_{array}, index_{index}
    {
    }

    indirect_ndarray<T, N - 1> operator*() const { return array_[index_]; }

    indirect_ndarray_iterator& operator++();
    indirect_ndarray_iterator operator++(int);
    bool operator==(const indirect_ndarray_iterator& other) const { return index_ == other.index_; }
    bool operator!=(const indirect_ndarray_iterator& other) const { return index_ != other.index_; }

    // TODO other random access operations

    std::ptrdiff_t operator-(const indirect_ndarray_iterator& other) const { return index_ - other.index_; }

  private:
    mutable indirect_ndarray<T, N> array_;
    std::size_t index_;
};

// TODO: iterators and specializations like ref_ndarray

template <typename T, std::size_t N> class indirect_ndarray_data_iterator
{
  public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using indirecterence = T&;
    using pointer = T*;
    using iterator_category = std::forward_iterator_tag;

    indirect_ndarray_data_iterator() = default;
    indirect_ndarray_data_iterator(T* data, const std::vector<std::size_t>& indexes, std::size_t i)
        : data_{data}, indexes_{indexes}, current_{i}
    {
    }

    T& operator*() const { return data_[indexes_[current_]]; }

    indirect_ndarray_data_iterator& operator++();
    indirect_ndarray_data_iterator operator++(int);
    bool operator==(const indirect_ndarray_data_iterator& other) const { return current_ == other.current_; }
    bool operator!=(const indirect_ndarray_data_iterator& other) const { return current_ != other.current_; }

    std::ptrdiff_t operator-(const indirect_ndarray_data_iterator& other) const
    {
        return current_ - other.current_;
    }

    operator indirect_ndarray_data_iterator<const T, N>() const { return {data_, indexes_, current_}; }

  private:
    T* data_ = nullptr;
    const std::vector<std::size_t>& indexes_;
    std::size_t current_;
};

} // namespace detail
} // namespace jules

#include "array/detail/undef_macros.hpp"

#endif // JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H
