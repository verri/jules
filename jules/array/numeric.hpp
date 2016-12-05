#ifndef JULES_ARRAY_NUMERIC_H
#define JULES_ARRAY_NUMERIC_H

#include <jules/array/array.hpp>
#include <jules/array/detail/slicing.hpp>
#include <jules/core/type.hpp>

#include <iterator>
#include <type_traits>

namespace jules
{

static inline auto seq(index_t start, index_t stop, index_t step = 1u) -> vector<index_t>
{
  index_t size = detail::seq_size(start, stop, step);
  vector<index_t> indexes(size);

  if (start < stop) {
    for (index_t i = 0; i < size; ++i) {
      indexes[i] = start;
      start += step;
    }
  } else {
    for (index_t i = 0; i < size; ++i) {
      indexes[i] = start;
      start -= step;
    }
  }

  return indexes;
}

template <typename T, typename Rng, CONCEPT_REQUIRES_(range::Range<Rng>())>
auto to_vector(const Rng& rng) -> vector<T>
{
    vector<T> vec(range::size(rng));
    range::copy(rng, vec.begin());
    return vec;
}

template <typename Rng, typename R = range::range_value_t<Rng>, CONCEPT_REQUIRES_(range::Range<Rng>())>
auto as_vector(const Rng& rng) -> vector<R>
{
  return to_vector<R>(rng);
}

template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>>
auto as_vector(Args&&... args)
{
  using R = std::common_type_t<Args...>;
  auto values = std::array<R, sizeof...(Args)>{{ std::forward<Args>(args)...  }};
  return vector<R>(std::make_move_iterator(std::begin(values)),
                   std::make_move_iterator(std::end(values)));
}

}

#endif // JULES_ARRAY_NUMERIC_H
