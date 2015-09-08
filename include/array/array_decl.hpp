#ifndef JULES_ARRAY_ARRAY_DECL_H
#define JULES_ARRAY_ARRAY_DECL_H

#include <array/detail/array.hpp>
#include "range/range.hpp"

namespace jules
{
template <typename T, std::size_t N> class ndarray : public detail::base_ndarray<T, N>
{
  public:
    ndarray() = default;
    ~ndarray() = default;

    ndarray(const ndarray& source) = default;
    ndarray(ndarray&& source) = default;

    ndarray& operator=(const ndarray& source) = default;
    ndarray& operator=(ndarray&& source) = default;
};

template <typename T> class ndarray<T, 1> : public detail::base_ndarray<T, 1>
{
  public:
    ndarray() = default;
    explicit ndarray(std::size_t size);

    ndarray(const T* data, std::size_t size);
    ndarray(const T& value, std::size_t size);

    template <typename Range, typename R = typename std::remove_reference<Range>::type::value_type>
    ndarray(Range&& rng);

    ~ndarray() = default;

    ndarray(const ndarray& source) = default;
    ndarray(ndarray&& source) = default;

    ndarray& operator=(const ndarray& source) = default;
    ndarray& operator=(ndarray&& source) = default;
};

template <typename T> class ndarray<T, 0>
{
  public:
    ndarray() = default;

    ndarray(const T& data) : data_{data} {}
    ndarray(T&& data) : data_{std::move(data)} {}

    ~ndarray() = default;

    ndarray(const ndarray& source) = default;
    ndarray(ndarray&& source) = default;

    ndarray& operator=(const ndarray& source) = default;
    ndarray& operator=(ndarray&& source) = default;

    ndarray& operator=(const T& source) { data_ = source; }
    ndarray& operator=(T&& source) { data_ = std::move(source); }

    operator T&() { return data_; }
    operator const T&() const { return data_; }

    constexpr std::size_t size() const { return 0; }

    T* data() { return &data_; }
    const T* data() const { return &data_; }

  private:
    T data_;
};

template <typename T> using matrix = ndarray<T, 2>;
template <typename T> using vector = ndarray<T, 1>;
template <typename T> using scalar = ndarray<T, 0>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_DECL_H
