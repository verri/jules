// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

#if __has_include(<cblas.h>)
#ifndef JULES_ARRAY_BLAS_H
#define JULES_ARRAY_BLAS_H

#include <jules/array/functional.hpp>
#include <jules/array/fwd.hpp>
#include <jules/core/debug.hpp>

extern "C" {
#include <cblas.h>
}

namespace jules::blas
{

template <typename T> struct invalid : public std::false_type
{};

template <typename T> struct cblas
{
  static_assert(invalid<T>::value, "the requested cblas method is not implemented for this data type");
};

template <> struct cblas<float>
{
  static constexpr auto gemm = &cblas_sgemm;
};

template <> struct cblas<double>
{
  static constexpr auto gemm = &cblas_dgemm;
};

template <typename T> static inline auto safe_int_cast(T value)
{
  DEBUG_ASSERT(value <= static_cast<T>(numeric_traits<int>::max()), debug::default_module, debug::level::invalid_argument,
               "blas doesn't handle arrays this big");
  return static_cast<int>(value);
}

template <typename T, typename U, typename R = std::remove_const_t<T>>
auto product(const ref_array<T, 2u>& lhs, const ref_array<U, 2u>& rhs, R alpha = numeric_traits<R>::multiplicative_identity())
  -> array<R, 2u>
{
  static_assert(std::is_same_v<decltype(lhs.begin()), decltype(rhs.begin())>);

  DEBUG_ASSERT(lhs.size() > 0 && rhs.size() > 0, debug::default_module, debug::level::invalid_argument, "empty matrix");
  DEBUG_ASSERT(column_count(lhs) == row_count(rhs), debug::default_module, debug::level::invalid_argument, "invalid extents");

  auto result = array<R, 2u>(uninitialized, row_count(lhs), column_count(rhs));

  const auto k = safe_int_cast(column_count(lhs)); // same as row_count(rhs)
  const auto result_row_count = safe_int_cast(row_count(result));
  const auto result_column_count = safe_int_cast(column_count(result));
  const auto lhs_row_count = safe_int_cast(row_count(lhs));
  const auto rhs_row_count = safe_int_cast(row_count(rhs));

  cblas<R>::gemm(CblasColMajor, CblasNoTrans, CblasNoTrans,       //
                 result_row_count, result_column_count, k, alpha, //
                 lhs.begin(), lhs_row_count,                      //
                 rhs.begin(), rhs_row_count,                      //
                 0.0, result.data(), result_row_count);

  return result;
}

template <typename T, typename U, typename V, typename R = std::remove_const_t<T>>
requires same_as<std::remove_const_t<T>, std::remove_const_t<U>>&& convertible_to<V, R> auto
product(const unary_expr_array<T*, left_operation<V, std::multiplies<>>, 2u>& lhs, const ref_array<U, 2u>& rhs)
{
  const auto new_lhs = ref_array<T, 2u>{lhs.first(), {lhs.dimensions()}};
  return product(new_lhs, rhs, lhs.op().lhs);
}

// TODO: other optimized versions.

} // namespace jules::blas

#endif // JULES_ARRAY_BLAS_H
#endif // __has_include(<cblas.h>)
