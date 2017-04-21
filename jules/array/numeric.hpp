#ifndef JULES_ARRAY_NUMERIC_H
#define JULES_ARRAY_NUMERIC_H

#include <jules/array/array.hpp>
#include <jules/base/const_vector.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/type.hpp>

#include <iterator>
#include <type_traits>
#include <vector>

namespace jules
{

static inline auto seq(const index_t start, const index_t stop, const distance_t step = 1) -> const_vector<index_t>
{
  DEBUG_ASSERT(step != 0, debug::default_module, debug::level::invalid_argument, "step cannot be zero");
  DEBUG_ASSERT(start <= stop || step < 0, debug::default_module, debug::level::invalid_argument, "stop value is unreachable");
  DEBUG_ASSERT(start >= stop || step > 0, debug::default_module, debug::level::invalid_argument, "stop value is unreachable");

  const auto size = static_cast<index_t>((static_cast<distance_t>(stop) - static_cast<distance_t>(start)) / step + 1);

  std::vector<index_t> indexes;
  indexes.reserve(size);

  for (auto i = start; indexes.size() < size; i += step)
    indexes.push_back(i);

  return {std::move(indexes)};
}

template <typename T, typename A> auto to_vector(A&& array) -> meta::requires_t<vector<T>, Array<std::decay_t<A>>>
{
  // TODO: Optimize if A is a temporary.
  return {array.begin(), array.end()};
}

template <typename T, typename Rng, typename = meta::requires<range::Range<std::decay_t<Rng>>>>
auto to_vector(Rng&& rng) -> meta::fallback_t<vector<T>, Array<std::decay_t<Rng>>>
{
  return {std::forward<Rng>(rng)};
}

// If higher order, copy elements column-wise.
template <typename A> auto as_vector(A&& array) -> meta::requires_t<vector<typename A::value_type>, Array<std::decay_t<A>>>
{
  // TODO: Optimize if A is a temporary.
  return {array.begin(), array.end()};
}

template <typename Rng, typename R = range::range_value_t<std::decay_t<Rng>>,
          typename = meta::requires<range::Range<std::decay_t<Rng>>>>
auto as_vector(Rng&& rng) -> meta::fallback_t<vector<R>, Array<std::decay_t<Rng>>>
{
  return to_vector<R>(std::forward<Rng>(rng));
}

template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>> auto as_vector(Args&&... args)
{
  using R = std::common_type_t<Args...>;
  auto values = std::array<R, sizeof...(Args)>{{std::forward<Args>(args)...}};
  return vector<R>(std::make_move_iterator(std::begin(values)), std::make_move_iterator(std::end(values)));
}

} // namespace jules

#endif // JULES_ARRAY_NUMERIC_H
