// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_RELATIVE_H
#define JULES_ARRAY_SLICING_RELATIVE_H

#include <jules/array/slicing/absolute.hpp>
#include <jules/core/type.hpp>
#include <type_traits>

namespace jules
{
inline namespace slicing
{

template <typename F> class relative_index
{
public:
  static_assert(std::is_nothrow_invocable_r_v<index_t, F, index_t>);

  template <typename G> friend constexpr auto make_relative_index(G g) noexcept -> relative_index<G>;

  relative_index() = delete;

  [[nodiscard]] constexpr auto operator()(index_t size) const noexcept -> index_t { return f_(size); }

  constexpr relative_index(const relative_index&) noexcept = default;
  constexpr relative_index(relative_index&&) noexcept = default;

  constexpr auto operator=(const relative_index&) noexcept -> relative_index& = default;
  constexpr auto operator=(relative_index&&) noexcept -> relative_index& = default;

private:
  constexpr relative_index(tag<relative_index>, F f) noexcept : f_{std::move(f)} {}
  F f_;
};

template <typename G> constexpr auto make_relative_index(G g) noexcept -> relative_index<G> { return {{}, std::move(g)}; }

template <typename G> constexpr auto operator-(relative_index<G> index, index_t value) noexcept
{
  return make_relative_index([ g = std::move(index), value = value ](index_t i) constexpr noexcept { return g(i) - value; });
}

template <typename G> constexpr auto operator-(index_t value, relative_index<G> index) noexcept
{
  return make_relative_index([ g = std::move(index), value = value ](index_t i) constexpr noexcept { return value - g(i); });
}

template <typename G> constexpr auto operator+(relative_index<G> index, index_t value) noexcept
{
  return make_relative_index([ g = std::move(index), value = value ](index_t i) constexpr noexcept { return g(i) + value; });
}

template <typename G> constexpr auto operator+(index_t value, relative_index<G> index) noexcept { return index + value; }

template <typename G> constexpr auto operator/(relative_index<G> index, index_t value) noexcept
{
  return make_relative_index([ g = std::move(index), value = value ](index_t i) constexpr noexcept { return g(i) / value; });
}

template <typename G> constexpr auto operator/(index_t value, relative_index<G> index) noexcept
{
  return make_relative_index([ g = std::move(index), value = value ](index_t i) constexpr noexcept { return value / g(i); });
}

template <typename G> constexpr auto operator*(relative_index<G> index, index_t value) noexcept
{
  return make_relative_index([ g = std::move(index), value = value ](index_t i) constexpr noexcept { return g(i) * value; });
}

template <typename G> constexpr auto operator*(index_t value, relative_index<G> index) noexcept { return index * value; }

template <typename F, typename G> constexpr auto operator+(relative_index<F> lhs, relative_index<G> rhs) noexcept
{
  return make_relative_index([ lhs = std::move(lhs), rhs = rhs ](index_t i) constexpr noexcept { return lhs(i) + rhs(i); });
}

template <typename F, typename G> constexpr auto operator-(relative_index<F> lhs, relative_index<G> rhs) noexcept
{
  return make_relative_index([ lhs = std::move(lhs), rhs = rhs ](index_t i) constexpr noexcept { return lhs(i) - rhs(i); });
}

template <typename F, typename G> constexpr auto operator*(relative_index<F> lhs, relative_index<G> rhs) noexcept
{
  return make_relative_index([ lhs = std::move(lhs), rhs = rhs ](index_t i) constexpr noexcept { return lhs(i) * rhs(i); });
}

template <typename F, typename G> constexpr auto operator/(relative_index<F> lhs, relative_index<G> rhs) noexcept
{
  return make_relative_index([ lhs = std::move(lhs), rhs = rhs ](index_t i) constexpr noexcept { return lhs(i) / rhs(i); });
}

constexpr auto dim() noexcept
{
  return make_relative_index(+[](index_t size) constexpr noexcept { return size; });
};

template <typename F> struct relative_slice
{
public:
  relative_slice() = delete;

  relative_slice(index_t start, relative_index<F> extent) : start_{start}, extent_{std::move(extent)} {}

  [[nodiscard]] constexpr auto start() const noexcept { return start_; }
  [[nodiscard]] constexpr auto extent() const noexcept { return extent_; }

private:
  index_t start_;
  relative_index<F> extent_;
};

template <typename F> struct relative_strided_slice
{
public:
  relative_strided_slice() = delete;

  relative_strided_slice(index_t start, relative_index<F> extent, index_t stride)
    : start_{start}, extent_{std::move(extent)}, stride_{stride}
  {}

  [[nodiscard]] constexpr auto start() const noexcept { return start_; }
  [[nodiscard]] constexpr auto extent() const noexcept { return extent_; }
  [[nodiscard]] constexpr auto stride() const noexcept { return stride_; }

private:
  index_t start_;
  relative_index<F> extent_;
  index_t stride_;
};

template <typename F> auto slice(index_t start, relative_index<F> extent) -> relative_slice<F>
{
  return {start, std::move(extent)};
}

template <typename F> auto slice(index_t start, relative_index<F> extent, index_t stride) -> relative_strided_slice<F>
{
  return {start, std::move(extent), stride};
}

template <typename F> struct slice_traits<relative_slice<F>>
{
  using absolute_type = absolute_slice;
  struct absolutize_type
  {
    constexpr auto operator()(relative_slice<F> slice, index_t dim) const noexcept -> absolute_type
    {
      return {slice.start(), slice.extent()(dim)};
    }
  };
};

template <typename F> struct slice_traits<relative_strided_slice<F>>
{
  using absolute_type = absolute_strided_slice;
  struct absolutize_type
  {
    constexpr auto operator()(relative_strided_slice<F> slice, index_t dim) const noexcept -> absolute_type
    {
      return {slice.start(), slice.extent()(dim), slice.stride()};
    }
  };
};

} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_RELATIVE_H
