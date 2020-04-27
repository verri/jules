// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DETAIL_MAPPER_H
#define JULES_ARRAY_DETAIL_MAPPER_H

#include <jules/array/descriptor.hpp>
#include <jules/array/strided_descriptor.hpp>
#include <jules/base/index_span.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

template <std::size_t N> class strided_mapper : private strided_descriptor<N>
{
public:
  static constexpr auto order = N;
  using iterator = typename strided_descriptor<N>::iterator;

  constexpr strided_mapper(const strided_descriptor<N>& descriptor) noexcept : strided_descriptor<N>{descriptor} {}

  constexpr auto index_begin() const noexcept -> iterator { return as_descriptor().begin(); }
  constexpr auto index_end() const noexcept -> iterator { return as_descriptor().end(); }

  template <std::size_t D> constexpr auto drop_one_level_dimensions() const -> strided_mapper<D>
  {
    return {as_descriptor().template drop_one_level_dimensions<D>()};
  }

  constexpr auto as_descriptor() const -> const strided_descriptor<N>&
  {
    return static_cast<const strided_descriptor<N>&>(*this);
  }
  constexpr auto as_descriptor() -> strided_descriptor<N>& { return static_cast<strided_descriptor<N>&>(*this); }

  using strided_descriptor<N>::size;
  using strided_descriptor<N>::dimensions;
  using strided_descriptor<N>::operator();
};

template <std::size_t N> class container_mapper
{
public:
  static constexpr auto order = N;
  using iterator = typename container<index_t>::const_iterator;

  container_mapper(const std::array<index_t, N>& extents, container<index_t> indexes)
    : descriptor_{0u, extents}, indexes_(std::move(indexes))
  {
    DEBUG_ASSERT(indexes_.size() == descriptor_.size(), debug::default_module, debug::level::invalid_argument,
                 "invalid descriptor for mapper");
  }

  [[nodiscard]] auto operator()(const std::array<index_t, N>& pos) const
  {
    DEBUG_ASSERT(pos < indexes_.size(), debug::default_module, debug::level::boundary_check, "out of range");
    return indexes_[descriptor_(pos)];
  }

  [[nodiscard]] auto index_begin() const noexcept -> iterator { return indexes_.begin(); }
  [[nodiscard]] auto index_end() const noexcept -> iterator { return indexes_.end(); }

  template <std::size_t D> constexpr auto drop_one_level_dimensions() const -> container_mapper<D>
  {
    return {descriptor_.template drop_one_level_dimensions<D>(), indexes_};
  }

  constexpr auto size() const noexcept { return indexes_.size(); }

  constexpr auto dimensions() const noexcept { return descriptor_.dimensions(); }

private:
  descriptor<N> descriptor_;
  container<index_t> indexes_;
};

class span_mapper
{
public:
  static constexpr auto order = 1;
  using iterator = typename index_span::const_iterator;

  constexpr span_mapper(index_span indexes) : indexes_(indexes) {}

  [[nodiscard]] auto operator()(index_t pos) const
  {
    DEBUG_ASSERT(pos < indexes_.size(), debug::default_module, debug::level::boundary_check, "out of range");
    return indexes_[pos];
  }

  [[nodiscard]] auto operator()(const std::array<index_t, 1>& pos) const { return (*this)(pos[0]); }

  [[nodiscard]] constexpr auto index_begin() const noexcept -> iterator { return indexes_.begin(); }
  [[nodiscard]] constexpr auto index_end() const noexcept -> iterator { return indexes_.end(); }

private:
  index_span indexes_;
};

} // namespace jules

#endif // JULES_ARRAY_DETAIL_MAPPER_H
