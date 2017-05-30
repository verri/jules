// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#if __has_include(<cblas.h>)
#ifndef JULES_ARRAY_BLAS_H
#define JULES_ARRAY_BLAS_H

#include <jules/array/detail/common.hpp>
#include <jules/core/debug.hpp>

namespace jules
{
namespace blas
{

#include <cblas.h>

template <typename T> struct invalid : public std::false_type {
};

template <typename T> struct cblas {
  static_assert(invalid<T>::value, "the requested cblas method is not implemented for this data type");
};

template <> struct cblas<float> {
  static constexpr auto gemm = &cblas_sgemm;
};

template <> struct cblas<double> {
  static constexpr auto gemm = &cblas_dgemm;
};

template <typename T> static inline auto safe_int_cast(T value)
{
  DEBUG_ASSERT(value <= numeric_traits<int>::max(), debug::default_module, debug::level::invalid_argument,
               "blas doesn't handle arrays this big");
  return static_cast<int>(value);
}

template <typename T> auto product(const array<T, 2u>& lhs, const array<T, 2u>& rhs) -> array<T, 2u>
{
  DEBUG_ASSERT(lhs.size() > 0 && rhs.size() > 0, debug::default_module, debug::level::invalid_argument, "empty matrix");
  DEBUG_ASSERT(lhs.column_count() == rhs.row_count(), debug::default_module, debug::level::invalid_argument, "invalid extents");

  auto result = array<T, 2u>(lhs.row_count(), rhs.column_count());

  const auto k = safe_int_cast(lhs.column_count()); // same as rhs.row_count()
  const auto result_row_count = safe_int_cast(result.row_count());
  const auto result_column_count = safe_int_cast(result.column_count());
  const auto lhs_row_count = safe_int_cast(lhs.row_count());
  const auto rhs_row_count = safe_int_cast(rhs.row_count());

  cblas<T>::gemm(CblasColMajor, CblasNoTrans, CblasNoTrans,     //
                 result_row_count, result_column_count, k, 1.0, //
                 lhs.data(), lhs_row_count,                     //
                 rhs.data(), rhs_row_count,                     //
                 0.0, result.data(), result_row_count);

  return result;
}

} // namespace blas
} // namespace jules

#endif // JULES_ARRAY_BLAS_H
#endif // __has_include(<cblas.h>)
