#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include "util/numeric.hpp"

#include <array>
#include <cstddef>

namespace jules
{
namespace detail
{
template <typename T, std::size_t N> class base_array
{
    static_assert(N > 0, "array dimension must be greater than 0.");

  public:
    explicit base_array(T* data, const std::array<std::size_t, N>& dim) : data_{data}, dim_(dim) {}
    template <typename... Dim> explicit base_array(T* data, Dim... dim) : data_{data}, dim_{dim...} {}

    base_array(const base_array& source) = delete;
    base_array(base_array&& source) : data_{source.data_}, dim_(source.dim_)
    {
        source.data_ = nullptr;
        for (auto&& d : dim_)
            d = 0;
    }

    base_array& operator=(const base_array& source) = delete;
    base_array& operator=(base_array&& source)
    {
        data_ = source.data_;
        dim_ = source.dim_;

        source.data_ = nullptr;
        for (auto&& d : dim_)
            d = 0;

        return *this;
    }

    template <typename F> base_array& apply(F&& f)
    {
        for (std::size_t i{}; i < size(); ++i)
            data_[i] = f(data_[i]);
        return *this;
    }

    std::size_t size() const { return prod(dim_); }

  private:
    T* data_;
    std::array<std::size_t, N> dim_;
};

template <typename T> using base_matrix = base_array<T, 2>;
template <typename T> using base_vector = base_array<T, 1>;

} // namespace detail

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
