#ifndef JULES_ARRAY_NUMERIC_H
#define JULES_ARRAY_NUMERIC_H

#include <jules/array/array.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/type.hpp>

#include <iterator>
#include <type_traits>
#include <vector>

namespace jules
{

static inline auto seq(const index_t start, const index_t stop, const distance_t step = 1) -> std::vector<index_t>
{
  DEBUG_ASSERT(step != 0, debug::module{}, debug::level::invalid_argument, "step cannot be zero");
  DEBUG_ASSERT(start <= stop || step < 0, debug::module{}, debug::level::invalid_argument, "stop value is unreachable");
  DEBUG_ASSERT(start >= stop || step > 0, debug::module{}, debug::level::invalid_argument, "stop value is unreachable");

  const auto size = static_cast<index_t>((static_cast<distance_t>(stop) - static_cast<distance_t>(start)) / step + 1);

  std::vector<index_t> indexes;
  indexes.reserve(size);

  for (auto i = start; indexes.size() < size; i += step)
    indexes.push_back(i);

  return indexes;
}

template <typename T, typename Rng, typename = meta::requires<range::Range<std::decay_t<Rng>>>>
auto to_vector(Rng&& rng) -> array_fallback<vector<T>, std::decay_t<Rng>>
{
  return vector<T>(std::forward<Rng>(rng));
}

template <typename T, typename Array> auto to_vector(Array&& array) -> array_request<vector<T>, std::decay_t<Array>>
{
  // TODO: XXX: as_vector(Array) and to_vector(Array) have mismatching behavior.
  return {std::forward<Array>(array)};
}

template <typename Rng, typename R = range::range_value_t<std::decay_t<Rng>>,
          typename = meta::requires<range::Range<std::decay_t<Rng>>>>
auto as_vector(Rng&& rng) -> array_fallback<vector<R>, std::decay_t<Rng>>
{
  return to_vector<R>(std::forward<Rng>(rng));
}

// If higher order, copy elements column-wise.
template <typename Array> auto as_vector(Array&& array) -> array_request<vector<typename Array::value_type>, std::decay_t<Array>>
{
  return {array.begin(), array.end()};
}

template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>> auto as_vector(Args&&... args)
{
  using R = std::common_type_t<Args...>;
  auto values = std::array<R, sizeof...(Args)>{{std::forward<Args>(args)...}};
  return vector<R>(std::make_move_iterator(std::begin(values)), std::make_move_iterator(std::end(values)));
}

template <typename Array, typename T = typename Array::value_type, typename = array_request<void, Array>>
auto normal_pdf(const Array& array, T mu, T sigma)
{
  return apply(array, [ mu = std::move(mu), sigma = std::move(sigma) ](const auto& x) { return normal_pdf(x, mu, sigma); });
}

} // namespace jules

#endif // JULES_ARRAY_NUMERIC_H
