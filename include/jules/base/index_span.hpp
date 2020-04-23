// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_INDEX_SPAN_H
/// \exclude
#define JULES_BASE_INDEX_SPAN_H

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

  index_span(const container_for<index_t> auto& container) : begin_(container.data()), end_(container.data() + container.size())
  {}

  [[nodiscard]] auto begin() const -> const_iterator { return begin_; }
  [[nodiscard]] auto end() const -> const_iterator { return end_; }

  [[nodiscard]] auto cbegin() const -> const_iterator { return begin_; }
  [[nodiscard]] auto cend() const -> const_iterator { return end_; }

  [[nodiscard]] auto size() const -> size_type { return end_ - begin_; }

  auto operator[](index_t i) const -> const_reference { return *(begin_ + i); }

private:
  const index_t* begin_;
  const index_t* end_;
};

} // namespace jules

#endif // JULES_BASE_INDEX_SPAN_H
