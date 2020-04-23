// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_COMMON_H
/// \exclude
#define JULES_ARRAY_META_COMMON_H

#include <jules/core/concepts.hpp>
#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>

namespace jules
{

// clang-format off
template <std::size_t N, typename... Dims>
concept valid_extents_for = (sizeof...(Dims) == N) && (convertible_to<Dims, index_t> && ...);

template <typename Array> struct array_traits {
  using eval_type = void;
  using concrete_type = void;
};

template <typename Array> using array_eval_t = typename array_traits<Array>::eval_type;
template <typename Array> using array_concrete_t = typename array_traits<Array>::concrete_type;

template <typename T, typename Array, typename Result> concept eval_for = requires(const T& eval, const Array& a) {
  { eval(a) } -> convertible_to<Result>;
};

template <typename T> concept common_array = requires(const T& t)
{
  ranges::range<T>;

  typename T::value_type;
  typename T::size_type;

  !same_as<void, array_eval_t<T>>;
  !same_as<void, array_concrete_t<T>>;

  eval_for<array_eval_t<T>, T, array_concrete_t<T>>;

  T::order > 0u;

  { T::order } -> same_as<const std::size_t>;

  { t.size() } -> same_as<typename T::size_type>;
  { t.dimensions() } -> same_as<std::array<typename T::size_type, T::order>>;
};
// clang-format on

} // namespace jules

#endif // JULES_ARRAY_META_COMMON_H
