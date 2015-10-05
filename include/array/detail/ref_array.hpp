#ifndef JULES_ARRAY_DETAIL_REF_ARRAY_H
#define JULES_ARRAY_DETAIL_REF_ARRAY_H

#include "array/detail/ref_array_decl.hpp"

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
// Reference Array

template <typename T, std::size_t N>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const ref_ndarray& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const base_ndarray<U, N>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(base_ndarray<U, N>&& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    MOVE_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const ref_ndarray<U, N>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const indirect_ndarray<U, N>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename LhsIt, typename RhsIt, typename F>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const binary_expr_ndarray<LhsIt, RhsIt, F, N>& source)
{
    using U = typename binary_expr_ndarray<LhsIt, RhsIt, F, N>::value_type;
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename It, typename F>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const unary_expr_ndarray<It, F, N>& source)
{
    using U = typename unary_expr_ndarray<It, F, N>::value_type;
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const U& source)
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

template <typename T, std::size_t N> ref_ndarray<T, N - 1> ref_ndarray<T, N>::operator[](std::size_t i)
{
    auto start = descriptor_.start() + descriptor_.stride(0) * i;
    std::array<std::size_t, N - 1> extents, strides;

    const auto& e = descriptor_.extents();
    const auto& s = descriptor_.strides();

    std::copy(e.begin() + 1, e.end(), extents.begin());
    std::copy(s.begin() + 1, s.end(), strides.begin());

    return {data_, {start, extents, strides}};
}

template <typename T, std::size_t N>
ref_ndarray<const T, N - 1> ref_ndarray<T, N>::operator[](std::size_t i) const
{
    return static_cast<ref_ndarray<const T, N>>(*this)[i];
}

template <typename T, std::size_t N>
template <typename... Args>
indirect_request<indirect_ndarray<T, N>, Args...> ref_ndarray<T, N>::operator()(Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");

    auto slicing = indirect_slicing(this->descriptor_, std::forward<Args>(args)...);
    return {data_, slicing.first, std::move(slicing.second)};
}

template <typename T, std::size_t N>
template <typename... Args>
slice_request<ref_ndarray<T, N>, Args...> ref_ndarray<T, N>::operator()(Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");
    auto slice = default_slicing(this->descriptor_, std::forward<Args>(args)...);
    return {data_, slice};
}

template <typename T, std::size_t N>
template <typename... Args>
element_request<T&, Args...> ref_ndarray<T, N>::operator()(Args&&... args)
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");
    return data_[descriptor_(std::size_t(args)...)];
}

template <typename T, std::size_t N>
template <typename... Args>
indirect_request<indirect_ndarray<const T, N>, Args...> ref_ndarray<T, N>::operator()(Args&&... args) const
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
slice_request<ref_ndarray<const T, N>, Args...> ref_ndarray<T, N>::operator()(Args&&... args) const
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");
    auto slice = default_slicing(this->descriptor_, std::forward<Args>(args)...);
    return {data_, slice};
}

template <typename T, std::size_t N>
template <typename... Args>
element_request<const T&, Args...> ref_ndarray<T, N>::operator()(Args&&... args) const
{
    static_assert(sizeof...(args) == N, "Invalid number of arguments.");
    return data_[descriptor_(std::size_t(args)...)];
}

template <typename T, std::size_t N> void ref_ndarray<T, N>::clone_from(const ref_ndarray& source)
{
    data_ = source.data_;
    descriptor_ = source.descriptor_;
}

template <typename T, std::size_t N> void ref_ndarray<T, N>::clone_from(ref_ndarray&& source)
{
    data_ = move_ptr(source.data_);
    descriptor_ = std::move(source.descriptor_);
}

template <typename T, std::size_t N> void ref_ndarray<T, N>::assign(const ref_ndarray& source)
{
    clone_from(source);
}

// Reference Array Specialization

template <typename T> ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const ref_ndarray& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename U>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const base_ndarray<U, 1>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename U>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(base_ndarray<U, 1>&& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    MOVE_FROM_SOURCE;
}

template <typename T>
template <typename U>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const ref_ndarray<U, 1>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename U>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const indirect_ndarray<U, 1>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename LhsIt, typename RhsIt, typename F>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const binary_expr_ndarray<LhsIt, RhsIt, F, 1>& source)
{
    using U = typename binary_expr_ndarray<LhsIt, RhsIt, F, 1>::value_type;
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename It, typename F>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const unary_expr_ndarray<It, F, 1>& source)
{
    using U = typename unary_expr_ndarray<It, F, 1>::value_type;
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    CHECK_ASSIGNABLE(U);
    COPY_FROM_SOURCE;
}

template <typename T> template <typename U> ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const U& source)
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
indirect_ndarray<T, 1> ref_ndarray<T, 1>::operator()(const Range& rng)
{
    // TODO: Range&& and specialization of indirect_slicing
    auto slicing = indirect_slicing(this->descriptor_, rng);

    std::vector<std::size_t> indexes;
    indexes.reserve(slicing.second.size());

    for (std::size_t j : slicing.second)
        indexes.push_back(j);

    return {data_, slicing.first, std::move(indexes)};
}

template <typename T> ref_ndarray<T, 1> ref_ndarray<T, 1>::operator()(const base_slice<1>& s)
{
    auto slice = default_slicing(this->descriptor_, s);
    return {data_, slice};
}

template <typename T> T& ref_ndarray<T, 1>::operator()(std::size_t i) { return data_[descriptor_(i)]; }

template <typename T>
template <typename Range>
indirect_ndarray<const T, 1> ref_ndarray<T, 1>::operator()(const Range& rng) const
{
    auto slicing = indirect_slicing(this->descriptor_, rng);

    std::vector<std::size_t> indexes;
    indexes.reserve(slicing.second.size());

    for (std::size_t j : slicing.second)
        indexes.push_back(j);

    return {data_, slicing.first, std::move(indexes)};
}

template <typename T> ref_ndarray<const T, 1> ref_ndarray<T, 1>::operator()(const base_slice<1>& s) const
{
    auto slice = default_slicing(this->descriptor_, s);

    std::vector<std::size_t> indexes;
    indexes.reserve(slice.size());

    for (std::size_t j : slice)
        indexes.push_back(this->indexes_[j]);

    return {data_, slice.extents(), std::move(indexes)};
}

template <typename T> const T& ref_ndarray<T, 1>::operator()(std::size_t i) const
{
    return data_[descriptor_(i)];
}

template <typename T> void ref_ndarray<T, 1>::clone_from(const ref_ndarray& source)
{
    data_ = source.data_;
    descriptor_ = source.descriptor_;
}

template <typename T> void ref_ndarray<T, 1>::clone_from(ref_ndarray&& source)
{
    data_ = move_ptr(source.data_);
    descriptor_ = std::move(source.descriptor_);
}

template <typename T> void ref_ndarray<T, 1>::assign(const ref_ndarray& source) { clone_from(source); }

// Iterator

template <typename T, std::size_t N> auto ref_ndarray_iterator<T, N>::operator++() -> ref_ndarray_iterator &
{
    ++index_;
    return *this;
}

template <typename T, std::size_t N> auto ref_ndarray_iterator<T, N>::operator++(int) -> ref_ndarray_iterator
{
    ref_ndarray_iterator copy = *this;
    ++index_;
    return copy;
}

// Iterator Specialization

template <typename T> auto ref_ndarray_iterator<T, 1>::operator++() -> ref_ndarray_iterator &
{
    ++index_;
    return *this;
}

template <typename T> auto ref_ndarray_iterator<T, 1>::operator++(int) -> ref_ndarray_iterator
{
    ref_ndarray_iterator copy = *this;
    ++index_;
    return copy;
}

// Data Iterator

template <typename T, std::size_t N>
auto ref_ndarray_data_iterator<T, N>::operator++() -> ref_ndarray_data_iterator &
{
    ++it_;
    return *this;
}

template <typename T, std::size_t N>
auto ref_ndarray_data_iterator<T, N>::operator++(int) -> ref_ndarray_data_iterator
{
    auto c = *this;
    ++it_;
    return c;
}

} // namespace detail
} // namespace jules

#undef CHECK_ASSIGNMENT
#undef CHECK_ASSIGNABLE

#undef COPY_FROM_SOURCE
#undef MOVE_FROM_SOURCE

#endif // JULES_ARRAY_DETAIL_REF_ARRAY_H
