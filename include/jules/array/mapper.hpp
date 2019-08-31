// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DETAIL_MAPPER_H
#define JULES_ARRAY_DETAIL_MAPPER_H

#include <jules/array/strided_descriptor.hpp>
#include <jules/base/index_view.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

template <std::size_t N> class identity_mapper
{
public:
  static constexpr auto order = N;
  using iterator = typename strided_descriptor<N>::iterator;

  constexpr identity_mapper(const strided_descriptor<N>& descriptor) noexcept : descriptor_{descriptor} {}

  constexpr auto map(index_t index) const noexcept { return index; }

  constexpr auto descriptor() const noexcept -> const strided_descriptor<N>& { return descriptor_; }
  constexpr auto index_begin() const noexcept -> iterator { return descriptor_.begin(); }
  constexpr auto index_end() const noexcept -> iterator { return descriptor_.end(); }

  constexpr auto drop_first_dimension(index_t i) const noexcept -> identity_mapper<N - 1>
  {
    auto new_descriptor = descriptor_.discard_dimension();
    new_descriptor.start = descriptor_.start + descriptor_.strides[0] * i;
    return {new_descriptor};
  }

  template <std::size_t D> constexpr auto drop_one_level_dimensions() const -> identity_mapper<D>
  {
    return {descriptor_.template drop_one_level_dimensions<D>()};
  }

private:
  strided_descriptor<N> descriptor_;
};

template <std::size_t N> class vector_mapper
{
public:
  static constexpr auto order = N;
  using iterator = typename index_view::const_iterator;

  vector_mapper(const std::array<index_t, N>& extents, index_view indexes)
    : descriptor_{0u, extents}, indexes_{std::move(indexes)}
  {
    DEBUG_ASSERT(indexes_.size() == descriptor_.size(), debug::default_module, debug::level::invalid_argument,
                 "invalid descriptor for mapper");
  }

  auto map(index_t index) const
  {
    DEBUG_ASSERT(index < indexes_.size(), debug::default_module, debug::level::boundary_check, "out of range");
    return indexes_[index];
  }

  auto map(std::vector<index_t> indexes) const -> index_view
  {
    for (auto& index : indexes)
      index = map(index);
    return {std::move(indexes)};
  }

  auto map(const strided_descriptor<N>& indexes) const -> index_view
  {
    auto result = std::vector<index_t>();
    result.reserve(indexes.size());
    for (const auto index : indexes)
      result.push_back(map(index));
    return {std::move(result)};
  }

  auto descriptor() const noexcept -> const strided_descriptor<N>& { return descriptor_; }
  auto index_begin() const noexcept -> iterator { return indexes_.begin(); }
  auto index_end() const noexcept -> iterator { return indexes_.end(); }

  auto drop_first_dimension(index_t i) const noexcept -> vector_mapper<N - 1>
  {
    auto dropped = descriptor_.discard_dimension();
    dropped.start = descriptor_.start + i; // strides[0] is always 1u for vector mapped.

    auto new_indexes = std::vector<index_t>();

    new_indexes.reserve(prod(dropped.extents));
    for (const auto index : dropped)
      new_indexes.push_back(map(index));

    return {dropped.extents, {std::move(new_indexes)}};
  }

  template <std::size_t D> constexpr auto drop_one_level_dimensions() const -> vector_mapper<D>
  {
    return {descriptor_.template drop_one_level_dimensions<D>(), indexes_};
  }

private:
  strided_descriptor<N> descriptor_;
  index_view indexes_;
};

} // namespace jules

#endif // JULES_ARRAY_DETAIL_MAPPER_H
