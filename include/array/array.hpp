#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include <cstddef>

namespace jules
{

namespace detail
{

template <typename T>
class base_array
{
public:
    explicit base_array(T *data, std::size_t size) : data_{data}, size_{size} {}

    base_array(const base_array& source) = delete;
    base_array(base_array&& source) : data_{source.data_}, size_{source.size_} {
        source.data_ = nullptr;
        source.size_ = 0;
    }

    base_array& operator=(const base_array& source) = delete;
    base_array& operator=(base_array&& source) {
        data_ = source.data_;
        size_ = source.size_;

        source.data_ = nullptr;
        source.size_ = 0;

        return *this;
    }

    template <typename F> base_array& apply(F&& f) {
        for (std::size_t i{}; i < size_; ++i)
            data_[i] = f(data_[i]);
        return *this;
    }

private:
    T* data_;
    std::size_t size_;
};

} // namespace detail

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H

