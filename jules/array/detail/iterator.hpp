#ifndef JULES_ARRAY_DETAIL_ITERATOR_H
#define JULES_ARRAY_DETAIL_ITERATOR_H

#include <jules/array/detail/common.hpp>
#include <jules/core/type.hpp>

#include <iterator>

namespace jules
{
namespace detail
{
template <typename T, typename It> class iterator_from_indexes : public std::iterator<std::forward_iterator_tag, T, distance_t>
{
  template <typename, std::size_t> friend class ::jules::ref_array;
  template <typename, std::size_t> friend class ::jules::ind_array;

public:
  constexpr iterator_from_indexes() = default;

  constexpr iterator_from_indexes(const iterator_from_indexes& source) = default;
  constexpr iterator_from_indexes(iterator_from_indexes&& source) noexcept = default;

  constexpr iterator_from_indexes& operator=(const iterator_from_indexes& source) = default;
  constexpr iterator_from_indexes& operator=(iterator_from_indexes&& source) noexcept = default;

  constexpr auto operator++() -> iterator_from_indexes&
  {
    ++it_;
    return *this;
  }

  constexpr auto operator++(int) -> iterator_from_indexes
  {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  constexpr auto operator==(const iterator_from_indexes& other) const { return it_ == other.it_; }

  constexpr auto operator!=(const iterator_from_indexes& other) const { return !(*this == other); }

  constexpr auto operator*() -> T& { return data_[*it_]; }

  constexpr auto operator-> () -> T* { return data_ + *it_; }

private:
  iterator_from_indexes(T* data, It it) : data_{data}, it_{std::move(it)} {}

  T* data_ = nullptr;
  It it_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ITERATOR_H
