// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_RANGE_H
#define JULES_CORE_RANGE_H

#include <jules/core/detail/numbers.hpp>
#include <jules/core/type.hpp>
#include <range/v3/all.hpp>

namespace jules
{
namespace ranges
{
using namespace ::ranges;
}
constexpr auto closed_indices = ::ranges::views::closed_indices;
constexpr auto indices = ::ranges::views::indices;
} // namespace jules

#endif // JULES_CORE_RANGE_H
