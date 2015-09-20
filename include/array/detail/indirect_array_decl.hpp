#ifndef JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H

#include "array/detail/utility.hpp"

namespace jules
{
namespace detail
{
template <typename T, std::size_t N> class indirect_ndarray
{
    template <typename, std::size_t> friend class base_ndarray;
    template <typename, std::size_t> friend class ref_ndarray;

  public:
    ~indirect_ndarray() = default;

    indirect_ndarray& operator=(const indirect_ndarray& source);

    template <typename U> indirect_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> indirect_ndarray& operator=(base_ndarray<U, N>&& source);

    template <typename U> indirect_ndarray& operator=(const ref_ndarray<U, N>& source);
    // TODO: template <typename U> ref_ndarray& operator=(const indirect_ndarray<U, N>& source);
    template <typename U> indirect_ndarray& operator=(const U& source);

    operator indirect_ndarray<const T, N>() const { return {array_, indexes_}; }

    auto extents() const { return extents_helper(std::make_index_sequence<N>()); }

  private:
    template <std::size_t... I> std::array<std::size_t, N> extents_helper(std::index_sequence<I...>) const;

    indirect_ndarray() = default;
    indirect_ndarray(const ref_ndarray<T, N>& source, std::vector<std::size_t>(&&indexes)[N]);
    indirect_ndarray(const ref_ndarray<T, N>& source, const std::vector<std::size_t>(&indexes)[N]);

    indirect_ndarray(const indirect_ndarray& source) = default;
    indirect_ndarray(indirect_ndarray&& source) = default;

    ref_ndarray<T, N> array_;
    std::vector<std::size_t> indexes_[N];
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_INDIRECT_ARRAY_DECL_H
