// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DETAIL_MAPPER_H
#define JULES_ARRAY_DETAIL_MAPPER_H

#include <jules/base/const_vector.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules::detail
{

struct identity_mapper {
  constexpr auto map(index_t index) const noexcept { return index; }
  auto map(const_vector<index_t> indexes) const noexcept { return indexes; }
};

class vector_mapper
{
  vector_mapper(const_vector<index_t> indexes) : indexes_{std::move(indexes)} {}

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

private:
  const_vector<index_t> indexes_;
};

} // namespace jules::detail

#endif // JULES_ARRAY_DETAIL_MAPPER_H
