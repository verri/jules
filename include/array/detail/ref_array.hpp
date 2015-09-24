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

#define COPY_FROM_REFERENCE                                                                                  \
    do {                                                                                                     \
        auto it = source.descriptor_.begin();                                                                \
        for (std::size_t i : this->descriptor_)                                                              \
            data_[i] = source.data_[*it++];                                                                  \
        return *this;                                                                                        \
    } while (false)

#define COPY_FROM_BASE                                                                                       \
    do {                                                                                                     \
        auto data = source.data_;                                                                            \
        for (std::size_t i : this->descriptor_)                                                              \
            data_[i] = *data++;                                                                              \
        return *this;                                                                                        \
    } while (false)

#define MOVE_FROM_BASE                                                                                       \
    do {                                                                                                     \
        auto data = source.data_;                                                                            \
        for (std::size_t i : this->descriptor_)                                                              \
            data_[i] = std::move(*data++);                                                                   \
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
    COPY_FROM_REFERENCE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const base_ndarray<U, N>& source)
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    COPY_FROM_BASE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(base_ndarray<U, N>&& source)
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    MOVE_FROM_BASE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const ref_ndarray<U, N>& source)
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    COPY_FROM_REFERENCE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const U& source)
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    for (std::size_t i : descriptor_)
        data_[i] = source;
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
    COPY_FROM_REFERENCE;
}

template <typename T>
template <typename U>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const base_ndarray<U, 1>& source)
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    COPY_FROM_BASE;
}

template <typename T>
template <typename U>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(base_ndarray<U, 1>&& source)
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    MOVE_FROM_BASE;
}

template <typename T>
template <typename U>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const ref_ndarray<U, 1>& source)
{
    CHECK_ASSIGNMENT(source.data_, source.descriptor_);
    COPY_FROM_REFERENCE;
}

template <typename T> template <typename U> ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const U& source)
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    for (std::size_t i : descriptor_)
        data_[i] = source;
    return *this;
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
#undef COPY_FROM_REFERENCE
#undef COPY_FROM_BASE
#undef MOVE_FROM_BASE

#endif // JULES_ARRAY_DETAIL_REF_ARRAY_H
