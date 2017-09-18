// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_SLICING_RANGE_H
#define JULES_ARRAY_SLICING_RANGE_H

#include <jules/array/slicing/relative.hpp>
#include <jules/core/type.hpp>

namespace jules
{
inline namespace slicing
{

struct range_slice {
  range_slice() = delete;
  constexpr range_slice(tag<range_slice>, index_t start, index_t last) noexcept : start{start}, last{last} {}

  index_t start;
  index_t last;
};

struct range_strided_slice {
  range_strided_slice() = delete;
  constexpr range_strided_slice(tag<range_slice>, index_t start, index_t last, index_t stride) noexcept
    : start{start}, last{last}, stride{stride}
  {
  }

  index_t start;
  index_t last;
  index_t stride;
};

struct relative_range_slice {
  relative_range_slice() = delete;
  relative_range_slice(tag<range_slice>, index_t start, relative_index last) : start{start}, last{std::move(last)} {}

  index_t start;
  relative_index last;
};

struct relative_range_strided_slice {
  relative_range_strided_slice() = delete;
  relative_range_strided_slice(tag<range_slice>, index_t start, relative_index last, index_t stride)
    : start{start}, last{std::move(last)}, stride{stride}
  {
  }

  index_t start;
  relative_index last;
  index_t stride;
};

class from_index
{
public:
  friend constexpr auto from(index_t start) noexcept -> from_index { return {{}, start}; }

  constexpr auto operator()() const noexcept { return start_; }

  operator relative_range_slice() const { return {start_, size}; }

  constexpr from_index(const from_index&) noexcept = default;
  constexpr from_index(from_index&&) noexcept = default;

  constexpr auto operator=(const from_index&) noexcept -> from_index& = default;
  constexpr auto operator=(from_index&&) noexcept -> from_index& = default;

private:
  constexpr from_index(tag<from_index>, index_t start) noexcept : start_{start} {}
  index_t start_;
};

class to_index
{
public:
  friend constexpr auto to(index_t last) noexcept -> to_index { return {{}, last}; }

  constexpr auto operator()() const noexcept { return last_; }

  constexpr to_index(const to_index&) noexcept = default;
  constexpr to_index(to_index&&) noexcept = default;

  constexpr auto operator=(const to_index&) noexcept -> to_index& = default;
  constexpr auto operator=(to_index&&) noexcept -> to_index& = default;

private:
  constexpr to_index(tag<to_index>, index_t last) noexcept : last_{last} {}
  index_t last_;
};

constexpr auto operator,(from_index f, to_index t) noexcept -> range_slice { return {f(), t()}; }

class relative_to_index
{
public:
  friend auto to(relative_index last) -> relative_to_index { return {{}, std::move(last)}; }

  auto operator()() const & { return last_; }
  auto operator()() && -> relative_index&& { return std::move(last_); }

  relative_to_index(const relative_to_index&) = default;
  relative_to_index(relative_to_index&&) noexcept = default;

  auto operator=(const relative_to_index&) -> relative_to_index& = default;
  auto operator=(relative_to_index&&) noexcept -> relative_to_index& = default;

private:
  relative_to_index(tag<to_index>, relative_index last) : last_{std::move(last)} {}
  relative_index last_;
};

auto operator,(from_index f, relative_to_index t) -> relative_range_slice { return {f(), std::move(t)()}; }

class by_index
{
public:
  friend constexpr auto by(index_t stride) noexcept -> by_index { return {{}, stride}; }

  constexpr auto operator()() const noexcept { return stride_; }

  constexpr by_index(const by_index&) noexcept = default;
  constexpr by_index(by_index&&) noexcept = default;

  constexpr auto operator=(const by_index&) noexcept -> by_index& = default;
  constexpr auto operator=(by_index&&) noexcept -> by_index& = default;

private:
  constexpr by_index(tag<to_index>, index_t stride) noexcept : stride_{stride} {}
  index_t stride_;
};

auto operator,(const range_slice& slice, by_index b) -> range_strided_slice { return {slice.start, slice.last, b()}; }

auto operator,(relative_range_slice slice, by_index b) -> relative_range_strided_slice
{
  return {slice.start, std::move(slice.last), b()};
}

} // namespace slicing
} // namespace jules

#endif // JULES_ARRAY_SLICING_RANGE_H
