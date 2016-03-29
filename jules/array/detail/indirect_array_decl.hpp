#ifndef JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H

#include <jules/array/detail/common.hpp>
#include <jules/array/detail/slice.hpp>

#include <jules/array/detail/define_macros.hpp>

namespace jules
{
namespace detail
{
FRIEND_OPERATIONS_DECLARATION((typename R, std::size_t M), (const indirect_ndarray<R, M>&))

template <typename T, std::size_t N> class indirect_ndarray
{
    static_assert(N > 1, "Invalid array dimension.");

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
    template <typename It, typename F> indirect_ndarray& operator=(const unary_expr_ndarray<It, F, N>& source);

    template <typename U> indirect_ndarray& operator=(const U& source);

    operator indirect_ndarray<const T, N>() const { return {data_, descriptor_, indexes_}; }

    indirect_ndarray<T, N - 1> operator[](std::size_t i);
    indirect_ndarray<const T, N - 1> operator[](std::size_t i) const;

    template <typename... Args> indirect_request<indirect_ndarray<T, N>, Args...> operator()(Args&&... args);
    template <typename... Args> slice_request<indirect_ndarray<T, N>, Args...> operator()(Args&&... args);
    template <typename... Args> element_request<T&, Args...> operator()(Args&&... args);

    template <typename... Args> indirect_request<indirect_ndarray<const T, N>, Args...> operator()(Args&&... args) const;
    template <typename... Args> slice_request<indirect_ndarray<const T, N>, Args...> operator()(Args&&... args) const;
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

    indirect_ndarray_data_iterator<T> data_begin() { return {data_, indexes_, 0}; }
    indirect_ndarray_data_iterator<T> data_end() { return {data_, indexes_, indexes_.size()}; }

    indirect_ndarray_data_iterator<const T> data_begin() const { return {data_, indexes_, 0}; }
    indirect_ndarray_data_iterator<const T> data_end() const { return {data_, indexes_, indexes_.size()}; }

    OPERATIONS_LIST((typename R, std::size_t M), (const indirect_ndarray<R, M>&), N)

  private:
    indirect_ndarray() = default;

    // guarantees default strides and start=0.
    indirect_ndarray(T* data, const std::array<std::size_t, N>& extents, std::vector<std::size_t>&& indexes);
    indirect_ndarray(T* data, const std::array<std::size_t, N>& extents, const std::vector<std::size_t>& indexes);

    indirect_ndarray(const indirect_ndarray& source) = default;
    indirect_ndarray(indirect_ndarray&& source) = default;

    void clone_from(const indirect_ndarray& source);
    void clone_from(indirect_ndarray&& source);

    T* data_;
    base_slice<N> descriptor_;
    std::vector<std::size_t> indexes_;
};

template <typename T, std::size_t N>
class indirect_ndarray_iterator : public std::iterator<std::random_access_iterator_tag, indirect_ndarray<T, N - 1>,
                                                       std::ptrdiff_t, indirect_ndarray<T, N - 1>>
{
  public:
    indirect_ndarray_iterator() = default;
    indirect_ndarray_iterator(const indirect_ndarray<T, N>& array, std::size_t index);

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

template <typename T> class indirect_ndarray<T, 1>
{
    template <typename, std::size_t> friend class indirect_ndarray;
    template <typename, std::size_t> friend class indirect_ndarray_iterator;
    template <typename, std::size_t> friend class ref_ndarray;
    template <typename, typename, typename, std::size_t> friend class binary_expr_ndarray;
    template <typename, typename, std::size_t> friend class unary_expr_ndarray;

  public:
    using value_type = T;
    static constexpr auto order = 1;

    using iterator = indirect_ndarray_iterator<T, 1>;
    using const_iterator = indirect_ndarray_iterator<const T, 1>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    ~indirect_ndarray() = default;

    indirect_ndarray& operator=(const indirect_ndarray& source);

    template <typename U> indirect_ndarray& operator=(const base_ndarray<U, 1>& source);
    template <typename U> indirect_ndarray& operator=(base_ndarray<U, 1>&& source);

    template <typename U> indirect_ndarray& operator=(const ref_ndarray<U, 1>& source);
    template <typename U> indirect_ndarray& operator=(const indirect_ndarray<U, 1>& source);

    template <typename LhsIt, typename RhsIt, typename F>
    indirect_ndarray& operator=(const binary_expr_ndarray<LhsIt, RhsIt, F, 1>& source);
    template <typename It, typename F> indirect_ndarray& operator=(const unary_expr_ndarray<It, F, 1>& source);

    template <typename U> indirect_ndarray& operator=(const U& source);

    operator indirect_ndarray<const T, 1>() const { return {data_, descriptor_, indexes_}; }

    T& operator[](std::size_t i) { return data_[indexes_[i]]; }
    const T& operator[](std::size_t i) const { return data_[indexes_[i]]; }

    template <typename Range> indirect_ndarray<T, 1> operator()(const Range& rng);
    indirect_ndarray<T, 1> operator()(const base_slice<1>& slice);
    T& operator()(std::size_t i);

    template <typename Range> indirect_ndarray<const T, 1> operator()(const Range& rng) const;
    indirect_ndarray<const T, 1> operator()(const base_slice<1>& slice) const;
    const T& operator()(std::size_t i) const;

    indirect_ndarray_iterator<T, 1> begin() { return {*this, 0}; }
    indirect_ndarray_iterator<T, 1> end() { return {*this, this->size(0)}; }

    indirect_ndarray_iterator<const T, 1> begin() const { return {*this, 0}; }
    indirect_ndarray_iterator<const T, 1> end() const { return {*this, this->size(0)}; }

    indirect_ndarray_iterator<const T, 1> cbegin() const { return {*this, 0}; }
    indirect_ndarray_iterator<const T, 1> cend() const { return {*this, this->size(0)}; }

    std::size_t size() const { return descriptor_.size(); }
    const auto& extents() const { return descriptor_.extents(); }

    indirect_ndarray_data_iterator<T> data_begin() { return {data_, indexes_, 0}; }
    indirect_ndarray_data_iterator<T> data_end() { return {data_, indexes_, indexes_.size()}; }

    indirect_ndarray_data_iterator<const T> data_begin() const { return {data_, indexes_, 0}; }
    indirect_ndarray_data_iterator<const T> data_end() const { return {data_, indexes_, indexes_.size()}; }

    OPERATIONS_LIST((typename R), (const indirect_ndarray<R, 1>&), 1)

  private:
    indirect_ndarray() = default;

    // guarantees default strides and start=0.
    indirect_ndarray(T* data, const std::array<std::size_t, 1>& extents, std::vector<std::size_t>&& indexes);
    indirect_ndarray(T* data, const std::array<std::size_t, 1>& extents, const std::vector<std::size_t>& indexes);

    indirect_ndarray(const indirect_ndarray& source) = default;
    indirect_ndarray(indirect_ndarray&& source) = default;

    void clone_from(const indirect_ndarray& source);
    void clone_from(indirect_ndarray&& source);

    T* data_;
    base_slice<1> descriptor_;
    std::vector<std::size_t> indexes_;
};

template <typename T> class indirect_ndarray_iterator<T, 1> : public std::iterator<std::random_access_iterator_tag, T>
{
  public:
    indirect_ndarray_iterator() = default;
    indirect_ndarray_iterator(const indirect_ndarray<T, 1>& array, std::size_t index);

    T& operator*() const { return array_[index_]; }

    indirect_ndarray_iterator& operator++();
    indirect_ndarray_iterator operator++(int);
    bool operator==(const indirect_ndarray_iterator& other) const { return index_ == other.index_; }
    bool operator!=(const indirect_ndarray_iterator& other) const { return index_ != other.index_; }

    // TODO other random access operations

    std::ptrdiff_t operator-(const indirect_ndarray_iterator& other) const { return index_ - other.index_; }

  private:
    mutable indirect_ndarray<T, 1> array_;
    std::size_t index_;
};

template <typename T> class indirect_ndarray_data_iterator : public std::iterator<std::forward_iterator_tag, T>
{
  public:
    indirect_ndarray_data_iterator() = default;
    indirect_ndarray_data_iterator(T* data, const std::vector<std::size_t>& indexes, std::size_t i);

    T& operator*() const { return data_[indexes_[current_]]; }

    indirect_ndarray_data_iterator& operator++();
    indirect_ndarray_data_iterator operator++(int);
    bool operator==(const indirect_ndarray_data_iterator& other) const { return current_ == other.current_; }
    bool operator!=(const indirect_ndarray_data_iterator& other) const { return current_ != other.current_; }

    std::ptrdiff_t operator-(const indirect_ndarray_data_iterator& other) const;

    operator indirect_ndarray_data_iterator<const T>() const { return {data_, indexes_, current_}; }

  private:
    T* data_ = nullptr;
    const std::vector<std::size_t>& indexes_;
    std::size_t current_;
};

} // namespace detail
} // namespace jules

#include <jules/array/detail/undef_macros.hpp>

#endif // JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H
