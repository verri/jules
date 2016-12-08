// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_BLAS_H
#define JULES_ARRAY_BLAS_H

#include <jules/array/array.hpp>
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

template <typename T> auto product(const matrix<T>& lhs, const matrix<T>& rhs) -> matrix<T>
{
  DEBUG_ASSERT(lhs.size() > 0 && rhs.size() > 0, debug::module{}, debug::level::invalid_argument, "empty matrix");
  DEBUG_ASSERT(lhs.column_count() == rhs.row_count(), debug::module{}, debug::level::invalid_argument, "invalid extents");

  matrix<T> result(lhs.row_count(), rhs.column_count());
  auto k = lhs.column_count(); // same as rhs.row_count()

  cblas<T>::gemm(CblasColMajor, CblasNoTrans, CblasNoTrans,         //
                 result.row_count(), result.column_count(), k, 1.0, //
                 lhs.data(), lhs.row_count(),                       //
                 rhs.data(), rhs.row_count(),                       //
                 0.0, result.data(), result.row_count());

  return result;
}

} // namespace blas
} // namespace jules

#endif // JULES_ARRAY_BLAS_H
