#ifndef JULES_ARRAY_DETAIL_INDIRECT_ARRAY_H
#define JULES_ARRAY_DETAIL_INDIRECT_ARRAY_H

#include "array/detail/indirect_array_decl.hpp"

#ifndef NDEBUG
#define CHECK_ASSIGNMENT(data, descriptor)                                                                   \
    do {                                                                                                     \
        if (this->data_ == data)                                                                             \
            throw std::runtime_error{"self-assignment is not supported"};                                    \
        if (!all(this->descriptor_.extents(), descriptor.extents()))                                         \
            throw std::out_of_range{"extents do not match"};                                                 \
    } while (false)
#else
#define CHECK_ASSIGNMENT(data, descriptor)
#endif // NDEBUG

#define CHECK_ASSIGNABLE(U)                                                                                  \
    static_assert(std::is_assignable<T&, U>::value,                                                          \
                  "The elements with values of type U can not be assigned to the values with"                \
                  "type T of the N-dimensional array reference.")

#define COPY_FROM_SOURCE                                                                                     \
    do {                                                                                                     \
        auto from = source.data_begin();                                                                     \
        auto to = this->data_begin();                                                                        \
        auto end = this->data_end();                                                                         \
        while (to != end) {                                                                                  \
            *to = *from;                                                                                     \
            ++to;                                                                                            \
            ++from;                                                                                          \
        }                                                                                                    \
        return *this;                                                                                        \
    } while (false)

#define MOVE_FROM_SOURCE                                                                                     \
    do {                                                                                                     \
        auto from = source.data_begin();                                                                     \
        auto to = this->data_begin();                                                                        \
        auto end = this->data_end();                                                                         \
        while (to != end) {                                                                                  \
            *to = std::move(*from);                                                                          \
            ++to;                                                                                            \
            ++from;                                                                                          \
        }                                                                                                    \
        return *this;                                                                                        \
    } while (false)

namespace jules
{
namespace detail
{
template <typename T, std::size_t N>
indirect_ndarray<T, N>& indirect_ndarray<T, N>::operator=(const indirect_ndarray& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
indirect_ndarray<T, N>& indirect_ndarray<T, N>::operator=(const base_ndarray<U, N>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
indirect_ndarray<T, N>& indirect_ndarray<T, N>::operator=(base_ndarray<U, N>&& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    MOVE_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
indirect_ndarray<T, N>& indirect_ndarray<T, N>::operator=(const ref_ndarray<U, N>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
indirect_ndarray<T, N>& indirect_ndarray<T, N>::operator=(const indirect_ndarray<U, N>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename LhsIt, typename RhsIt, typename F>
indirect_ndarray<T, N>& indirect_ndarray<T, N>::
operator=(const binary_expr_ndarray<LhsIt, RhsIt, F, N>& source)
{
    using U = typename binary_expr_ndarray<LhsIt, RhsIt, F, N>::value_type;
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename It, typename F>
indirect_ndarray<T, N>& indirect_ndarray<T, N>::operator=(const unary_expr_ndarray<It, F, N>& source)
{
    using U = typename unary_expr_ndarray<It, F, N>::value_type;
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
indirect_ndarray<T, N>& indirect_ndarray<T, N>::operator=(const U& source)
{
    CHECK_ASSIGNABLE(U);

    auto to = this->data_begin();
    auto end = this->data_end();

    while (to != end) {
        *to = source;
        ++to;
    }

    return *this;
}

template <typename T, std::size_t N>
indirect_ndarray<T, N - 1> indirect_ndarray<T, N>::operator[](std::size_t i)
{
    std::vector<std::size_t> indexes;
    indexes.reserve(this->size() / this->size(0));

    std::array<std::size_t, N - 1> extents;
    std::copy(std::begin(this->extents()) + 1, std::end(this->extents()), std::begin(extents));

    std::copy(indexes_.begin() + i * this->size(0),
              indexes_.begin() + i * this->size(0) + this->size() / this->size(0),
              std::back_inserter(indexes));

    return {data_, extents, std::move(indexes)};
}

template <typename T, std::size_t N>
indirect_ndarray<const T, N - 1> indirect_ndarray<T, N>::operator[](std::size_t i) const
{
    return static_cast<indirect_ndarray<const T, N>>(*this)[i];
}

template <typename T, std::size_t N>
template <typename... Args>
indirect_request<indirect_ndarray<T, N>, Args...> indirect_ndarray<T, N>::operator()(Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");

    auto slicing = indirect_slicing(this->descriptor_, std::forward<Args>(args)...);

    std::vector<std::size_t> indexes;
    indexes.reserve(slicing.second.size());

    for (std::size_t j : slicing.second)
        indexes.push_back(this->indexes_[j]);

    return {data_, slicing.first, std::move(indexes)};
}

template <typename T, std::size_t N>
template <typename... Args>
slice_request<indirect_ndarray<T, N>, Args...> indirect_ndarray<T, N>::operator()(Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");

    auto slice = default_slicing(this->descriptor_, std::forward<Args>(args)...);

    std::vector<std::size_t> indexes;
    indexes.reserve(slice.size());

    for (std::size_t j : slice)
        indexes.push_back(this->indexes_[j]);

    return {data_, slice.extents(), std::move(indexes)};
}

template <typename T, std::size_t N>
template <typename... Args>
element_request<T&, Args...> indirect_ndarray<T, N>::operator()(Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");
    return data_[indexes_[descriptor_(std::size_t(args)...)]];
}

template <typename T, std::size_t N>
template <typename... Args>
indirect_request<indirect_ndarray<const T, N>, Args...> indirect_ndarray<T, N>::
operator()(Args&&... args) const
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");

    auto slicing = indirect_slicing(this->descriptor_, std::forward<Args>(args)...);

    std::vector<std::size_t> indexes;
    indexes.reserve(slicing.second.size());

    for (std::size_t j : slicing.second)
        indexes.push_back(this->indexes_[j]);

    return {data_, slicing.first, std::move(indexes)};
}

template <typename T, std::size_t N>
template <typename... Args>
slice_request<indirect_ndarray<const T, N>, Args...> indirect_ndarray<T, N>::operator()(Args&&... args) const
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");

    auto slice = default_slicing(this->descriptor_, std::forward<Args>(args)...);

    std::vector<std::size_t> indexes;
    indexes.reserve(slice.size());

    for (std::size_t j : slice)
        indexes.push_back(this->indexes_[j]);

    return {data_, slice.extents(), std::move(indexes)};
}

template <typename T, std::size_t N>
template <typename... Args>
element_request<const T&, Args...> indirect_ndarray<T, N>::operator()(Args&&... args) const
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");
    return data_[indexes_[descriptor_(std::size_t(args)...)]];
}

template <typename T, std::size_t N>
indirect_ndarray<T, N>::indirect_ndarray(T* data, const std::array<std::size_t, N>& extents,
                                         std::vector<std::size_t>&& indexes)
    : data_{data}, descriptor_{0, extents}, indexes_{std::move(indexes)}
{
}

template <typename T, std::size_t N>
indirect_ndarray<T, N>::indirect_ndarray(T* data, const std::array<std::size_t, N>& extents,
                                         const std::vector<std::size_t>& indexes)
    : data_{data}, descriptor_{0, extents}, indexes_{indexes}
{
}

template <typename T, std::size_t N> void indirect_ndarray<T, N>::clone_from(const indirect_ndarray& source)
{
    this->data_ = source.data_;
    this->descriptor_ = source.descriptor_;
    this->indexes_ = source.indexes;
}

template <typename T, std::size_t N> void indirect_ndarray<T, N>::clone_from(indirect_ndarray&& source)
{
    this->data_ = source.data_;
    this->descriptor_ = std::move(source.descriptor_);
    this->indexes_ = std::move(source.indexes);
}

template <typename T, std::size_t N>
indirect_ndarray_iterator<T, N>::indirect_ndarray_iterator(const indirect_ndarray<T, N>& array,
                                                           std::size_t index)
    : array_{array}, index_{index}
{
}

// Specialization

template <typename T>
indirect_ndarray<T, 1>& indirect_ndarray<T, 1>::operator=(const indirect_ndarray& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename U>
indirect_ndarray<T, 1>& indirect_ndarray<T, 1>::operator=(const base_ndarray<U, 1>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename U>
indirect_ndarray<T, 1>& indirect_ndarray<T, 1>::operator=(base_ndarray<U, 1>&& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    MOVE_FROM_SOURCE;
}

template <typename T>
template <typename U>
indirect_ndarray<T, 1>& indirect_ndarray<T, 1>::operator=(const ref_ndarray<U, 1>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename U>
indirect_ndarray<T, 1>& indirect_ndarray<T, 1>::operator=(const indirect_ndarray<U, 1>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename LhsIt, typename RhsIt, typename F>
indirect_ndarray<T, 1>& indirect_ndarray<T, 1>::
operator=(const binary_expr_ndarray<LhsIt, RhsIt, F, 1>& source)
{
    using U = typename binary_expr_ndarray<LhsIt, RhsIt, F, 1>::value_type;
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename It, typename F>
indirect_ndarray<T, 1>& indirect_ndarray<T, 1>::operator=(const unary_expr_ndarray<It, F, 1>& source)
{
    using U = typename unary_expr_ndarray<It, F, 1>::value_type;
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename U>
indirect_ndarray<T, 1>& indirect_ndarray<T, 1>::operator=(const U& source)
{
    CHECK_ASSIGNABLE(U);

    auto to = this->data_begin();
    auto end = this->data_end();

    while (to != end) {
        *to = source;
        ++to;
    }

    return *this;
}

template <typename T>
template <typename Range>
indirect_ndarray<T, 1> indirect_ndarray<T, 1>::operator()(const Range& rng)
{
    auto slicing = indirect_slicing(this->descriptor_, rng);

    std::vector<std::size_t> indexes;
    indexes.reserve(slicing.second.size());

    for (std::size_t j : slicing.second)
        indexes.push_back(this->indexes_[j]);

    return {data_, slicing.first, std::move(indexes)};
}

template <typename T> indirect_ndarray<T, 1> indirect_ndarray<T, 1>::operator()(const base_slice<1>& s)
{
    auto slice = default_slicing(this->descriptor_, s);

    std::vector<std::size_t> indexes;
    indexes.reserve(slice.size());

    for (std::size_t j : slice)
        indexes.push_back(this->indexes_[j]);

    return {data_, slice.extents(), std::move(indexes)};
}

template <typename T> T& indirect_ndarray<T, 1>::operator()(std::size_t i)
{
    return data_[indexes_[descriptor_(i)]];
}

template <typename T>
template <typename Range>
indirect_ndarray<const T, 1> indirect_ndarray<T, 1>::operator()(const Range& rng) const
{
    auto slicing = indirect_slicing(this->descriptor_, rng);

    std::vector<std::size_t> indexes;
    indexes.reserve(slicing.second.size());

    for (std::size_t j : slicing.second)
        indexes.push_back(this->indexes_[j]);

    return {data_, slicing.first, std::move(indexes)};
}

template <typename T>
indirect_ndarray<const T, 1> indirect_ndarray<T, 1>::operator()(const base_slice<1>& s) const
{
    auto slice = default_slicing(this->descriptor_, s);

    std::vector<std::size_t> indexes;
    indexes.reserve(slice.size());

    for (std::size_t j : slice)
        indexes.push_back(this->indexes_[j]);

    return {data_, slice.extents(), std::move(indexes)};
}

template <typename T> const T& indirect_ndarray<T, 1>::operator()(std::size_t i) const
{
    return data_[indexes_[descriptor_(i)]];
}

template <typename T>
indirect_ndarray<T, 1>::indirect_ndarray(T* data, const std::array<std::size_t, 1>& extents,
                                         std::vector<std::size_t>&& indexes)
    : data_{data}, descriptor_{0, extents}, indexes_{std::move(indexes)}
{
}

template <typename T>
indirect_ndarray<T, 1>::indirect_ndarray(T* data, const std::array<std::size_t, 1>& extents,
                                         const std::vector<std::size_t>& indexes)
    : data_{data}, descriptor_{0, extents}, indexes_{indexes}
{
}

template <typename T> void indirect_ndarray<T, 1>::clone_from(const indirect_ndarray& source)
{
    this->data_ = source.data_;
    this->descriptor_ = source.descriptor_;
    this->indexes_ = source.indexes;
}

template <typename T> void indirect_ndarray<T, 1>::clone_from(indirect_ndarray&& source)
{
    this->data_ = source.data_;
    this->descriptor_ = std::move(source.descriptor_);
    this->indexes_ = std::move(source.indexes);
}

template <typename T>
indirect_ndarray_iterator<T, 1>::indirect_ndarray_iterator(const indirect_ndarray<T, 1>& array,
                                                           std::size_t index)
    : array_{array}, index_{index}
{
}

// Data Iterator

template <typename T>
indirect_ndarray_data_iterator<T>::indirect_ndarray_data_iterator(T* data,
                                                                  const std::vector<std::size_t>& indexes,
                                                                  std::size_t i)
    : data_{data}, indexes_{indexes}, current_{i}
{
}

template <typename T>
std::ptrdiff_t indirect_ndarray_data_iterator<T>::operator-(const indirect_ndarray_data_iterator& other) const
{
    return current_ - other.current_;
}

} // namespace detail
} // namespace jules

#undef CHECK_ASSIGNMENT
#undef CHECK_ASSIGNABLE

#undef COPY_FROM_SOURCE
#undef MOVE_FROM_SOURCE

#endif // JULES_ARRAY_DETAIL_INDIRECT_ARRAY_H
