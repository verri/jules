#ifndef JULES_CORE_DETAIL_NUMBERS_H
#define JULES_CORE_DETAIL_NUMBERS_H

#include <jules/core/type.hpp>

#include <iterator>
#include <type_traits>

namespace jules
{
namespace detail
{

template <typename T> class number_iterator
{
  static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");

public:
  using iterator_category = std::input_iterator_tag;
  using value_type = T;
  using difference_type = distance_t;
  using pointer = const T*;
  using reference = const T&;

  constexpr number_iterator() = default;
  constexpr number_iterator(T current, T step) : current_{std::move(current)}, step_{std::move(step)} {}

  constexpr number_iterator(const number_iterator& source) = default;
  constexpr number_iterator(number_iterator&& source) noexcept = default;

  constexpr number_iterator& operator=(const number_iterator& source) = default;
  constexpr number_iterator& operator=(number_iterator&& source) noexcept = default;

  constexpr auto operator==(const number_iterator& other) const { return current_ == other.current_; }
  constexpr auto operator!=(const number_iterator& other) const { return current_ != other.current_; }
  constexpr auto operator<(const number_iterator& other) const { return current_ < other.current_; }
  constexpr auto operator>(const number_iterator& other) const { return current_ > other.current_; }
  constexpr auto operator>=(const number_iterator& other) const { return current_ >= other.current_; }
  constexpr auto operator<=(const number_iterator& other) const { return current_ <= other.current_; }

  constexpr auto operator*() const -> reference { return current_; }
  constexpr auto operator-> () const -> pointer { return &current_; }
  constexpr auto operator[](difference_type i) const { return current_ + i * step_; }

  constexpr auto operator++() -> number_iterator&
  {
    current_ += step_;
    return *this;
  }

  constexpr auto operator++(int) -> number_iterator
  {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  constexpr auto operator--() -> number_iterator&
  {
    current_ -= step_;
    return *this;
  }

  constexpr auto operator--(int) -> number_iterator
  {
    auto copy = *this;
    --(*this);
    return copy;
  }

  constexpr auto operator+=(difference_type i) -> number_iterator&
  {
    current_ += i * step_;
    return *this;
  }

  constexpr auto operator-=(difference_type i) -> number_iterator&
  {
    current_ -= i * step_;
    return *this;
  }

  constexpr auto operator+(difference_type i) -> number_iterator { return {current_ + i * step_, step_}; }
  constexpr auto operator-(difference_type i) -> number_iterator { return {current_ - i * step_, step_}; }
  constexpr friend auto operator+(difference_type i, const number_iterator& it) -> number_iterator { return it + i; }
  constexpr friend auto operator-(difference_type i, const number_iterator& it) -> number_iterator { return it - i; }

private:
  T current_;
  T step_;
};

} // namespace detail
} // namespace jules

#endif // JULES_CORE_DETAIL_NUMBERS_H
