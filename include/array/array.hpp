#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include "util/numeric.hpp"

#include <array>
#include <valarray>

namespace jules
{
template <typename T, std::size_t N> class array
{
    static_assert(N > 0, "array dimension must be greater than 0.");

  public:
    array() = default;

    explicit array(const std::array<std::size_t, N>& dim) : dim_(dim), data_(prod(dim)){};

    explicit array(const T* data, const std::array<std::size_t, N>& dim) : dim_(dim), data_(data, prod(dim))
    {
    }
    template <typename... Dim>
    explicit array(const T* data, Dim... dim)
        : dim_{dim...}, data_(data, prod(dim_))
    {
    }

    explicit array(T*&& data, const std::array<std::size_t, N>& dim) : dim_(dim), data_(data, prod(dim)) {}
    template <typename... Dim> explicit array(T*&& data, Dim... dim) : dim_{dim...}, data_(data, prod(dim_))
    {
    }

    ~array() = default;

    array(const array& source) = default;
    array(array&& source) = default;

    array& operator=(const array& source) = default;
    array& operator=(array&& source) = default;

    template <typename F> array& apply(F&& f)
    {
        for (auto&& value : data_)
            value = f(value);
        return *this;
    }

    std::size_t size() const { return data_.size(); }
    std::size_t size(std::size_t i) const { return dim_.at(i); }

    T* data() { return data_.data(); }
    const T* data() const { return data_.data(); }

  private:
    std::array<std::size_t, N> dim_;
    std::valarray<T> data_;
};

template <typename T> using matrix = array<T, 2>;
template <typename T> using vector = array<T, 1>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
