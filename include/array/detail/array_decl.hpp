#ifndef JULES_ARRAY_DETAIL_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_ARRAY_DECL_H

#include <array>
#include <valarray>

namespace jules
{
namespace detail
{
template <typename T, std::size_t N> class base_ndarray
{
  public:
    base_ndarray() = default;

    explicit base_ndarray(const std::array<std::size_t, N>& dim);

    explicit base_ndarray(const T* data, const std::array<std::size_t, N>& dim);
    explicit base_ndarray(const T& value, const std::array<std::size_t, N>& dim);
    template <typename Range>
    explicit base_ndarray(Range&& range, const std::array<std::size_t, N>& dim);

    ~base_ndarray() = default;

    base_ndarray(const base_ndarray& source) = default;
    base_ndarray(base_ndarray&& source) = default;

    base_ndarray& operator=(const base_ndarray& source) = default;
    base_ndarray& operator=(base_ndarray&& source) = default;

    std::size_t size() const { return data_.size(); }
    template <std::size_t i> std::size_t size() const {
        static_assert(i < N, "invalid dimension");
        return dim_.at(i);
    }

    T* data() { return data_.data(); }
    const T* data() const { return data_.data(); }

    auto begin() { return std::begin(data_); }
    auto end() { return std::end(data_); }

    auto cbegin() const { return std::begin(data_); }
    auto cend() const { return std::end(data_); }

  protected:
    std::array<std::size_t, N> dim_;
    std::valarray<T> data_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_DECL_H
