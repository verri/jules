#ifndef JULES_ARRAY_DETAIL_ARRAY_H
#define JULES_ARRAY_DETAIL_ARRAY_H

#include "array/detail/array_decl.hpp"
#include "util/async.hpp"

#define COPY_FROM_BASE                                                                                       \
    do {                                                                                                     \
        clear();                                                                                             \
        this->data_ = allocate(source.size());                                                               \
        auto data = source.data();                                                                           \
        for (auto it = this->data(); it != this->data() + this->size(); ++it)                                \
            new (it) T(*data++);                                                                             \
        this->descriptor_ = source.descriptor_;                                                              \
        return *this;                                                                                        \
    } while (false)

#define MOVE_FROM_BASE                                                                                       \
    do {                                                                                                     \
        clear();                                                                                             \
        this->data_ = move_ptr(source.data_);                                                                \
        this->descriptor_ = std::move(source.descriptor_);                                                   \
        return *this;                                                                                        \
    } while (false)

#define COPY_FROM_REF                                                                                        \
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
template <typename T, std::size_t N> base_ndarray<T, N>::base_ndarray() : ref_ndarray<T, N>{nullptr, {}} {}
template <typename T, std::size_t N> base_ndarray<T, N>::~base_ndarray() { clear(); }

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(Dims... dims)
    : ref_ndarray<T, N>{allocate(prod_args(dims...)), {0, {std::size_t(dims)...}}}
{
    create(data(), this->size());
}

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(const T& value, Dims... dims)
    : ref_ndarray<T, N>{allocate(prod_args(dims...)), {0, {std::size_t(dims)...}}}
{
    create(data(), this->size(), value);
}

template <typename T, std::size_t N>
template <typename... Dims, typename>
base_ndarray<T, N>::base_ndarray(const T* data, Dims... dims)
    : ref_ndarray<T, N>{allocate(prod_args(dims...)), {0, {std::size_t(dims)...}}}
{
    for (auto it = this->data(); it != this->data() + this->size(); ++it)
        new (it) T(*data++);
}

template <typename T, std::size_t N>
base_ndarray<T, N>::base_ndarray(const base_ndarray& source)
    : ref_ndarray<T, N>{allocate(source.size()), source.descriptor_}
{
    auto data = source.data();
    for (auto it = this->data(); it != this->data() + this->size(); ++it)
        new (it) T(*data++);
}

template <typename T, std::size_t N>
base_ndarray<T, N>::base_ndarray(base_ndarray&& source)
    : ref_ndarray<T, N>{move_ptr(source.data_), source.descriptor_}
{
}

template <typename T, std::size_t N>
template <typename U>
base_ndarray<T, N>::base_ndarray(const base_ndarray<U, N>& source)
    : ref_ndarray<T, N>{allocate(source.size()), source.descriptor_}
{
    static_assert(std::is_constructible<T, const U&>::value, "invalid values type");
    auto data = source.data();
    for (auto it = this->data(); it != this->data() + this->size(); ++it)
        new (it) T(*data++);
}

template <typename T, std::size_t N>
auto base_ndarray<T, N>::operator=(const base_ndarray& source) -> base_ndarray &
{
    COPY_FROM_BASE;
}

template <typename T, std::size_t N>
auto base_ndarray<T, N>::operator=(base_ndarray&& source) -> base_ndarray &
{
    MOVE_FROM_BASE;
}

template <typename T, std::size_t N>
template <typename U>
auto base_ndarray<T, N>::operator=(const base_ndarray<U, N>& source) -> base_ndarray &
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    COPY_FROM_BASE;
}

template <typename T, std::size_t N>
template <typename U>
auto base_ndarray<T, N>::operator=(const ref_ndarray<U, N>& source) -> base_ndarray &
{
    static_assert(std::is_assignable<T&, U>::value, "invalid values type");
    COPY_FROM_REF;
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

template <typename T, std::size_t N>
template <typename... Args>
void base_ndarray<T, N>::create(T* data, std::size_t size, Args&&... args)
{
    for (std::size_t i = 0; i < size; ++i)
        new (data + i) T(std::forward<Args>(args)...);
}

template <typename T, std::size_t N> void base_ndarray<T, N>::destroy(T* data, std::size_t size)
{
    for (std::size_t i = 0; i < size; ++i)
        data[i].~T();
}

template <typename T, std::size_t N> void base_ndarray<T, N>::clear()
{
    if (data()) {
        destroy(data(), this->size());
        deallocate(data(), this->size());
    }
}

} // namespace detail
} // namespace jules

#undef COPY_FROM_BASE
#undef MOVE_FROM_BASE
#undef COPY_FROM_REF

#endif // JULES_ARRAY_DETAIL_ARRAY_H
