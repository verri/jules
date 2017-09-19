// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_RELATIVE_H
#define JULES_ARRAY_SLICING_RELATIVE_H

#include <jules/core/type.hpp>

#include <functional>

namespace jules
{
inline namespace slicing
{

template <typename F> class relative_index
{
public:
  template <typename G> friend constexpr auto make_relative_index(G g) noexcept -> relative_index<G>;

  relative_index() = delete;

  constexpr auto operator()(index_t size) const noexcept -> index_t { return f_(size); }

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
  return make_relative_index([ g = std::move(index), value = value ](index_t i) constexpr { return g(i) - value; });
}

template <typename G> constexpr auto operator/(relative_index<G> index, index_t value) noexcept
{
  return make_relative_index([ g = std::move(index), value = value ](index_t i) constexpr { return g(i) / value; });
}

inline static auto size = make_relative_index(+[](index_t i) constexpr { return i; });

struct relative_slice
{

  relative_slice() = delete;

  template <typename F> relative_slice(index_t start, relative_index<F> extent) : start{start}, extent{std::move(extent)} {}

  index_t start;
  std::function<index_t(index_t)> extent;
};

struct relative_strided_slice
{

  relative_strided_slice() = delete;

  template <typename F>
  relative_strided_slice(index_t start, relative_index<F> extent, index_t stride)
    : start{start}, extent{std::move(extent)}, stride{stride}
  {}

  index_t start;
  std::function<index_t(index_t)> extent;
  index_t stride;
};

template <typename F> auto slice(index_t start, relative_index<F> extent) -> relative_slice { return {start, std::move(extent)}; }

template <typename F> auto slice(index_t start, relative_index<F> extent, index_t stride) -> relative_strided_slice
{
  return {start, std::move(extent), stride};
}
} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_RELATIVE_H
