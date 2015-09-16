#ifndef JULES_ARRAY_DETAIL_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_ARRAY_DECL_H

#include "array/detail/slice.hpp"
#include "array/detail/ref_array.hpp"

#include <array>
#include <utility>
#include <vector>

namespace jules
{
namespace detail
{
template <typename T, std::size_t N> class base_ndarray : public ref_ndarray<T, N>
{
  private:
    using storage_t = std::aligned_storage_t<sizeof(T), alignof(T)>;

  public:
    base_ndarray();
    ~base_ndarray();

    template <typename... Dims, typename = all_size_enabler<N, Dims...>> explicit base_ndarray(Dims... dims);

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    base_ndarray(const T& value, Dims... dims);

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    base_ndarray(const T* data, Dims... dims);

    template <typename Range, typename R = typename std::remove_reference<Range>::type::value_type>
    base_ndarray(const Range& rng);

    base_ndarray(const base_ndarray& source);
    base_ndarray(base_ndarray&& source);

    base_ndarray& operator=(const base_ndarray& source);
    base_ndarray& operator=(base_ndarray&& source);

    // TODO(optimation): template <typename U> base_ndarray& operator=(const base_ndarray<U, N>& source);
    // TODO(optimation): template <typename U> base_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> base_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(const indirect_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(const U& source);

    T* data() { return this->data_; }
    const T* data() const { return this->data_; }

  private:
    void clear();
};

template <typename T> class base_ndarray<T, 0>
{
  public:
    using value_type = T;

    base_ndarray() = default;

    base_ndarray(const T& data) : data_{data} {}
    base_ndarray(T&& data) : data_{std::move(data)} {}

    ~base_ndarray() = default;

    base_ndarray(const base_ndarray& source) = default;
    base_ndarray(base_ndarray&& source) = default;

    base_ndarray& operator=(const base_ndarray& source) = default;
    base_ndarray& operator=(base_ndarray&& source) = default;

    base_ndarray& operator=(const T& source) { data_ = source; }
    base_ndarray& operator=(T&& source) { data_ = std::move(source); }

    operator T&() { return data_; }
    operator const T&() const { return data_; }

    constexpr std::size_t size() const { return 0; }

    T* data() { return &data_; }
    const T* data() const { return &data_; }

  private:
    T data_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_DECL_H
