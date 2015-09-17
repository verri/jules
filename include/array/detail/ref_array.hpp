#ifndef JULES_ARRAY_DETAIL_REF_ARRAY_H
#define JULES_ARRAY_DETAIL_REF_ARRAY_H

#include "array/detail/ref_array_decl.hpp"

#define CHECK_ASSIGNMENT                                                                                     \
    do {                                                                                                     \
        if (this->data_ == data)                                                                             \
            throw std::runtime_error{"self-assignment is not supported"};                                    \
        if (!all(this->descriptor_.extents(), descriptor.extents()))                                         \
            throw std::out_of_range{"extents do not match"};                                                 \
    } while (false)

#define COPY_FROM_SOURCE                                                                                     \
    do {                                                                                                     \
        auto it = source.descriptor_.begin();                                                                \
        for (std::size_t i : this->descriptor_)                                                              \
            data_[i] = source.data_[*it++];                                                                  \
        return *this;                                                                                        \
    } while (false)

namespace jules
{
namespace detail
{
// Reference Array

#ifndef NDEBUG
template <typename T, std::size_t N>
void ref_ndarray<T, N>::check_assignment(const T* data, const base_slice<N> descriptor)
{
    CHECK_ASSIGNMENT;
}
#else
template <typename T, std::size_t N> void ref_ndarray<T, N>::check_assignment(const T*, const base_slice<N>)
{
}
#endif // NDEBUG

template <typename T, std::size_t N>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const ref_ndarray& source)
{
    check_assignment(source.data_, source.descriptor_);
    COPY_FROM_SOURCE;
}

template <typename T, std::size_t N>
template <typename U>
ref_ndarray<T, N>& ref_ndarray<T, N>::operator=(const ref_ndarray<U, N>& source)
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    check_assignment(nullptr, source.descriptor_);
    COPY_FROM_SOURCE;
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
    auto start = descriptor_.start() + descriptor_.extents(0) * i;
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
    auto start = descriptor_.start() + descriptor_.extents(0) * i;
    std::array<std::size_t, N - 1> extents, strides;

    const auto& e = descriptor_.extents();
    const auto& s = descriptor_.strides();

    std::copy(e.begin() + 1, e.end(), extents.begin());
    std::copy(s.begin() + 1, s.end(), strides.begin());

    return {data_, {start, extents, strides}};
}

// Reference Array Specialization

#ifndef NDEBUG
template <typename T> void ref_ndarray<T, 1>::check_assignment(const T* data, const base_slice<1> descriptor)
{
    CHECK_ASSIGNMENT;
}
#else
template <typename T> void ref_ndarray<T, 1>::check_assignment(const T*, const base_slice<1>) {}
#endif // 1DEBUG

template <typename T> ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const ref_ndarray& source)
{
    check_assignment(source.data_, source.descriptor_);
    COPY_FROM_SOURCE;
}

template <typename T>
template <typename U>
ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const ref_ndarray<U, 1>& source)
{
    check_assignment(nullptr, source.descriptor_);
    COPY_FROM_SOURCE;
}

template <typename T> template <typename U> ref_ndarray<T, 1>& ref_ndarray<T, 1>::operator=(const U& source)
{
    for (std::size_t i : descriptor_)
        data_[i] = source;
    return *this;
}

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

} // namespace detail
} // namespace jules

#undef CHECK_ASSIGNMENT
#undef COPY_FROM_SOURCE

#endif // JULES_ARRAY_DETAIL_REF_ARRAY_H
