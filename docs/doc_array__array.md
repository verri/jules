---
---

# Header file `array/array.hpp`<a id="array/array.hpp"></a>

``` cpp
#define JULES_ARRAY_ARRAY_H

namespace jules
{
    template <typename T, std::size_t N>
    class base_array;
    
    template <typename T>
    class base_array<T, 1>;
    
    template <std::size_t N, typename T = jules::numeric>
    using ndarray = base_array<T, N>;
    
    template <typename T = jules::numeric>
    using matrix = base_array<T, 2>;
    
    template <typename T = jules::numeric>
    using vector = base_array<T, 1>;
}
```

## Class template `jules::base_array` \[N-Dimensional Array\]<a id="jules::base_array-T,N-"></a>

``` cpp
template <typename T, std::size_t N>
class base_array
: public ref_array<T, N>
{
public:
    using value_type = T;
    
    static constexpr auto order = N;
    
    using iterator = T*;
    
    using const_iterator = const T*;
    
    using size_type = index_t;
    
    using difference_type = distance_t;
    
    ~base_array();
    
    base_array();
    template <typename ... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
    base_array(Dims... dims);
    template <typename ... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
    base_array(const T& value, Dims... dims);
    template <typename Iter, typename ... Dims, typename R = range::iterator_value_t<Iter>, typename = detail::n_indexes_enabler<N, Dims...>>
    base_array(Iter iter, Dims... dims);
    base_array(recursive_initializer_list_t<T, N> values);
    base_array(const base_array& source);
    base_array(base_array&& source) noexcept;
    template <typename Array, typename = array_request<void, Array>>
    base_array(const Array& source);
    
    base_array& operator=(const base_array& source);
    base_array& operator=(base_array&& source) noexcept;
    template <typename Array>
    array_request<base_array&, Array> operator=(const Array& source);
    
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    
    value_type* data();
    const value_type* data() const;
    
private:
    static base_slice<N> calculate_descriptor(recursive_initializer_list_t<T, N> values);
    
    static auto calculate_descriptor_fill(std::array<index_t, N>&, const T&, index_t);
    
    static constexpr auto calculate_descriptor_check(const std::array<index_t, N>&, const T&, index_t);
};
```

N-Dimensional concrete array.

Basic data-type to represent arrays with 2 or more dimensions. For more operations over this class, consult [jules::ref\_array\<T, N\>](doc_array__ref_array.html#jules::ref_array-T,N-).

*Notes*: Using the aliasing [jules::matrix](doc_array__array.html#jules::matrix-T-) for 2D structures and [jules::ndarray](doc_array__array.html#jules::ndarray-N,T-) for higher dimensions is recommended to keep portability between versions.

*Notes*: Elements are contiguously stored in column-major order.

### Type alias `jules::base_array::value_type`<a id="jules::base_array-T,N-::value_type"></a>

``` cpp
using value_type = T;
```

The type of the elements.

### Variable `jules::base_array::order`<a id="jules::base_array-T,N-::order"></a>

``` cpp
static constexpr auto order = N;
```

The order of dimensionality.

### Type alias `jules::base_array::iterator`<a id="jules::base_array-T,N-::iterator"></a>

``` cpp
using iterator = T*;
```

`RandomAccessIterator` over contiguous elements.

### Type alias `jules::base_array::const_iterator`<a id="jules::base_array-T,N-::const_iterator"></a>

``` cpp
using const_iterator = const T*;
```

Constant `RandomAccessIterator` over contiguous elements.

### Type alias `jules::base_array::size_type`<a id="jules::base_array-T,N-::size_type"></a>

``` cpp
using size_type = index_t;
```

Unsigned integer type that can store the dimensions of the array.

### Type alias `jules::base_array::difference_type`<a id="jules::base_array-T,N-::difference_type"></a>

``` cpp
using difference_type = distance_t;
```

Signed integer type that can store differences between sizes.

### Default constructor `jules::base_array::base_array`<a id="jules::base_array-T,N-"></a>

``` cpp
(1)  base_array();

(2)  template <typename ... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
     base_array(Dims... dims);

(3)  template <typename ... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
     base_array(const T& value, Dims... dims);

(4)  template <typename Iter, typename ... Dims, typename R = range::iterator_value_t<Iter>, typename = detail::n_indexes_enabler<N, Dims...>>
     base_array(Iter iter, Dims... dims);

(5)  base_array(recursive_initializer_list_t<T, N> values);

(6)  base_array(const base_array& source);

(7)  base_array(base_array&& source) noexcept;

(8)  template <typename Array, typename = array_request<void, Array>>
     base_array(const Array& source);
```

Constructs a new array from a variety of data sources.

1)  Default constructor. Constructs an empty array.

2)  Explicit constructor with dimensions `dims`.  Elements are default initialized.

3)  Constructs the array with copies of elements with value `value` and dimensions `dims`.

4)  Constructs the array with dimensions `dims`.  Elements are initialized with elements from iterating `iter`.

5)  Constructs the array from a `recursive_initialized_list`.

6)  Copy constructor.

7)  Move constructor.

8)  Converting constructor.  Constructs a array from other array-like structures, e.g. expression arrays or sliced arrays.

*Notes*: (2-4) For constructors that receive the array dimensions, the number of arguments must be exactly the dimensionality [jules::base\_array\<T, N\>::order](doc_array__array.html#jules::base_array-T,N-::order).

*Notes*: (5) Dimensions are inferred from the list, so it must have the same number of elements in each dimension.

### Assignment operator `jules::base_array::operator=`<a id="jules::base_array-T,N-"></a>

``` cpp
(1)  base_array& operator=(const base_array& source);

(2)  base_array& operator=(base_array&& source) noexcept;

(3)  template <typename Array>
     array_request<base_array&, Array> operator=(const Array& source);
```

1)  Copy assignment.

2)  Move assignment.

3)  Assignment from array-like structures.

### Function `jules::base_array::begin`<a id="jules::base_array-T,N-"></a>

``` cpp
(1)  iterator begin();

(2)  const_iterator begin() const;

(3)  const_iterator cbegin() const;
```

Returns an iterator to the first element of the array.

### Function `jules::base_array::end`<a id="jules::base_array-T,N-"></a>

``` cpp
(1)  iterator end();

(2)  const_iterator end() const;

(3)  const_iterator cend() const;
```

Returns an iterator to the element following the last element of the container.

### Function `jules::base_array::data`<a id="jules::base_array-T,N-"></a>

``` cpp
(1)  value_type* data();

(2)  const value_type* data() const;
```

Returns pointer to the underlying array serving as element storage.

-----

## Class template `jules::base_array<T, 1>` \[N-Dimensional Array\]<a id="jules::base_array-T,1-"></a>

``` cpp
template <typename T>
class base_array<T, 1>
: public ref_array<T, 1>
{
public:
    using value_type = T;
    
    static constexpr auto order = 1;
    
    using iterator = T*;
    
    using const_iterator = const T*;
    
    using size_type = index_t;
    
    using difference_type = distance_t;
    
    ~base_array();
    
    base_array();
    base_array(index_t length);
    base_array(const T& value, index_t length);
    template <typename Iter, typename U = range::iterator_value_t<Iter>>
    base_array(Iter first, Iter last);
    base_array(std::initializer_list<T> values);
    template <typename Rng, typename U = range::range_value_t<Rng>, int _concept_requires_319 = 42, typename std::enable_if<(_concept_requires_319==43)||(range::Range<Rng>()), int>::type=0, typename = array_fallback<void, Rng>>
    base_array(const Rng& rng);
    base_array(const base_array& source);
    base_array(base_array&& source) noexcept;
    template <typename Array, typename = array_request<void, Array>>
    base_array(const Array& source);
    
    base_array& operator=(const base_array& source);
    base_array& operator=(base_array&& source) noexcept;
    template <typename Array>
    array_request<base_array&, Array> operator=(const Array& source);
    
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    
    value_type* data();
    const value_type* data() const;
};
```

1-Dimensional concrete array.

Basic data-type to represent arrays with 1 dimension. For more operations over this class, consult [jules::ref\_array\<T, 1\>](doc_array__ref_array.html#jules::ref_array-T,1-).

*Notes*: Using the aliasing [jules::vector](doc_array__array.html#jules::vector-T-) is recommended to keep portability between versions.

*Notes*: Elements are contiguously stored.

### Type alias `jules::base_array<T, 1>::value_type`<a id="jules::base_array-T,1-::value_type"></a>

``` cpp
using value_type = T;
```

The type of the elements.

### Variable `jules::base_array<T, 1>::order`<a id="jules::base_array-T,1-::order"></a>

``` cpp
static constexpr auto order = 1;
```

The order of dimensionality.

### Type alias `jules::base_array<T, 1>::iterator`<a id="jules::base_array-T,1-::iterator"></a>

``` cpp
using iterator = T*;
```

`RandomAccessIterator` over contiguous elements.

### Type alias `jules::base_array<T, 1>::const_iterator`<a id="jules::base_array-T,1-::const_iterator"></a>

``` cpp
using const_iterator = const T*;
```

Constant `RandomAccessIterator` over contiguous elements.

### Type alias `jules::base_array<T, 1>::size_type`<a id="jules::base_array-T,1-::size_type"></a>

``` cpp
using size_type = index_t;
```

Unsigned integer type that can store the dimensions of the array.

### Type alias `jules::base_array<T, 1>::difference_type`<a id="jules::base_array-T,1-::difference_type"></a>

``` cpp
using difference_type = distance_t;
```

Signed integer type that can store differences between sizes.

### Default constructor `jules::base_array<T, 1>::base_array`<a id="jules::base_array-T,1-"></a>

``` cpp
(1)  base_array();

(2)  base_array(index_t length);

(3)  base_array(const T& value, index_t length);

(4)  template <typename Iter, typename U = range::iterator_value_t<Iter>>
     base_array(Iter first, Iter last);

(5)  base_array(std::initializer_list<T> values);

(6)  template <typename Rng, typename U = range::range_value_t<Rng>, int _concept_requires_319 = 42, typename std::enable_if<(_concept_requires_319==43)||(range::Range<Rng>()), int>::type=0, typename = array_fallback<void, Rng>>
     base_array(const Rng& rng);

(7)  base_array(const base_array& source);

(8)  base_array(base_array&& source) noexcept;

(9)  template <typename Array, typename = array_request<void, Array>>
     base_array(const Array& source);
```

Constructs a new vector from a variety of data sources.

1)  Default constructor. Constructs an empty vector.

2)  Explicit constructor with size `length`.  Elements are default initialized.

3)  Constructs the vector with copies of elements with value `value` and length `length`.

4)  Constructs the container with the contents of the range \[`first`, `last`).

5)  Constructs the vector from a `initialized_list`.

6)  Constructs the vector from a range.

7)  Copy constructor.

8)  Move constructor.

9)  Converting constructor.  Constructs a vector from other vector-like structures, e.g. expression arrays or sliced arrays.

### Copy assignment operator `jules::base_array<T, 1>::operator=`<a id="jules::base_array-T,1-"></a>

``` cpp
(1)  base_array& operator=(const base_array& source);

(2)  base_array& operator=(base_array&& source) noexcept;

(3)  template <typename Array>
     array_request<base_array&, Array> operator=(const Array& source);
```

1)  Copy assignment.

2)  Move assignment.

3)  Assignment from array-like structures.

### Function `jules::base_array<T, 1>::begin`<a id="jules::base_array-T,1-"></a>

``` cpp
(1)  iterator begin();

(2)  const_iterator begin() const;

(3)  const_iterator cbegin() const;
```

Returns an iterator to the first element of the array.

### Function `jules::base_array<T, 1>::end`<a id="jules::base_array-T,1-"></a>

``` cpp
(1)  iterator end();

(2)  const_iterator end() const;

(3)  const_iterator cend() const;
```

Returns an iterator to the element following the last element of the container.

### Function `jules::base_array<T, 1>::data`<a id="jules::base_array-T,1-"></a>

``` cpp
(1)  value_type* data();

(2)  const value_type* data() const;
```

Returns pointer to the underlying array serving as element storage.

-----

## Alias template `jules::ndarray`<a id="jules::ndarray-N,T-"></a>

``` cpp
template <std::size_t N, typename T = jules::numeric>
using ndarray = base_array<T, N>;
```

N-Dimensional array aliasing.  Data type defaults to [jules::numeric](doc_core__type.html#jules::numeric).

## Alias template `jules::matrix`<a id="jules::matrix-T-"></a>

``` cpp
template <typename T = jules::numeric>
using matrix = base_array<T, 2>;
```

Matrix aliasing.  Data type defaults to [jules::numeric](doc_core__type.html#jules::numeric).

## Alias template `jules::vector`<a id="jules::vector-T-"></a>

``` cpp
template <typename T = jules::numeric>
using vector = base_array<T, 1>;
```

Vector aliasing.  Data type defaults to [jules::numeric](doc_core__type.html#jules::numeric).

-----
