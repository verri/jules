// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_INDEX_SPAN_H
/// \exclude
#define JULES_BASE_INDEX_SPAN_H

#include "range/v3/range/traits.hpp"
#include <jules/core/concepts.hpp>
#include <jules/core/type.hpp>

namespace jules
{

class index_span
{
public:
  /// \group member_types
  using value_type = index_t;

  /// \group member_types
  using size_type = index_t;

  /// \group member_types
  using difference_type = distance_t;

  /// \group member_types
  using const_reference = const value_type&;

  /// \group member_types
  using const_iterator = const index_t*;

  constexpr index_span(const index_t* begin, const index_t* end) noexcept : begin_{begin}, end_{end} {}

  constexpr index_span(const contiguous_of<index_t> auto& container) noexcept
    : index_span(container.data(), container.data() + container.size())
  {}

  template <ranges::range Rng>
    requires convertible_to<ranges::range_common_iterator_t<Rng>, const index_t*> &&
    (!contiguous_of<Rng, index_t>)constexpr index_span(const Rng& rng) noexcept : index_span(ranges::begin(rng), ranges::end(rng))
  {}

  [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator { return begin_; }
  [[nodiscard]] constexpr auto end() const noexcept -> const_iterator { return end_; }

  [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator { return begin_; }
  [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator { return end_; }

  [[nodiscard]] constexpr auto size() const noexcept -> size_type { return end_ - begin_; }

  constexpr auto operator[](index_t i) const noexcept -> const_reference { return *(begin_ + i); }

private:
  const index_t* begin_;
  const index_t* end_;
};

} // namespace jules

#endif // JULES_BASE_INDEX_SPAN_H
