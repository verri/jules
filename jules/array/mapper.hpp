// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_MAPPER_H
#define JULES_ARRAY_MAPPER_H

#include <jules/base/const_vector.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

namespace jules
{

struct identity_mapper {
  constexpr auto operator()(index_t index) const noexcept { return index; }
  auto operator()(const_vector<index_t> indexes) const noexcept { return indexes; }
};

class vector_mapper
{
  vector_mapper(const_vector<index_t> indexes) : indexes_{std::move(indexes)} {}

  auto operator()(index_t index) const
  {
    DEBUG_ASSERT(index < indexes_.size(), debug::default_module, debug::level::boundary_check, "out of range");
    return indexes_[index];
  }

  auto operator()(const const_vector<index_t>& indexes) const -> const_vector<index_t>
  {
    auto mapped = typename const_vector<index_t>::container_type{};
    mapped.reserve(indexes.size());
    for (const auto index : indexes)
      mapped.push_back((*this)(index));
    return mapped;
  }

private:
  const_vector<index_t> indexes_;
};

} // namespace jules

#endif // JULES_ARRAY_MAPPER_H
