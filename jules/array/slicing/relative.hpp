// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_RELATIVE_H
#define JULES_ARRAY_SLICING_RELATIVE_H

#include <jules/core/type.hpp>

#include <functional>

namespace jules
{
inline namespace slicing
{

// clang-format off
class relative_index
{
public:
  static auto identity() -> relative_index { return {{}, [](index_t value) { return value; }}; }

  auto operator-(index_t value) const & -> relative_index
  {
    return {{}, [ value, g = f_ ](index_t size) { return g(size) - value; }};
  }

  auto operator-(index_t value) && -> relative_index
  {
    return {{}, [ value, g = std::move(f_) ](index_t size) { return g(size) - value; }};
  }

  auto operator/(index_t value) const & -> relative_index
  {
    return {{}, [ value, g = f_ ](index_t size){return g(size) / value; }};
  }

  auto operator/(index_t value) && -> relative_index
  {
    return {{}, [ value, g = std::move(f_) ](index_t size){return g(size) / value; }};
  }

  auto operator()(index_t size) const { return f_(size); }

  relative_index(const relative_index&) = default;
  relative_index(relative_index&&) noexcept = default;

  auto operator=(const relative_index&) -> relative_index& = default;
  auto operator=(relative_index&&) noexcept -> relative_index& = default;

private:
  relative_index(tag<relative_index>, std::function<index_t(index_t)> f) : f_{std::move(f)} {}
  std::function<index_t(index_t)> f_;
};
// clang-format on

inline static auto size = relative_index::identity();

struct relative_slice {
  index_t start;
  relative_index extent;
};

struct relative_strided_slice {
  index_t start;
  relative_index extent;
  index_t stride;
};

auto slice(index_t start, relative_index extent) -> relative_slice { return {start, extent}; }

auto slice(index_t start, relative_index extent, index_t stride) -> relative_strided_slice { return {start, extent, stride}; }
} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_RELATIVE_H
