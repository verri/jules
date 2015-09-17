#ifndef JULES_ARRAY_DETAIL_ARRAY_H
#define JULES_ARRAY_DETAIL_ARRAY_H

#include "array/detail/array_decl.hpp"
#include "util/async.hpp"

#define COPY_FROM_SOURCE                                                                                     \
    do {                                                                                                     \
        auto it = source.descriptor_.begin();                                                                \
        for (std::size_t i = 0; i < this->size(); ++i)                                                       \
            this->data_[i] = source.data_[*it++];                                                            \
        return *this;                                                                                        \
    } while (false)

// TODO: operator= debug checks

namespace jules
{
namespace detail
{
template <typename T, std::size_t N> void base_ndarray<T, N>::clear()
{
    if (data()) {
        for (auto it = data(); it != data() + this->size(); ++it)
            it->~T();
        delete[] reinterpret_cast<storage_t*>(data());
    }
}

template <typename T, std::size_t N> base_ndarray<T, N>::base_ndarray() : ref_ndarray<T, N>{nullptr, {}} {}

template <typename T, std::size_t N> base_ndarray<T, N>::~base_ndarray() { clear(); }

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(Dims... dims)
    : ref_ndarray<T, N>{reinterpret_cast<T*>(new storage_t[prod_args(dims...)]), {0, {std::size_t(dims)...}}}
{
    for (auto it = data(); it != data() + this->size(); ++it)
        new (it) T;
}

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(const T& value, Dims... dims)
    : ref_ndarray<T, N>{reinterpret_cast<T*>(new storage_t[prod_args(dims...)]), {0, {std::size_t(dims)...}}}
{
    for (auto it = data(); it != data() + this->size(); ++it)
        new (it) T(value);
}

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(const T* data, Dims... dims)
    : ref_ndarray<T, N>{reinterpret_cast<T*>(new storage_t[prod_args(dims...)]), {0, {std::size_t(dims)...}}}
{
    for (auto it = this->data(); it != this->data() + this->size(); ++it)
        new (it) T(*data++);
}

template <typename T, std::size_t N>
base_ndarray<T, N>::base_ndarray(const base_ndarray& source)
    : ref_ndarray<T, N>{reinterpret_cast<T*>(new storage_t[source.size()]), source.descriptor_}
{
    auto data = source.data();
    for (auto it = data(); it != data() + this->size(); ++it)
        new (it) T(*data++);
}

template <typename T, std::size_t N>
base_ndarray<T, N>::base_ndarray(base_ndarray&& source)
    : ref_ndarray<T, N>{move_ptr(source.data_), source.descriptor_}
{
}

template <typename T, std::size_t N>
auto base_ndarray<T, N>::operator=(const base_ndarray& source) -> base_ndarray &
{
    clear();

    this->data_ = reinterpret_cast<T*>(new storage_t[source.size()]);
    auto data = source.data();
    for (auto it = this->data(); it != this->data() + this->size(); ++it)
        new (it) T(*data++);

    this->descriptor_ = source.descriptor_;

    return *this;
}

template <typename T, std::size_t N>
auto base_ndarray<T, N>::operator=(base_ndarray&& source) -> base_ndarray &
{
    clear();

    this->data_ = move_ptr(source.data_);
    this->descriptor_ = std::move(source.descriptor_);

    return *this;
}

template <typename T, std::size_t N>
template <typename U>
auto base_ndarray<T, N>::operator=(const ref_ndarray<U, N>& source) -> base_ndarray &
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    COPY_FROM_SOURCE;
}

// template <typename T, std::size_t N>
// template <typename U> auto base_ndarray<T, N>::operator=(const indirect_ndarray<U, N>& source) ->
// base_ndarray& {}

template <typename T, std::size_t N>
template <typename U>
auto base_ndarray<T, N>::operator=(const U& source) -> base_ndarray &
{
    for (std::size_t i = 0; i < this->size(); ++i)
        this->data_[i] = source;
    return *this;
}

} // namespace detail
} // namespace jules

#undef COPY_FROM_SOURCE

#endif // JULES_ARRAY_DETAIL_ARRAY_H
