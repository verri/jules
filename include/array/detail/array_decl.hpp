#ifndef JULES_ARRAY_DETAIL_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_ARRAY_DECL_H

#include "util/numeric.hpp"

#include <array>
#include <utility>
#include <vector>

namespace jules
{
namespace detail
{
constexpr bool All() { return true; }
template <typename... Tail> constexpr bool All(bool head, Tail... tail) { return head && All(tail...); }

template <std::size_t N, typename... Types>
using all_size_enabler =
    std::enable_if_t<N == sizeof...(Types) && All(std::is_convertible<Types, std::size_t>::value...)>;

template <typename Range, typename R, typename T = void>
using range_type_enabler =
    std::enable_if_t<std::is_same<typename std::remove_reference<Range>::type::value_type, R>::value, T>;

template <std::size_t N> class base_slice
{
    static_assert(N > 0, "invalid slice dimension");

  public:
    class iterator
    {
      public:
        std::size_t operator*() const { return index(std::make_index_sequence<N>{}); }

        iterator& operator++();
        iterator operator++(int) const;
        bool operator==(const iterator& other) { return all(indexes, other.indexes); }

      private:
        template <std::size_t... I> auto index(std::index_sequence<I...>) { return (*slice)(indexes[I]...); }

        std::array<std::size_t, N> indexes = {{0}};
        base_slice<N>* slice;
    };

  public:
    base_slice() = default;

    base_slice(std::size_t start, std::initializer_list<std::size_t> extents);
    base_slice(std::size_t start, std::initializer_list<std::size_t> extents,
               std::initializer_list<std::size_t> strides);

    base_slice(const base_slice& source) = default;
    base_slice(base_slice&& source) = default;

    base_slice& operator=(const base_slice& source) = default;
    base_slice& operator=(base_slice&& source) = default;

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    std::size_t operator()(Dims... dims) const;

    auto start() const { return start_; }
    auto size() const { return size_; }
    const auto& extents() const { return extents_; }
    const auto& strides() const { return strides_; }

  private:
    std::size_t start_ = 0;
    std::size_t size_ = 0;
    std::array<std::size_t, N> extents_ = {{0}};
    std::array<std::size_t, N> strides_ = {{0}};
};

template <typename T, std::size_t N> constexpr bool size_or_slice()
{
    return std::is_convertible<T, std::size_t>::value || std::is_convertible<T, base_slice<N>>::value;
}

template <typename Return, typename... Args>
using element_request = std::enable_if_t<All(std::is_convertible<Args, std::size_t>::value...), Return>;
template <typename Return, typename... Args>
using slice_request = std::enable_if_t<All(size_or_slice<Args, sizeof...(Args)>()...) &&
                                           !All(std::is_convertible<Args, std::size_t>::value...),
                                       Return>;
template <typename Return, typename... Args>
using indirect_request = std::enable_if_t<!All(size_or_slice<Args, sizeof...(Args)>()...), Return>;

template <typename T, std::size_t N> class base_ndarray;
template <typename T, std::size_t N> class ref_ndarray;

template <typename T, std::size_t N> class indirect_ndarray
{
    template <typename U, std::size_t M> friend class base_ndarray;
    template <typename U, std::size_t M> friend class ref_ndarray;

  public:
  private:
    base_ndarray<T, N>* array_;
    std::vector<std::size_t> indexes[N];
};

template <typename T, std::size_t N> class ref_ndarray
{
  public:
    using value_type = T;
    static constexpr auto order = N;

    ref_ndarray() = delete;
    ~ref_ndarray() = default;

    ref_ndarray(const ref_ndarray& source) = delete;
    ref_ndarray(ref_ndarray&& source) = delete;

    template <typename U> ref_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> ref_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(ref_ndarray<U, N>&& source);

    template <typename U> ref_ndarray& operator=(const indirect_ndarray<U, N>& source);
    template <typename U> ref_ndarray& operator=(indirect_ndarray<U, N>&& source);

    template <typename U> ref_ndarray& operator=(const U& source);
    template <typename U> ref_ndarray& operator=(U&& source);

    ref_ndarray<T, N - 1> operator[](std::size_t i);
    ref_ndarray<const T, N - 1> operator[](std::size_t i) const;

    template <typename... Args> indirect_request<indirect_ndarray<T, N>, Args...> operator()(Args&&... args);
    template <typename... Args> slice_request<ref_ndarray<T, N>, Args...> operator()(Args&&... args);
    template <typename... Args> element_request<T&, Args...> operator()(Args&&... args);

    template <typename... Args>
    indirect_request<indirect_ndarray<const T, N>, Args...> operator()(Args&&... args) const;
    template <typename... Args>
    slice_request<ref_ndarray<const T, N>, Args...> operator()(Args&&... args) const;
    template <typename... Args> element_request<const T&, Args...> operator()(Args&&... args) const;

  protected:
    ref_ndarray(T* data, const base_slice<N>& descriptor) : data_{data}, descriptor_{descriptor} {}

    T* data_;
    base_slice<N> descriptor_;
};

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

    base_ndarray(const base_ndarray& source) = default;
    base_ndarray(base_ndarray&& source) = default;

    template <typename U> base_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> base_ndarray& operator=(const ref_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(ref_ndarray<U, N>&& source);

    template <typename U> base_ndarray& operator=(const indirect_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(indirect_ndarray<U, N>&& source);

    template <typename U> base_ndarray& operator=(const U& source);
    template <typename U> base_ndarray& operator=(U&& source);

    std::size_t size() const { return this->descriptor_.size(); }
    std::size_t size(std::size_t i) const { return this->descriptor_.extents()[i]; }

    T* data() { return this->data_; }
    const T* data() const { return this->data_; }
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_DECL_H
