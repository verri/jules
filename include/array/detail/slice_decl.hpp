#ifndef JULES_ARRAY_DETAIL_SLICE_DECL_H
#define JULES_ARRAY_DETAIL_SLICE_DECL_H

#include "util/numeric.hpp"

#include <array>

namespace jules
{
namespace detail
{
template <std::size_t N, typename... Types>
using all_size_enabler =
    std::enable_if_t<N == sizeof...(Types) && all_args(std::is_convertible<Types, std::size_t>::value...)>;

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
using element_request = std::enable_if_t<all_args(std::is_convertible<Args, std::size_t>::value...), Return>;
template <typename Return, typename... Args>
using slice_request = std::enable_if_t<all_args(size_or_slice<Args, sizeof...(Args)>()...) &&
                                           !all_args(std::is_convertible<Args, std::size_t>::value...),
                                       Return>;
template <typename Return, typename... Args>
using indirect_request = std::enable_if_t<!all_args(size_or_slice<Args, sizeof...(Args)>()...), Return>;

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_SLICE_DECL_H
