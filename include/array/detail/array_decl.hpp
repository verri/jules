#ifndef JULES_ARRAY_DETAIL_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_ARRAY_DECL_H

#include <valarray>

namespace jules
{
namespace detail
{
template <typename Type, typename... Types> struct all_size_helper : public std::false_type {};
template <typename... Types> struct all_size_helper<std::size_t, Types...> : public all_size_helper<Types...> {};
template <> struct all_size_helper<std::size_t> : public std::true_type {};
template <typename... Types> using all_size_t = std::enable_if_t<all_size_helper<Types...>::value>;


template <std::size_t N> class base_slice {};
template <typename T, std::size_t N> class base_ref_ndarray {};

template <typename T, std::size_t N> class base_ndarray
{
  public:
    using value_type = typename std::valarray<T>::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;

    static constexpr auto order = N;

    base_ndarray() = default;
    ~base_ndarray() = default;

    template <typename... Dims, typename E = all_size_t<Dims...>>
    explicit base_ndarray(Dims... dims);

    template <typename... Dims, typename E = all_size_t<Dims...>>
    base_ndarray(const T& value, Dims... dims);

    template <typename... Dims, typename E = all_size_t<Dims...>>
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
