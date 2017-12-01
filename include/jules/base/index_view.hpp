// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_INDEX_VIEW_H
/// \exclude
#define JULES_BASE_INDEX_VIEW_H

#include <jules/base/const_vector.hpp>
#include <jules/core/range.hpp>
#include <jules/core/type.hpp>

#include <optional>

namespace jules
{

// TODO: Better name instead of "view"
class index_view
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

  index_view(const_vector<index_t> data) : data_{std::move(data)}, begin_(data_->data()), end_(data_->data() + data_->size()) {}

  index_view(const std::vector<index_t>& data) : begin_(data.data()), end_(data.data() + data.size()) {}

  index_view(std::vector<index_t>&& data) : data_{std::move(data)}, begin_(data_->data()), end_(data_->data() + data_->size()) {}

  template <typename Rng, typename = meta::requires<range::Range<Rng>>> index_view(const Rng& rng)
  {
    using iterator_type = range::iterator_t<Rng>;
    if constexpr (std::is_same_v<iterator_type, index_t*> || std::is_same_v<iterator_type, const index_t*>) {
      begin_ = range::begin(rng);
      end_ = range::end(rng);
    } else {
      static_assert(std::is_constructible_v<index_t, range::reference_t<iterator_type>>);
      data_ = const_vector<index_t>(range::begin(rng), range::end(rng));
      begin_ = data_->data();
      end_ = data_->data() + data_->size();
    }
  }

  auto begin() const -> const_iterator { return begin_; }
  auto end() const -> const_iterator { return end_; }

  auto cbegin() const -> const_iterator { return begin_; }
  auto cend() const -> const_iterator { return end_; }

  auto size() const -> size_type { return end_ - begin_; }

  auto operator[](index_t i) const -> const_reference { return *(begin_ + i); }

private:
  std::optional<const_vector<index_t>> data_;
  const index_t* begin_;
  const index_t* end_;
};

} // namespace jules

#endif // JULES_BASE_INDEX_VIEW_H
