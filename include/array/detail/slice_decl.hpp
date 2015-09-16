#ifndef JULES_ARRAY_DETAIL_SLICE_DECL_H
#define JULES_ARRAY_DETAIL_SLICE_DECL_H

#include "array/detail/utility.hpp"

#include <array>

namespace jules
{
namespace detail
{
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

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_SLICE_DECL_H
