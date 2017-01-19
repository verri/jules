---
---

# Header file `array/ref_array.hpp`<a id="array/ref_array.hpp"></a>

``` cpp
namespace jules
{
    template <typename T, std::size_t N>
    class ref_array;
    
    template <typename T>
    class ref_array<T, 1>;
}
```

## Class template `jules::ref_array` \[Array Types\]<a id="jules::ref_array-T,N-"></a>

``` cpp
template <typename T, std::size_t N>
class ref_array
{
public:
    //=== Class Types and Constants ===//
    static constexpr auto order = N;
    using value_type = T;
    using iterator = detail::iterator_from_indexes<T, typename base_slice<N>::iterator>;
    using const_iterator = detail::iterator_from_indexes<const T, typename base_slice<N>::iterator>;
    using size_type = index_t;
    using difference_type = distance_t;
    
    ref_array() = default;
    
    ref_array(T* data, base_slice<N> descriptor);
    
    ref_array(const ref_array& source) = default;
    
    ref_array(ref_array&& source) noexcept = default;
    
    ~ref_array() = default;
    
    template <typename A>
    meta::requires_t<ref_array&, Array<A>> operator=(const A& source);
    template <typename U>
    meta::fallback_t<ref_array&, Array<U>> operator=(const U& source);
    ref_array& operator=(base_array<T, N>&& source) noexcept;
    
    operator ref_array<const T, N>() const;
    
    ref_array<T, N-1> operator[](index_t i);
    ref_array<const T, N-1> operator[](index_t i) const;
    
    ref_array<T, N>& operator()();
    
    ref_array<const T, N> operator()() const;
    
    template <typename ... Args>
    detail::indirect_request<ind_array<T, N>, Args...> operator()(Args&&... args);
    
    template <typename ... Args>
    detail::slice_request<ref_array<T, N>, Args...> operator()(Args&&... args);
    
    template <typename ... Args>
    detail::element_request<T&, Args...> operator()(Args&&... args);
    
    template <typename ... Args>
    decltype(auto) operator()(Args&&... args) const;
    
    iterator begin();
    
    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;
    
    const_iterator cbegin() const;
    
    const_iterator cend() const;
    
    base_slice<order> descriptor() const;
    
    value_type* data() const;
    
    /*unspecified*/ extents() const;
    
    index_t size() const;
    
    index_t row_count() const;
    
    index_t column_count() const;
};
```

Array reference.

This class represents a view of an concrete array.

*Notes*: This class is not meant to be used in user code.

### Class Types and Constants<a id="jules::ref_array-T,N-::order"></a>

``` cpp
(1)  static constexpr auto order = N;

(2)  using value_type = T;

(3)  using iterator = detail::iterator_from_indexes<T, typename base_slice<N>::iterator>;

(4)  using const_iterator = detail::iterator_from_indexes<const T, typename base_slice<N>::iterator>;

(5)  using size_type = index_t;

(6)  using difference_type = distance_t;
```

(1) The order of dimensionality.

(2) The type of the elements.

(3) `ForwardIterator` over the elements.

(4) Constant `ForwardIterator` over the elements.

(5) Unsigned integer type that can store the dimensions of the array.

(6) Signed integer type that can store differences between sizes.

### Default constructor `jules::ref_array::ref_array`<a id="jules::ref_array-T,N-::ref_array()"></a>

``` cpp
ref_array() = default;
```

*TODO*: Explain why the user should probably not call these functions. In C++17, we can provide a helper that generates a view with more security.

### Assignment operator `jules::ref_array::operator=`<a id="jules::ref_array-T,N-::operator=(constA&)"></a>

``` cpp
(1)  template <typename A>
     meta::requires_t<ref_array&, Array<A>> operator=(const A& source);

(2)  template <typename U>
     meta::fallback_t<ref_array&, Array<U>> operator=(const U& source);

(3)  ref_array& operator=(base_array<T, N>&& source) noexcept;
```

### Conversion operator `jules::ref_array::operator ref_array<const T, N>`<a id="jules::ref_array-T,N-::operatorref_array-constT,N-()const"></a>

``` cpp
operator ref_array<const T, N>() const;
```

Implicitly convertable to hold const values.

### Array subscript operator `jules::ref_array::operator[]`<a id="jules::ref_array-T,N-::operator%5b%5d(index_t)"></a>

``` cpp
(1)  ref_array<T, N-1> operator[](index_t i);

(2)  ref_array<const T, N-1> operator[](index_t i) const;
```

-----

## Class template `jules::ref_array<T, 1>` \[Array Types\]<a id="jules::ref_array-T,1-"></a>

``` cpp
template <typename T>
class ref_array<T, 1>
{
public:
    //=== Class Types and Constants ===//
    static constexpr auto order = std::size_t{1u};
    using value_type = T;
    using iterator = detail::iterator_from_indexes<T, typename base_slice<1>::iterator>;
    using const_iterator = detail::iterator_from_indexes<const T, typename base_slice<1>::iterator>;
    using size_type = index_t;
    using difference_type = distance_t;
    
    ref_array() = default;
    
    ref_array(T* data, base_slice<1> descriptor);
    
    ref_array(const ref_array& source) = default;
    
    ref_array(ref_array&& source) noexcept = default;
    
    ~ref_array() = default;
    
    template <typename A>
    meta::requires_t<ref_array&, Array<A>> operator=(const A& source);
    template <typename U>
    meta::fallback_t<ref_array&, Array<U>> operator=(const U& source);
    ref_array& operator=(const ref_array<T, 1>& source);
    ref_array& operator=(base_array<T, 1>&& source) noexcept;
    
    operator ref_array<const T, 1>() const;
    
    T& operator[](index_t i);
    const T& operator[](index_t i) const;
    
    ref_array<T, 1>& operator()();
    
    ref_array<const T, 1> operator()() const;
    
    template <typename Rng, typename U = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    ind_array<T, 1> operator()(const Rng& rng);
    
    ind_array<T, 1> operator()(std::vector<index_t>&& indexes);
    
    ref_array<T, 1> operator()(const base_slice<1>& slice);
    
    T& operator()(index_t i);
    
    template <typename Rng, typename U = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    ind_array<const T, 1> operator()(const Rng& rng) const;
    
    ind_array<const T, 1> operator()(std::vector<index_t>&& indexes) const;
    
    ref_array<const T, 1> operator()(const base_slice<1>& slice) const;
    
    const T& operator()(index_t i) const;
    
    iterator begin();
    
    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;
    
    const_iterator cbegin() const;
    
    const_iterator cend() const;
    
    base_slice<1> descriptor() const;
    
    value_type* data() const;
    
    index_t extents() const;
    
    index_t size() const;
    
    index_t length() const;
};
```

1-D Array reference specialization.

This class represents a view of a concrete array.

*Notes*: This class is not meant to be used in user code.

### Class Types and Constants<a id="jules::ref_array-T,1-::order"></a>

``` cpp
(1)  static constexpr auto order = std::size_t{1u};

(2)  using value_type = T;

(3)  using iterator = detail::iterator_from_indexes<T, typename base_slice<1>::iterator>;

(4)  using const_iterator = detail::iterator_from_indexes<const T, typename base_slice<1>::iterator>;

(5)  using size_type = index_t;

(6)  using difference_type = distance_t;
```

(1) The order of dimensionality.

(2) The type of the elements.

(3) `ForwardIterator` over the elements.

(4) Constant `ForwardIterator` over the elements.

(5) Unsigned integer type that can store the dimensions of the array.

(6) Signed integer type that can store differences between sizes.

### Default constructor `jules::ref_array<T, 1>::ref_array`<a id="jules::ref_array-T,1-::ref_array()"></a>

``` cpp
ref_array() = default;
```

*TODO*: Explain why the user should probably not call this function. In C++17, we can provide a helper that generates a view with more security.

### Assignment operator `jules::ref_array<T, 1>::operator=`<a id="jules::ref_array-T,1-::operator=(constA&)"></a>

``` cpp
(1)  template <typename A>
     meta::requires_t<ref_array&, Array<A>> operator=(const A& source);

(2)  template <typename U>
     meta::fallback_t<ref_array&, Array<U>> operator=(const U& source);

(3)  ref_array& operator=(const ref_array<T, 1>& source);

(4)  ref_array& operator=(base_array<T, 1>&& source) noexcept;
```

### Conversion operator `jules::ref_array<T, 1>::operator ref_array<const T, 1>`<a id="jules::ref_array-T,1-::operatorref_array-constT,1-()const"></a>

``` cpp
operator ref_array<const T, 1>() const;
```

Implicitly convertable to hold const values.

### Array subscript operator `jules::ref_array<T, 1>::operator[]`<a id="jules::ref_array-T,1-::operator%5b%5d(index_t)"></a>

``` cpp
(1)  T& operator[](index_t i);

(2)  const T& operator[](index_t i) const;
```

-----

-----
