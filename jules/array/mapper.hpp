// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DETAIL_MAPPER_H
#define JULES_ARRAY_DETAIL_MAPPER_H

#include <jules/array/strided_descriptor.hpp>
#include <jules/base/const_vector.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules::detail
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

  constexpr auto drop_dimension(index_t i) const noexcept -> identity_mapper<N - 1>
  {
    auto new_descriptor = descriptor_.drop_dimension();
    new_descriptor.start = descriptor_.start + descriptor_.strides[0] * i;
    return {new_descriptor};
  }

private:
  strided_descriptor<N> descriptor_;
};

template <std::size_t N> class vector_mapper
{
public:
  static constexpr auto order = N;
  using iterator = typename const_vector<index_t>::const_iterator;

  vector_mapper(const strided_descriptor<N>& descriptor, const_vector<index_t> indexes)
    : descriptor_{descriptor}, indexes_{std::move(indexes)}
  {
  }

  auto map(index_t index) const
  {
    DEBUG_ASSERT(index < indexes_.size(), debug::default_module, debug::level::boundary_check, "out of range");
    return indexes_[index];
  }

  auto map(const_vector<index_t>::container_type indexes) const -> const_vector<index_t>
  {
    for (auto& index : indexes)
      index = map(index);
    return {std::move(indexes)};
  }

  auto map(const strided_descriptor<N>& indexes) const -> const_vector<index_t>
  {
    auto result = const_vector<index_t>::container_type();
    result.reserve(indexes.size());
    for (const auto index : indexes)
      result.push_back(map(index));
    return {std::move(result)};
  }

  auto descriptor() const noexcept -> const strided_descriptor<N>& { return descriptor_; }
  auto index_begin() const noexcept -> iterator { return indexes_.begin(); }
  auto index_end() const noexcept -> iterator { return indexes_.end(); }

  auto drop_dimension(index_t i) const noexcept -> vector_mapper<N - 1>
  {

    auto dropped = descriptor_.drop_dimension();
    dropped.start = descriptor_.start + i; // strides[0] is always 1u for vector mapped.

    const auto new_descriptor = strided_descriptor<N - 1>{0u, dropped.extents};
    auto new_indexes = typename const_vector<index_t>::container_type();

    new_indexes.reserve(new_descriptor.size());
    for (const auto index : dropped)
      new_indexes.push_back(map(index));

    return {new_descriptor, {std::move(new_indexes)}};
  }

private:
  strided_descriptor<N> descriptor_;
  const_vector<index_t> indexes_;
};

} // namespace jules::detail

#endif // JULES_ARRAY_DETAIL_MAPPER_H
