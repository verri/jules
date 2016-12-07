// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_IO_H
#define JULES_ARRAY_IO_H

#include <jules/array/detail/common.hpp>

#include <iosfwd>

template <typename T> std::ostream& operator<<(std::ostream& os, const jules::ref_array<T, 1>& array)
{
  os << "{ ";
  for (const auto& value : array)
    os << value << ' ';
  return os << '}';
}

template <typename T, std::size_t N> std::ostream& operator<<(std::ostream& os, const jules::ref_array<T, N>& array)
{
  os << "{ ";
  for (auto i = 0u; i < array.row_count(); ++i)
    os << array[i] << ' ';
  return os << '}';
}

#endif // JULES_ARRAY_IO_H
