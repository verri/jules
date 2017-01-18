---
---

# Header file `array/array.hpp`<a id="array/array.hpp"></a>

``` cpp
namespace jules
{
    template <typename T, std::size_t N>
    class base_array;
    
    template <typename T>
    class base_array<T, 1>;
    
    //=== Array Aliasing ===//
    template <std::size_t N, typename T = jules::numeric>
    using ndarray = base_array<T, N>;
    template <typename T = jules::numeric>
    using matrix = base_array<T, 2>;
    template <typename T = jules::numeric>
    using vector = base_array<T, 1>;
}
```

## Class template `jules::base_array` \[Array Types\]<a id="jules::base_array-T,N-"></a>

``` cpp
template <typename T, std::size_t N>
class base_array
: public ref_array<T, N>
{
public:
    //=== Class Types and Constants ===//
    static constexpr auto order = N;
    using value_type = T;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using size_type = index_t;
    using difference_type = distance_t;
    
    //=== Destructor ===//
    ~base_array();
    
    //=== Constructors ===//
    base_array();
    template <typename ... Dims>
    base_array(Dims... dims);
    template <typename ... Dims>
    base_array(const value_type& value, Dims... dims);
    template <typename Iter, typename ... Dims>
    base_array(Iter iter, Dims... dims);
    base_array(recursive_initializer_list_t<value_type, order> values);
    base_array(const base_array& source);
    base_array(base_array&& source) noexcept;
    template <typename A>
    base_array(const A& source);
    
    //=== Assignment ===//
    base_array& operator=(const base_array& source);
    base_array& operator=(base_array&& source) noexcept;
    template <typename A>
    base_array& operator=(const A& source);
    
    auto fill(const value_type& value);
    template <typename ... Args>
    auto fill(in_place_t, Args&&... args);
    
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

N-Dimensional concrete array.

Basic data-type to represent arrays with 2 or more dimensions. (1-D array is specialized, see [jules::base\_array\<T, 1\>](doc_array__array.html#jules::base_array-T,1-).)

Most of the operations are implemented in its base class, consult [jules::ref\_array\<T, N\>](doc_array__ref_array.html#jules::ref_array-T,N-).

*Notes*: It is recommended to use the alias [jules::matrix](doc_array__array.html#jules::ndarray-N,T-) for 2-dimensional structures and [jules::ndarray](doc_array__array.html#jules::ndarray-N,T-) for higher dimensions.

*Notes*: Elements are contiguously stored in column-major order.

### Class Types and Constants<a id="jules::base_array-T,N-::order"></a>

``` cpp
(1)  static constexpr auto order = N;

(2)  using value_type = T;

(3)  using iterator = value_type*;

(4)  using const_iterator = const value_type*;

(5)  using size_type = index_t;

(6)  using difference_type = distance_t;
```

(1) The order of dimensionality.

(2) The type of the elements.

(3) `RandomAccessIterator` over contiguous elements.

(4) Constant `RandomAccessIterator` over contiguous elements.

(5) Unsigned integer type that can store the dimensions of the array.

(6) Signed integer type that can store differences between sizes.

### Destructor<a id="jules::base_array-T,N-::~base_array()"></a>

``` cpp
(1)  ~base_array();
```

### Constructors<a id="jules::base_array-T,N-::base_array()"></a>

``` cpp
(1)  base_array();

(2)  template <typename ... Dims>
     base_array(Dims... dims);

(3)  template <typename ... Dims>
     base_array(const value_type& value, Dims... dims);

(4)  template <typename Iter, typename ... Dims>
     base_array(Iter iter, Dims... dims);

(5)  base_array(recursive_initializer_list_t<value_type, order> values);

(6)  base_array(const base_array& source);

(7)  base_array(base_array&& source) noexcept;

(8)  template <typename A>
     base_array(const A& source);
```

Constructs a new array from a variety of data sources.

(1) Default constructor. Constructs an empty array.

(2) Explicit constructor with dimensions `dims`.  Elements are default initialized.

(3) Constructs the array with copies of elements with value `value` and dimensions `dims`.

(4) Constructs the array with dimensions `dims`.  Elements are initialized by iterating `iter`.

(5) Constructs the array from a `recursive_initialized_list`.

(6) Copy constructor.

(7) Move constructor.

(8) Converting constructor.  Constructs a array from other array-like structures, e.g. expression arrays or sliced arrays.

*Requires*: `Iter` shall be `OutputIterator`

*Requires*: `A` shall be `Array`

*Requires*: `sizeof...(Dims)` shall be `order`

*Notes*: (5) Dimensions are inferred from the list, so it must have the same number of elements in each dimension.

### Assignment<a id="jules::base_array-T,N-::operator=(constbase_array&)"></a>

``` cpp
(1)  base_array& operator=(const base_array& source);

(2)  base_array& operator=(base_array&& source) noexcept;

(3)  template <typename A>
     base_array& operator=(const A& source);
```

(1) Copy assignment.

(2) Move assignment.

(3) Assignment from array-like structures.

*Requires*: `A` shall be `Array`

### Function `jules::base_array::fill`<a id="jules::base_array-T,N-::fill(constvalue_type&)"></a>

``` cpp
(1)  auto fill(const value_type& value);

(2)  template <typename ... Args>
     auto fill(in_place_t, Args&&... args);
```

Fills the array.

### Function `jules::base_array::begin`<a id="jules::base_array-T,N-::begin()"></a>

``` cpp
(1)  iterator begin();

(2)  const_iterator begin() const;

(3)  const_iterator cbegin() const;
```

Returns an iterator to the first element of the array.

### Function `jules::base_array::end`<a id="jules::base_array-T,N-::end()"></a>

``` cpp
(1)  iterator end();

(2)  const_iterator end() const;

(3)  const_iterator cend() const;
```

Returns an iterator to the element past the last element of the container.

### Function `jules::base_array::data`<a id="jules::base_array-T,N-::data()"></a>

``` cpp
(1)  value_type* data();

(2)  const value_type* data() const;
```

Returns pointer to the underlying array serving as element storage.

-----

## Class template `jules::base_array<T, 1>` \[Array Types\]<a id="jules::base_array-T,1-"></a>

``` cpp
template <typename T>
class base_array<T, 1>
: public ref_array<T, 1>
{
public:
    //=== Class Types and Constants ===//
    static constexpr auto order = 1ul;
    using value_type = T;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using size_type = index_t;
    using difference_type = distance_t;
    
    ~base_array();
    
    //=== Constructors ===//
    base_array();
    base_array(index_t length);
    base_array(const value_type& value, index_t length);
    template <typename Iter, typename Sent>
    base_array(Iter first, Sent last);
    base_array(std::initializer_list<value_type> values);
    template <typename Rng>
    base_array(Rng&& rng);
    base_array(const base_array& source);
    base_array(base_array&& source) noexcept;
    template <typename A>
    base_array(const A& source);
    
    //=== Assignment ===//
    base_array& operator=(const base_array& source) &;
    base_array& operator=(base_array&& source) & noexcept;
    template <typename A>
    base_array& operator=(const A& source) &;
    
    auto fill(const value_type& value);
    template <typename ... Args>
    auto fill(in_place_t, Args&&... args);
    
    ind_array<value_type, 1> operator()(std::vector<index_t>&& indexes);
    ind_array<const value_type, 1> operator()(std::vector<index_t>&& indexes) const;
    
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

Basic data-type to represent arrays with 1 dimension. For more operations over this class, consult the base class [jules::ref\_array\<T, 1\>](doc_array__ref_array.html#jules::ref_array-T,1-).

*Notes*: Using the aliasing [jules::vector](doc_array__array.html#jules::ndarray-N,T-) is recommended.

*Notes*: Elements are contiguously stored.

### Class Types and Constants<a id="jules::base_array-T,1-::order"></a>

``` cpp
(1)  static constexpr auto order = 1ul;

(2)  using value_type = T;

(3)  using iterator = value_type*;

(4)  using const_iterator = const value_type*;

(5)  using size_type = index_t;

(6)  using difference_type = distance_t;
```

(1) The order of dimensionality.

(2) The type of the elements.

(3) `RandomAccessIterator` over contiguous elements.

(4) Constant `RandomAccessIterator` over contiguous elements.

(5) Unsigned integer type that can store the dimensions of the array.

(6) Signed integer type that can store differences between sizes.

### Constructors<a id="jules::base_array-T,1-::base_array()"></a>

``` cpp
(1)  base_array();

(2)  base_array(index_t length);

(3)  base_array(const value_type& value, index_t length);

(4)  template <typename Iter, typename Sent>
     base_array(Iter first, Sent last);

(5)  base_array(std::initializer_list<value_type> values);

(6)  template <typename Rng>
     base_array(Rng&& rng);

(7)  base_array(const base_array& source);

(8)  base_array(base_array&& source) noexcept;

(9)  template <typename A>
     base_array(const A& source);
```

Constructs a new vector from a variety of data sources.

(1) Default constructor. Constructs an empty vector.

(2) Explicit constructor with size `length`.  Elements are default initialized.

(3) Constructs the vector with copies of elements with value `value` and length `length`.

(4) Constructs the container with the contents of the range \[`first`, `last`).

(5) Constructs the vector from a `initialized_list`.

(6) Constructs the vector from a range.

(7) Copy constructor.

(8) Move constructor.

(9) Converting constructor.  Constructs a vector from other vector-like structures, e.g. expression arrays or sliced arrays.

*Requires*: `Sent` shall be `Sentinel<Iter>`

*Requires*: `Rng` shall be `Range` but not `Array`

*Requires*: `A` shall be `Array`

### Assignment<a id="jules::base_array-T,1-::operator=(constbase_array&)&"></a>

``` cpp
(1)  base_array& operator=(const base_array& source) &;

(2)  base_array& operator=(base_array&& source) & noexcept;

(3)  template <typename A>
     base_array& operator=(const A& source) &;
```

(1) Copy assignment.

(2) Move assignment.

(3) Assignment from array-like structures.

*Requires*: `A` shall be `Array`

### Function `jules::base_array<T, 1>::fill`<a id="jules::base_array-T,1-::fill(constvalue_type&)"></a>

``` cpp
(1)  auto fill(const value_type& value);

(2)  template <typename ... Args>
     auto fill(in_place_t, Args&&... args);
```

Fills the array.

### Function call operator `jules::base_array<T, 1>::operator()`<a id="jules::base_array-T,1-::operator()(std::vector-index_t-&&)"></a>

``` cpp
(1)  ind_array<value_type, 1> operator()(std::vector<index_t>&& indexes);

(2)  ind_array<const value_type, 1> operator()(std::vector<index_t>&& indexes) const;
```

Optimized indirect indexing from temporary indexes.

### Function `jules::base_array<T, 1>::begin`<a id="jules::base_array-T,1-::begin()"></a>

``` cpp
(1)  iterator begin();

(2)  const_iterator begin() const;

(3)  const_iterator cbegin() const;
```

Returns an iterator to the first element of the array.

### Function `jules::base_array<T, 1>::end`<a id="jules::base_array-T,1-::end()"></a>

``` cpp
(1)  iterator end();

(2)  const_iterator end() const;

(3)  const_iterator cend() const;
```

Returns an iterator to the element past the last element of the container.

### Function `jules::base_array<T, 1>::data`<a id="jules::base_array-T,1-::data()"></a>

``` cpp
(1)  value_type* data();

(2)  const value_type* data() const;
```

Returns pointer to the underlying array serving as element storage.

-----

## Array Aliasing<a id="jules::ndarray-N,T-"></a>

``` cpp
(1)  template <std::size_t N, typename T = jules::numeric>
     using ndarray = base_array<T, N>;

(2)  template <typename T = jules::numeric>
     using matrix = base_array<T, 2>;

(3)  template <typename T = jules::numeric>
     using vector = base_array<T, 1>;
```

(1) N-Dimensional array aliasing.

(2) Matrix aliasing.

(3) Vector aliasing.

-----
