#ifndef JULES_ARRAY_DETAIL_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_ARRAY_DECL_H

#include <array>
#include <valarray>

namespace jules
{
namespace detail
{
template <typename... Tail> constexpr bool all(bool head, Tail... tail) { return head && all(tail...); }
constexpr bool all() { return true; }
template <typename... Types>
using all_size_enabler = std::enable_if_t<All(std::is_convertible<Types, std::size_t>::value...)>;

template <typename Range, typename R, typename T = void>
using check_range_t = std::enable_if_t<std::is_same<std::remove_reference<Range>::type::value_type, R>, T>;

template <std::size_t N> class base_slice
{
  public:
    base_slice() = default;

    base_slice(std::size_t start, std::initializer_list<std::size_t> extents);
    base_slice(std::size_t start, std::initializer_list<std::size_t> extents,
               std::initializer_list<std::size_t> strides);

    base_slice(const base_slice& source) = default;
    base_slice(base_slice&& source) = default;

    base_slice& operator=(const base_slice& source) = default;
    base_slice& operator=(base_slice&& source) = default;

    template <typename... Dims, typename = all_size_enabler<Dims...>>
    std::size_t operator()(Dims... dims) const;

  private:
    std::size_t start_;
    std::array<std::size_t, N> extents_;
    std::array<std::size_t, N> strides_;
};

template <typename T, std::size_t N> class indirect_ndarray
{
};

template <typename T, std::size_t N> class ref_ndarray
{
  public:
    using value_type = typename std::valarray<T>::value_type;
    static constexpr auto order = N;

    ref_ndarray() = delete;
    ~ref_ndarray() = default;

    ref_ndarray(const ref_ndarray& source) = delete;
    ref_ndarray(ref_ndarray&& source) = delete;

    ref_ndarray& operator=(const ref_ndarray& source) = delete;
    ref_ndarray& operator=(ref_ndarray&& source) = delete;

    ref_ndarray<T, N - 1> operator[](std::size_t i);
    ref_ndarray<const T, N - 1> operator[](std::size_t i) const;

    template <typename... Args> indirect_request<indirect_ndarray<T, N>, Args...> operator()(Args...&& args);
    template <typename... Args> slice_request<ref_ndarray<T, N>, Args...> operator()(Args...&& args);


  protected:
    ref_ndarray(T* data, const descriptor_& descriptor) : data_{data}, descriptor_{descriptor} {}

  private:
    T* data_;
    slice<N> descriptor_;
};

template <typename T, std::size_t N> class base_ndarray : public ref_ndarray<T, N>
{
    base_ndarray() = default;
    ~base_ndarray() = default;

    template <typename... Dims, typename = all_size_enabler<Dims...>> explicit base_ndarray(Dims... dims);

    template <typename... Dims, typename = all_size_enabler<Dims...>>
    base_ndarray(const T& value, Dims... dims);

    template <typename... Dims, typename E = all_size_enabler<Dims...>>
    base_ndarray(const T* data, Dims... dims);

    base_ndarray(const base_ndarray& source) = default;
    base_ndarray(base_ndarray&& source) = default;

    base_ndarray& operator=(const base_ndarray& source) = default;
    base_ndarray& operator=(base_ndarray&& source) = default;

    // std::size_t size() const { return data_.size(); }
    // std::size_t size(std::size_t i) const { return dim_.at(i); }
    // template <std::size_t i> [[deprecated]] std::size_t size() const;

    T* data() { return data_.data(); }
    const T* data() const { return data_.data(); }

    auto begin() { return std::begin(data_); }
    auto end() { return std::end(data_); }

    auto begin() const { return std::begin(data_); }
    auto end() const { return std::end(data_); }

    auto cbegin() const { return std::begin(data_); }
    auto cend() const { return std::end(data_); }

  protected:
    base_slice<N> descriptor_;
    std::valarray<T> data_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_DECL_H
