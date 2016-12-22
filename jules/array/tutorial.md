# What is an array?

In `jules`, an `N`-dimensional array consists of a collection of variables of
same type that can be selected by N indices. The order of dimensionality `N` is
a compile-time constant. The size of each dimension, although fixed after
construction, is calculated at run-time.

In other words, an 1-dimensional array with 10 integer numbers is roughly
equivalent to `int array[10]`, while a 2-dimensional array with dimensions
5 and 2 to `int array[5][2]`. C arrays are contiguously stored in row-major
order. `jules` arrays are also stored contiguously but in column-major order at
the free store.

`jules` provides a series of convenient functions to manage arrays.

# Using arrays

The simplest way to use all array facilities in `jules` is including the whole
array module:

``` cpp
#include <jules/array/all.hpp>
```

This will expose the basic types `jules::vector<T>` for 1-dimensional arrays,
`jules::matrix<T>` for 2-dimensional arrays, and `jules::ndarray<N, T>` for
higher orders.  If the type `T` is not specified,
[jules::numeric](standardese://jules::numeric/) is used by default.

# Constructors

There are several ways to create a vector, see [the
reference](standardese://jules::base_array<T, 1>::base_array/) for more details.

``` cpp
// An empty vector.
auto a = jules::vector<>();

// A vector with 10 elements.
// If elements types are trivial, they are non-initialized.
// Otherwise, they are default initialized.
auto b = jules::vector<>(10u);

// A vector with 10 elements containing copies of 3.14.
auto c = jules::vector<>(3.14, 10u);

auto values[] = {0.1, 0.2, 0.3, 0.4, 0.5};

// A vector from range of iterators.
auto d = jules::vector<>(std::begin(values), std::end(values));

// A vector from initializer_list.
auto e = jules::vector<>{0.1, 0.2, 0.3, 0.4, 0.5};

// A vector from an object that satifies the `Range` concept.
auto f = jules::vector<>(values);

// Copy and move constructor.
auto g = a;
auto h = std::move(b);

// A vector from other vector-like structures.
// See section Array-like structures for more information.
auto i = jules::vector<>(d + e);
```

The function `jules::as_vector` can be used to construct a vector whose
elements type is inferred.

``` cpp
// An vector with elements whose type is inferred as double.
auto j = jules::as_vector(values);

// An vector with elements whose type is the common type of the arguments.
auto k = jules::as_vector(1, 2u, 3l);
"")
```

For higher dimensional arrays, the constructor are basically the same.  Consult
[the reference](standardese://jules::base_array<T, N>::base_array/) for
a comprehensive list.

``` cpp

auto a = jules::matrix<>(); // Empty matrix.
auto b = jules::matrix<>(5u, 2u); // 5x2 matrix.
auto c = jules::matrix<>(3.14, 5u, 2u); // 5x2 matrix with elements 3.14.

jules::numeric values[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

// 3x3 matrix with elements from an iterator.
// THe matrix is filled columns-first.
auto d = jules::matrix<>(std::begin(values), 3u, 3u);

// 3x3 matrix from a recursive initializer_list.
auto e = jules::matrix<>{
  {0.1, 0.4, 0.7},
  {0.2, 0.5, 0.8},
  {0.3, 0.6, 0.9},
};

auto f = a; // Copy constructor.
auto g = std::move(b); // Move constructor.
auto i = jules::matrix<>(d + e); // Matrix from other matrix-like structures. See section TODO.

```
