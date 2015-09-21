#ifndef JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H

#include "array/detail/common.hpp"

namespace jules
{
namespace detail
{
template <typename T, std::size_t N> class indirect_ndarray
{
    template <typename, std::size_t> friend class base_ndarray;
    template <typename, std::size_t> friend class ref_ndarray;

  public:
    using value_type = T;
    static constexpr auto order = N;

    using iterator = ref_ndarray_iterator<T, N>;
    using const_iterator = ref_ndarray_iterator<const T, N>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    ~indirect_ndarray() = default;

    indirect_ndarray& operator=(const indirect_ndarray& source);

    template <typename U> indirect_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> indirect_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> indirect_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> indirect_ndarray& operator=(const indirect_ndarray<U, N>& source);
    template <typename U> indirect_ndarray& operator=(const U& source);

    operator indirect_ndarray<const T, N>() const { return {array_, indexes_}; }

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

    indirect_ndarray_iterator<T, 1> begin();
    indirect_ndarray_iterator<T, 1> end();

    indirect_ndarray_iterator<const T, 1> begin() const;
    indirect_ndarray_iterator<const T, 1> end() const;

    indirect_ndarray_iterator<const T, 1> cbegin() const;
    indirect_ndarray_iterator<const T, 1> cend() const;

    std::size_t size() const { return size_helper(std::make_index_sequence<N>()); }
    std::size_t size(std::size_t i) const { return indexes_[i].size(); }

    auto extents() const { return extents_helper(std::make_index_sequence<N>()); }

    std::size_t nrow() const { return size(0); }
    std::size_t ncol() const { return size(1); }

    indirect_ndarray_data_iterator<T, 1> data_begin();
    indirect_ndarray_data_iterator<T, 1> data_end();

    indirect_ndarray_data_iterator<const T, 1> data_begin() const;
    indirect_ndarray_data_iterator<const T, 1> data_end() const;

  private:
    template <std::size_t... I> std::array<std::size_t, N> extents_helper(std::index_sequence<I...>) const;
    template <std::size_t... I> std::size_t size_helper(std::index_sequence<I...>) const;

    indirect_ndarray() = default;
    indirect_ndarray(const ref_ndarray<T, N>& source, std::vector<std::size_t>(&&indexes)[N]);
    indirect_ndarray(const ref_ndarray<T, N>& source, const std::vector<std::size_t>(&indexes)[N]);

    indirect_ndarray(const indirect_ndarray& source) = default;
    indirect_ndarray(indirect_ndarray&& source) = default;

    void clone_from(const indirect_ndarray& source);
    void clone_from(indirect_ndarray&& source);

    ref_ndarray<T, N> array_;
    std::vector<std::size_t> indexes_[N];
};

// TODO: iterators and specializations like ref_ndarray

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H
