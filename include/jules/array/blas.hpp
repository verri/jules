// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#if __has_include(<cblas.h>)
#ifndef JULES_ARRAY_BLAS_H
#define JULES_ARRAY_BLAS_H

#include <jules/array/functional.hpp>
#include <jules/array/fwd.hpp>
#include <jules/core/debug.hpp>

namespace jules
{
namespace blas
{

#include <cblas.h>

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
  DEBUG_ASSERT(value <= numeric_traits<int>::max(), debug::default_module, debug::level::invalid_argument,
               "blas doesn't handle arrays this big");
  return static_cast<int>(value);
}

template <typename T, typename U, typename R = std::remove_const_t<T>>
auto product(const ref_array<T, 2u>& lhs, const ref_array<U, 2u>& rhs, R alpha = numeric_traits<R>::multiplicative_identity())
  -> meta::requires_t<array<R, 2u>, std::is_same<decltype(lhs.begin()), decltype(rhs.begin())>>
{
  DEBUG_ASSERT(lhs.size() > 0 && rhs.size() > 0, debug::default_module, debug::level::invalid_argument, "empty matrix");
  DEBUG_ASSERT(lhs.column_count() == rhs.row_count(), debug::default_module, debug::level::invalid_argument, "invalid extents");

  auto result = array<R, 2u>(uninitialized, lhs.row_count(), rhs.column_count());

  const auto k = safe_int_cast(lhs.column_count()); // same as rhs.row_count()
  const auto result_row_count = safe_int_cast(result.row_count());
  const auto result_column_count = safe_int_cast(result.column_count());
  const auto lhs_row_count = safe_int_cast(lhs.row_count());
  const auto rhs_row_count = safe_int_cast(rhs.row_count());

  cblas<R>::gemm(CblasColMajor, CblasNoTrans, CblasNoTrans,       //
                 result_row_count, result_column_count, k, alpha, //
                 lhs.begin(), lhs_row_count,                      //
                 rhs.begin(), rhs_row_count,                      //
                 0.0, result.data(), result_row_count);

  return result;
}

template <typename T, typename U, typename V, typename R = std::remove_const_t<T>,
          typename = meta::requires<std::is_same<std::remove_const_t<T>, std::remove_const_t<U>>, std::is_convertible<V, R>>>
auto product(const unary_expr_array<T*, left_operation<V, std::multiplies<>>, 2u>& lhs, const ref_array<U, 2u>& rhs)
{
  const auto new_lhs = ref_array<T, 2u>{lhs.first(), {lhs.dimensions()}};
  return product(new_lhs, rhs, lhs.op().lhs);
}

// TODO: other optimized versions.

} // namespace blas
} // namespace jules

#endif // JULES_ARRAY_BLAS_H
#endif // __has_include(<cblas.h>)
