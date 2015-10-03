#ifndef JULES_ARRAY_DETAIL_REF_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_REF_ARRAY_DECL_H

#include "array/detail/common.hpp"
#include "array/detail/slice.hpp"

#include "array/detail/define_macros.hpp"

#include <array>
#include <iterator>
#include <vector>

namespace jules
{
namespace detail
{
FRIEND_OPERATIONS_DECLARATION((typename R, std::size_t M), (const ref_ndarray<R, M>&))

template <typename T, std::size_t N> class ref_ndarray
{
    static_assert(N > 1, "Invalid array dimension.");

    template <typename, std::size_t> friend class ref_ndarray;
    template <typename, std::size_t> friend class ref_ndarray_iterator;
    template <typename, std::size_t> friend class indirect_ndarray;
    template <typename, typename, typename, std::size_t> friend class binary_expr_ndarray;
    template <typename, typename, std::size_t> friend class unary_expr_ndarray;

    template <typename, typename> friend class jules::base_column_view;
    template <typename, typename, typename> friend class jules::base_dataframe_colview;

  public:
    using value_type = T;
    static constexpr auto order = N;

    using iterator = ref_ndarray_iterator<T, N>;
    using const_iterator = ref_ndarray_iterator<const T, N>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    ~ref_ndarray() = default;

    ref_ndarray& operator=(const ref_ndarray& source);

    template <typename U> ref_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> ref_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(const indirect_ndarray<U, N>& source);

    template <typename It, typename F> ref_ndarray& operator=(const unary_expr_ndarray<It, F, N>& source);
    template <typename LhsIt, typename RhsIt, typename F>
    ref_ndarray& operator=(const binary_expr_ndarray<LhsIt, RhsIt, F, N>& source);

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
    ref_ndarray_iterator<T, N> end() { return {*this, this->size(0)}; }

    ref_ndarray_iterator<const T, N> begin() const { return {*this, 0}; }
    ref_ndarray_iterator<const T, N> end() const { return {*this, this->size(0)}; }

    ref_ndarray_iterator<const T, N> cbegin() const { return {*this, 0}; }
    ref_ndarray_iterator<const T, N> cend() const { return {*this, this->size(0)}; }

    std::size_t size() const { return descriptor_.size(); }
    std::size_t size(std::size_t i) const { return descriptor_.extent(i); }

    const auto& extents() const { return descriptor_.extents(); }

    std::size_t nrow() const { return size(0); }
    std::size_t ncol() const { return size(1); }

    ref_ndarray_data_iterator<T, N> data_begin() { return {data_, descriptor_.begin()}; }
    ref_ndarray_data_iterator<T, N> data_end() { return {data_, descriptor_.end()}; }

    ref_ndarray_data_iterator<const T, N> data_begin() const { return {data_, descriptor_.begin()}; }
    ref_ndarray_data_iterator<const T, N> data_end() const { return {data_, descriptor_.end()}; }

    OPERATIONS_LIST((typename R, std::size_t M), (const ref_ndarray<R, M>&), N)

  protected:
    ref_ndarray(T* data, const base_slice<N>& descriptor) : data_{data}, descriptor_{descriptor} {}
    ref_ndarray() : data_{nullptr} {}

    ref_ndarray(const ref_ndarray& source) = default;
    ref_ndarray(ref_ndarray&& source) = default;

    void clone_from(const ref_ndarray& source);
    void clone_from(ref_ndarray&& source);
    [[deprecated("use clone_from")]] void assign(const ref_ndarray& source);

    T* data_;
    base_slice<N> descriptor_;
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

    ~ref_ndarray() = default;

    ref_ndarray& operator=(const ref_ndarray& source);

    template <typename U> ref_ndarray& operator=(const base_ndarray<U, 1>& source);
    template <typename U> ref_ndarray& operator=(base_ndarray<U, 1>&& source);

    template <typename U> ref_ndarray& operator=(const ref_ndarray<U, 1>& source);
    template <typename U> ref_ndarray& operator=(const indirect_ndarray<U, 1>& source);

    template <typename LhsIt, typename RhsIt, typename F>
    ref_ndarray& operator=(const binary_expr_ndarray<LhsIt, RhsIt, F, 1>& source);
    template <typename It, typename F> ref_ndarray& operator=(const unary_expr_ndarray<It, F, 1>& source);

    template <typename U> ref_ndarray& operator=(const U& source);

    operator ref_ndarray<const T, 1>() const { return {this->data_, descriptor_}; }

    T& operator[](std::size_t i) { return data_[descriptor_(i)]; }
    const T& operator[](std::size_t i) const { return data_[descriptor_(i)]; }

    template <typename Range> indirect_ndarray<T, 1> operator()(const Range& rng);
    ref_ndarray<T, 1> operator()(const base_slice<1>& slice);
    T& operator()(std::size_t i);

    template <typename Range> indirect_ndarray<const T, 1> operator()(const Range& rng) const;
    ref_ndarray<const T, 1> operator()(const base_slice<1>& slice) const;
    const T& operator()(std::size_t i) const;

    ref_ndarray_iterator<T, 1> begin() { return {*this, 0}; }
    ref_ndarray_iterator<T, 1> end() { return {*this, descriptor_.extent()}; }

    ref_ndarray_iterator<const T, 1> begin() const { return {*this, 0}; }
    ref_ndarray_iterator<const T, 1> end() const { return {*this, descriptor_.extent()}; }

    ref_ndarray_iterator<const T, 1> cbegin() const { return {*this, 0}; }
    ref_ndarray_iterator<const T, 1> cend() const { return {*this, descriptor_.extent()}; }

    std::size_t size() const { return descriptor_.size(); }
    const auto& extents() const { return descriptor_.extents(); }

    ref_ndarray_data_iterator<T, 1> data_begin() { return {data_, descriptor_.begin()}; }
    ref_ndarray_data_iterator<T, 1> data_end() { return {data_, descriptor_.end()}; }

    ref_ndarray_data_iterator<const T, 1> data_begin() const { return {data_, descriptor_.begin()}; }
    ref_ndarray_data_iterator<const T, 1> data_end() const { return {data_, descriptor_.end()}; }

    OPERATIONS_LIST((typename R), (const ref_ndarray<R, 1>&), 1)

  protected:
    ref_ndarray(T* data, const base_slice<1>& descriptor) : data_{data}, descriptor_{descriptor} {}
    ref_ndarray() : data_{nullptr} {}

    ref_ndarray(const ref_ndarray& source) = default;
    ref_ndarray(ref_ndarray&& source) = default;

    void clone_from(const ref_ndarray& source);
    void clone_from(ref_ndarray&& source);
    [[deprecated("use clone_from")]] void assign(const ref_ndarray& source);

    T* data_;
    base_slice<1> descriptor_;

  private:
    void check_assignment(const T* data, const base_slice<1> descriptor);
};

template <typename T, std::size_t N>
class ref_ndarray_iterator : public std::iterator<std::random_access_iterator_tag, ref_ndarray<T, N - 1>,
                                                  std::ptrdiff_t, ref_ndarray<T, N - 1>>
{
  public:
    ref_ndarray_iterator() = default;
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

template <typename T>
class ref_ndarray_iterator<T, 1> : public std::iterator<std::random_access_iterator_tag, T>
{
  public:
    // TODO iterators must have a default constructor  (check others)
    ref_ndarray_iterator() = default;
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

template <typename T, std::size_t N>
class ref_ndarray_data_iterator : public std::iterator<std::forward_iterator_tag, T>
{
  public:
    ref_ndarray_data_iterator() = default;
    ref_ndarray_data_iterator(T* data, const base_slice_iterator<N>& it) : data_{data}, it_{it} {}

    T& operator*() const { return data_[*it_]; }

    ref_ndarray_data_iterator& operator++();
    ref_ndarray_data_iterator operator++(int);
    bool operator==(const ref_ndarray_data_iterator& other) const { return it_ == other.it_; }
    bool operator!=(const ref_ndarray_data_iterator& other) const { return it_ != other.it_; }

    std::ptrdiff_t operator-(const ref_ndarray_data_iterator& other) const { return it_ - other.it_; }

    operator ref_ndarray_data_iterator<const T, N>() const { return {data_, it_}; }

  private:
    mutable T* data_ = nullptr;
    base_slice_iterator<N> it_;
};

FRIEND_OPERATIONS_DEFINITION((typename R, std::size_t M), (const ref_ndarray<R, M>&))

#include "array/detail/undef_macros.hpp"

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_REF_ARRAY_DECL_H
