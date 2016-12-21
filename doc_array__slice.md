---
---

# Header file `array/slice.hpp`<a id="array/slice.hpp"></a>

``` cpp
#define JULES_ARRAY_SLICE_H

namespace jules
{
    template <std::size_t N>
    class base_slice;
    
    template <>
    class base_slice<1>;
    
    using slice = base_slice<1>;
}
```

## Class template `jules::base_slice` \[N-Dimensional Array\]<a id="jules::base_slice-N-"></a>

``` cpp
template <std::size_t N>
class base_slice
{
public:
    using extent_type = std::array<index_t, N>;
    
    class iterator;
    
    constexpr base_slice(index_t start, extent_type extents, extent_type strides);
    constexpr base_slice(index_t start, extent_type extents);
    constexpr base_slice() = default;
    
    constexpr base_slice(const base_slice& source) = default;
    
    constexpr base_slice(base_slice&& source) noexcept = default;
    
    constexpr base_slice& operator=(const base_slice& source) = default;
    
    constexpr base_slice& operator=(base_slice&& source) noexcept = default;
    
    constexpr auto size() const;
    
    constexpr auto dimensions() const;
    
    constexpr index_t operator()(const extent_type& indexes) const;
    template <typename ... Args, typename = detail::n_indexes_enabler<N, Args...>>
    constexpr index_t operator()(Args&&... indexes) const;
    
    constexpr auto operator==(const base_slice& other) const;
    
    constexpr auto operator!=(const base_slice& other) const;
    
    constexpr iterator begin() const;
    
    constexpr iterator end() const;
    
    constexpr iterator cbegin() const;
    
    constexpr iterator cend() const;
    
    constexpr base_slice<N-1> drop_dimension() const;
    
    index_t start = 0ul;
    
    extent_type extents = repeat<N, index_t>(0ul);
    
    extent_type strides = repeat<N, index_t>(1ul);
};
```

Array slicing and dimensions representation.

This class is used internally by `jules` to represent the position of elements of an array in memory.

*Notes*: `base_slice` is `TriviallyCopyable` and `TriviallyMovable`.

**Members:**

  - <a id="jules::base_slice-N-::start"></a>`start` - Start position.

  - <a id="jules::base_slice-N-::extents"></a>`extents` - Size in each dimension.

  - <a id="jules::base_slice-N-::strides"></a>`strides` - Skip in each dimension.

### Class `jules::base_slice::iterator`<a id="jules::base_slice-N-::iterator"></a>

``` cpp
class iterator
: public std::iterator<std::input_iterator_tag, index_t, distance_t, void *, index_t>
{
public:
    iterator() = delete;
    
    constexpr iterator(const iterator& source) = default;
    
    constexpr iterator(iterator&& source) noexcept = default;
    
    constexpr iterator& operator=(const iterator& source) = default;
    
    constexpr iterator& operator=(iterator&& source) noexcept = default;
    
    constexpr iterator& operator++();
    
    constexpr iterator operator++(int);
    
    constexpr auto operator==(const iterator& other) const;
    
    constexpr auto operator!=(const iterator& other) const;
    
    constexpr index_t operator*() const;
    
    void* operator->() const;
};
```

`InputIterator` which gives the memory positions of a slice.

*Notes*: It depends on the slice that created it, and will be invalidated if have longer lifetime than the slice.

#### Comparison operator `jules::base_slice::iterator::operator==`<a id="jules::base_slice-N-::iterator::operator==(constjules::base_slice::iterator&)const"></a>

``` cpp
constexpr auto operator==(const iterator& other) const;
```

*Notes*: It does not check if the slices are the same.

#### Comparison operator `jules::base_slice::iterator::operator!=`<a id="jules::base_slice-N-::iterator::operator!=(constjules::base_slice::iterator&)const"></a>

``` cpp
constexpr auto operator!=(const iterator& other) const;
```

*Notes*: It only checks if the slices are the same if `JULES_DEBUG_LEVEL` \>= `incompatible_comparison`.

#### Operator `jules::base_slice::iterator::operator->`<a id="jules::base_slice-N-::iterator::operator--()const"></a>

``` cpp
void* operator->() const;
```

*Notes*: You should not call this function.

-----

### Constructor `jules::base_slice::base_slice`<a id="jules::base_slice-N-"></a>

``` cpp
(1)  constexpr base_slice(index_t start, extent_type extents, extent_type strides);

(2)  constexpr base_slice(index_t start, extent_type extents);

(3)  constexpr base_slice() = default;
```

*Notes*: If `strides` are inferred, `extents` cannot be zero.

**Parameters:**

  - <a id="jules::base_slice-N-::base_slice(index_t,extent_type,extent_type)"></a>`start` - Start position of the slicing. It defaults to 0ul.

  - <a id="jules::base_slice-N-::base_slice(index_t,extent_type,extent_type).extents"></a>`extents` - Size of the slicing in each dimension. It defaults to `base_slice<N>::all`.

  - <a id="jules::base_slice-N-::base_slice(index_t,extent_type,extent_type).strides"></a>`strides` - Number of skip positions in each dimension. It defaults to consistent strides based on the `extents`.

### Function `jules::base_slice::size`<a id="jules::base_slice-N-::size()const"></a>

``` cpp
constexpr auto size() const;
```

Effectively the product of the extents.

### Function call operator `jules::base_slice::operator()`<a id="jules::base_slice-N-"></a>

``` cpp
(1)  constexpr index_t operator()(const extent_type& indexes) const;

(2)  template <typename ... Args, typename = detail::n_indexes_enabler<N, Args...>>
     constexpr index_t operator()(Args&&... indexes) const;
```

Returns the memory position of the index.

**Parameters:**

  - <a id="jules::base_slice-N-::operator()(constextent_type&)const.indexes"></a>`indexes` - Index that can be either an array or more than one argument.

-----

## Class template `jules::base_slice<1>` \[N-Dimensional Array\]<a id="jules::base_slice-1-"></a>

``` cpp
template <>
class base_slice<1>
{
public:
    using extent_type = index_t;
    
    class iterator;
    
    static constexpr auto all = index_t{0ul};
    
    constexpr base_slice(index_t start, index_t extent, index_t stride);
    constexpr base_slice(index_t start, index_t extent);
    constexpr base_slice() = default;
    
    constexpr base_slice(const base_slice& source) = default;
    
    constexpr base_slice(base_slice&& source) noexcept = default;
    
    constexpr base_slice& operator=(const base_slice& source) = default;
    
    constexpr base_slice& operator=(base_slice&& source) noexcept = default;
    
    constexpr auto size() const;
    
    constexpr auto dimensions() const;
    
    constexpr index_t operator()(index_t index) const;
    
    constexpr iterator begin() const;
    
    constexpr iterator end() const;
    
    constexpr iterator cbegin() const;
    
    constexpr iterator cend() const;
    
    index_t start = 0ul;
    
    index_t extent = all;
    
    index_t stride = 1ul;
};
```

1D-Array specialization for slicing and dimensions representation.

This specialization is useful for slicing multidimensional arrays.

*TODO*: See the alias `jules::slice` for usage more information.

*Notes*: `base_slice` is `TriviallyCopyable` and `TriviallyMovable`.

**Members:**

  - <a id="jules::base_slice-1-::start"></a>`start` - Start position.

  - <a id="jules::base_slice-1-::extent"></a>`extent` - Number of position.

  - <a id="jules::base_slice-1-::stride"></a>`stride` - Skip positions.

### Class `jules::base_slice<1>::iterator`<a id="jules::base_slice-1-::iterator"></a>

``` cpp
class iterator
: public std::iterator<std::input_iterator_tag, index_t, distance_t, void *, index_t>
{
public:
    iterator() = delete;
    
    constexpr iterator(const iterator& source) = default;
    
    constexpr iterator(iterator&& source) noexcept = default;
    
    constexpr iterator& operator=(const iterator& source) = default;
    
    constexpr iterator& operator=(iterator&& source) noexcept = default;
    
    constexpr iterator& operator++();
    
    constexpr iterator operator++(int);
    
    constexpr auto operator==(const iterator& other) const;
    
    constexpr auto operator!=(const iterator& other) const;
    
    constexpr index_t operator*() const;
    
    void* operator->() const;
};
```

**TODO**: No documentation, consult `jules::base_slice<N>::iterator`.

*Notes*: For this specialization, the iterator does not depend on the parent slice.

#### Operator `jules::base_slice<1>::iterator::operator->`<a id="jules::base_slice-1-::iterator::operator--()const"></a>

``` cpp
void* operator->() const;
```

*Notes*: You should not call this function.

-----

### Variable `jules::base_slice<1>::all`<a id="jules::base_slice-1-::all"></a>

``` cpp
static constexpr auto all = index_t{0ul};
```

Unsigned integer that represents all possibles extents when applied to an array.

### Constructor `jules::base_slice<1>::base_slice`<a id="jules::base_slice-1-"></a>

``` cpp
(1)  constexpr base_slice(index_t start, index_t extent, index_t stride);

(2)  constexpr base_slice(index_t start, index_t extent);

(3)  constexpr base_slice() = default;
```

**Parameters:**

  - <a id="jules::base_slice-1-"></a>`start` - Start position of the slicing. It defaults to 0ul.

### Function `jules::base_slice<1>::size`<a id="jules::base_slice-1-::size()const"></a>

``` cpp
constexpr auto size() const;
```

Effectively the product of the extents.

### Function call operator `jules::base_slice<1>::operator()`<a id="jules::base_slice-1-"></a>

``` cpp
(1)  constexpr index_t operator()(index_t index) const;
```

Returns the memory position of the `index`.

-----

-----
