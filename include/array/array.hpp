#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include "array/array_decl.hpp"

namespace jules
{
template <typename T>
ndarray<T, 2>::ndarray(std::size_t nrow, std::size_t ncol)
    : detail::base_ndarray<T, 2>(std::array<std::size_t, 2>{{nrow, ncol}})
{
}

template <typename T>
ndarray<T, 2>::ndarray(const T* data, std::size_t nrow, std::size_t ncol)
    : detail::base_ndarray<T, 2>({{nrow, ncol}}, data, nrow * ncol)
{
}

template <typename T>
ndarray<T, 2>::ndarray(const T& value, std::size_t nrow, std::size_t ncol)
    : detail::base_ndarray<T, 2>({{nrow, ncol}}, value, nrow * ncol)
{
}

template <typename T>
ndarray<T, 1>::ndarray(std::size_t size)
    : detail::base_ndarray<T, 1>(std::array<std::size_t, 1>{{size}})
{
}

template <typename T>
ndarray<T, 1>::ndarray(const T* data, std::size_t size)
    : detail::base_ndarray<T, 1>({{size}}, data, size)
{
}

template <typename T>
ndarray<T, 1>::ndarray(const T& value, std::size_t size)
    : detail::base_ndarray<T, 1>({{size}}, value, size)
{
}

template <typename T>
template <typename Range, typename R>
ndarray<T, 1>::ndarray(Range&& rng)
    : detail::base_ndarray<T, 1>(std::array<std::size_t, 1>{{static_cast<std::size_t>(range::size(rng))}})
{
    static_assert(std::is_assignable<T&, R>::value, "invalid values type");
    if (this->data_.size() != range::size(rng))
        throw std::runtime_error{"data is not initialized"};
    range::copy(std::forward<Range>(rng), std::begin(this->data_));
}

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
