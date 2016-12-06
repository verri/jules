---
---

# What is an array?

For `jules`, an `N`-dimensional array consists of a collection of variables of
same type that can be selected by N indices. The order of dimensionality `N` is
a compile-time constant. The size of each dimension, although fixed after
construction, is calculated at run-time.

In other words, an 1-dimensional array with 10 integer number is roughly
equivalent to `int array[10]`, while a 2-dimensional array with dimensions
5 and 2 to `int array[5][2]`. C arrays are contiguously stored in row-major
order. `jules` arrays are also stored contiguously but in column-major order at
the free store.

`jules` provides a series of convenient functions to manage arrays.

# Using arrays

The simplest way to use array facilities in `jules` is to include the whole
array module:

``` cpp
#include <jules/array/all.hpp>
```

This will expose the basic types `jules::vector<T>` for 1-dimensional arrays,
`jules::matrix<T>` for 2-dimensional arrays, and `jules::ndarray<N, T>` for
higher orders.  If the type `T` is not specified,
[jules::numeric](doc_core__type.html#jules::numeric) is used by default.

# Constructors

There are several ways to create a vector, see [this
reference](doc_array__array.html#jules::base_array-T,1-) for more details.

``` cpp

auto a = jules::vector<>(); // Empty vector.
auto b = jules::vector<>(10u); // Vector with 10 elements.
auto c = jules::vector<>(3.14, 10u); // Vector with 10 elements containing 3.14.

jules::numeric values[] = {0.1, 0.2, 0.3, 0.4, 0.5};

auto d = jules::vector<>(std::begin(values), std::end(values)); // Vector from range of iterators.
auto e = jules::vector<>{0.1, 0.2, 0.3, 0.4, 0.5}; // Vector from initializer_list.
auto f = jules::vector<>(values); // Vector from `Range`.
auto g = a; // Copy constructor.
auto h = std::move(b); // Move constructor.
auto i = jules::vector<>(d + e); // Vector from other vector-like structures. See section TODO.
```
