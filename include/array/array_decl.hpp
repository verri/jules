#ifndef JULES_ARRAY_ARRAY_DECL_H
#define JULES_ARRAY_ARRAY_DECL_H

#include "core/type.hpp"

#include <array>
#include <valarray>

namespace jules
{
template <typename T, std::size_t N> class array
{
  public:
    array() = default;

    explicit array(const std::array<std::size_t, N>& dim);

    explicit array(const T* data, const std::array<std::size_t, N>& dim);
    template <typename Range, typename R = typename range_traits<Range>::value_type>
    explicit array(Range&& range, const std::array<std::size_t, N>& dim);

    ~array() = default;

    array(const array& source) = default;
    array(array&& source) = default;

    array& operator=(const array& source) = default;
    array& operator=(array&& source) = default;

    std::size_t size() const { return data_.size(); }
    std::size_t size(std::size_t i) const { return dim_.at(i); }

    // XXX
    // std::enable_if_t<N==1, std::size_t> length() const { return size(); }
    // std::enable_if_t<N>=2, std::size_t> nrow() const { return size(0); }
    // std::enable_if_t<N>=2, std::size_t> ncol() const { return size(1); }

    T* data() { return data_.data(); }
    const T* data() const { return data_.data(); }

  private:
    std::array<std::size_t, N> dim_;
    std::valarray<T> data_;
};

template <typename T> class array<T, 0>
{
  public:
    array() = default;

    array(const T& data) : data_{data} {}
    array(T&& data) : data_{std::move(data)} {}

    ~array() = default;

    array(const array& source) = default;
    array(array&& source) = default;

    array& operator=(const array& source) = default;
    array& operator=(array&& source) = default;

    array& operator=(const T& source) { data_ = source; }
    array& operator=(T&& source) { data_ = std::move(source); }

    operator T&() { return data_; }
    operator const T&() { return data_; }

    constexpr std::size_t size() const { return 0; }

    T* data() { return &data_; }
    const T* data() const { return &data_; }

  private:
    T data_;
};

template <typename T> using matrix = array<T, 2>;
template <typename T> using vector = array<T, 1>;
template <typename T> using scalar = array<T, 0>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_DECL_H
