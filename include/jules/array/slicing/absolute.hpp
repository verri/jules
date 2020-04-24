// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_ABSOLUTE_H
#define JULES_ARRAY_SLICING_ABSOLUTE_H

#include <jules/array/slicing/concepts.hpp>
#include <jules/core/type.hpp>

namespace jules
{
inline namespace slicing
{

class absolute_strided_slice
{
public:
  absolute_strided_slice() = delete;

  constexpr absolute_strided_slice(index_t start, index_t extent, index_t stride) noexcept
    : start_{start}, extent_{extent}, stride_{stride}
  {}

  [[nodiscard]] constexpr auto start() const noexcept { return start_; }
  [[nodiscard]] constexpr auto extent() const noexcept { return extent_; }
  [[nodiscard]] constexpr auto stride() const noexcept { return stride_; }

private:
  index_t start_;
  index_t extent_;
  index_t stride_;
};

class absolute_slice
{
public:
  absolute_slice() = delete;

  constexpr absolute_slice(index_t start, index_t extent) noexcept : start_{start}, extent_{extent} {}

  constexpr operator absolute_strided_slice() const noexcept { return {start_, extent_, 1u}; }

  [[nodiscard]] constexpr auto start() const noexcept { return start_; }
  [[nodiscard]] constexpr auto extent() const noexcept { return extent_; }

private:
  index_t start_;
  index_t extent_;
};

constexpr auto slice(index_t start, index_t extent) noexcept -> absolute_slice { return {start, extent}; }

constexpr auto slice(index_t start, index_t extent, index_t stride) noexcept -> absolute_strided_slice
{
  return {start, extent, stride};
}

template <> struct slice_traits<absolute_slice>
{
  using absolute_type = absolute_slice;
  struct absolutize_type
  {
    constexpr auto operator()(absolute_slice slice, index_t) const noexcept -> absolute_type { return slice; }
  };
};

template <> struct slice_traits<absolute_strided_slice>
{
  using absolute_type = absolute_strided_slice;
  struct absolutize_type
  {
    constexpr auto operator()(absolute_strided_slice slice, index_t) const noexcept -> absolute_type { return slice; }
  };
};

} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_ABSOLUTE_H
