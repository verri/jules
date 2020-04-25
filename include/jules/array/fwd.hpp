// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

// For by grace you have been saved through faith. And this is not your own doing; it is the gift of God,
//   not a result of works, so that no one may boast.
// Ephesians 2:8,9 (ESV)

#ifndef JULES_ARRAY_FWD_H
#define JULES_ARRAY_FWD_H

#include <cstddef>

namespace jules
{

template <typename, std::size_t> class array;
template <typename, typename> class strided_ref_array;
template <typename, std::size_t> class ref_array;
template <typename, typename, std::size_t> class unary_expr_array;
template <typename, typename> struct left_operation;
template <typename, typename> struct right_operation;

} // namespace jules

#endif // JULES_ARRAY_FWD_H
