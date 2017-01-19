---
---

# Header file `array/ind_array.hpp`<a id="array/ind_array.hpp"></a>

``` cpp
#define JULES_ARRAY_IND_ARRAY_H

namespace jules
{
    template <typename T, std::size_t N>
    class ind_array;
    
    template <typename T>
    class ind_array<T, 1>;
}
```

## Class template `jules::ind_array` \[Array Types\]<a id="jules::ind_array-T,N-"></a>

``` cpp
template <typename T, std::size_t N>
class ind_array
{
public:
    using vector_type = std::vector<index_t>;
    
    using value_type = T;
    
    static constexpr auto order = N;
    
    using size_type = index_t;
    
    using difference_type = distance_t;
    
    using iterator = /*unspecified*/;
    
    using const_iterator = /*unspecified*/;
    
    ind_array() = default;
    
    ind_array(T* data, std::array<index_t, N> extents, vector_type indexes);
    
    ind_array(const ind_array& source) = default;
    
    ind_array(ind_array&& source) noexcept = default;
    
    ~ind_array() = default;
    
    template <typename A>
    meta::requires_t<ind_array&, Array<A>> operator=(const A& source);
    template <typename U>
    meta::fallback_t<ind_array&, Array<U>> operator=(const U& source);
    ind_array& operator=(base_array<T, N>&& source) noexcept;
    
    operator ind_array<const T, N>() const;
    
    ind_array<T, N-1> operator[](index_t i);
    ind_array<const T, N-1> operator[](index_t i) const;
    
    template <typename ... Args>
    detail::indirect_request<ind_array<T, N>, Args...> operator()(Args&&... args);
    
    template <typename ... Args>
    detail::slice_request<ind_array<T, N>, Args...> operator()(Args&&... args);
    
    template <typename ... Args>
    detail::element_request<T&, Args...> operator()(Args&&... args);
    
    template <typename ... Args>
    detail::indirect_request<ind_array<const T, N>, Args...> operator()(Args&&... args) const;
    
    template <typename ... Args>
    detail::slice_request<ind_array<const T, N>, Args...> operator()(Args&&... args) const;
    
    template <typename ... Args>
    detail::element_request<const T&, Args...> operator()(Args&&... args) const;
    
    iterator begin();
    
    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;
    
    const_iterator cbegin() const;
    
    const_iterator cend() const;
    
    auto descriptor() const;
    
    auto data() const;
    
    auto extents() const;
    
    auto size() const;
    
    auto row_count() const;
    
    auto column_count() const;
};
```

Array indirect reference.

This class is used internally by `jules` to represent a view of an concrete array with arbitrary positions in each dimension.

### Default constructor `jules::ind_array::ind_array`<a id="jules::ind_array-T,N-::ind_array()"></a>

``` cpp
ind_array() = default;
```

*TODO*: Explain why the user should probably not call these functions. In C++17, we can provide a helper that generates a view with more security.

### Assignment operator `jules::ind_array::operator=`<a id="jules::ind_array-T,N-::operator=(constA&)"></a>

``` cpp
(1)  template <typename A>
     meta::requires_t<ind_array&, Array<A>> operator=(const A& source);

(2)  template <typename U>
     meta::fallback_t<ind_array&, Array<U>> operator=(const U& source);

(3)  ind_array& operator=(base_array<T, N>&& source) noexcept;
```

### Conversion operator `jules::ind_array::operator ind_array<const T, N>`<a id="jules::ind_array-T,N-::operatorind_array-constT,N-()const"></a>

``` cpp
operator ind_array<const T, N>() const;
```

Implicitly convertable to hold const values.

### Array subscript operator `jules::ind_array::operator[]`<a id="jules::ind_array-T,N-::operator%5b%5d(index_t)"></a>

``` cpp
(1)  ind_array<T, N-1> operator[](index_t i);

(2)  ind_array<const T, N-1> operator[](index_t i) const;
```

-----

## Class template `jules::ind_array<T, 1>` \[Array Types\]<a id="jules::ind_array-T,1-"></a>

``` cpp
template <typename T>
class ind_array<T, 1>
{
public:
    using vector_type = std::vector<index_t>;
    
    using value_type = T;
    
    static constexpr auto order = 1;
    
    using size_type = index_t;
    
    using difference_type = distance_t;
    
    using iterator = /*unspecified*/;
    
    using const_iterator = /*unspecified*/;
    
    ind_array() = default;
    
    ind_array(T* data, index_t extent, vector_type indexes);
    
    ind_array(const ind_array& source) = default;
    
    ind_array(ind_array&& source) noexcept = default;
    
    ~ind_array() = default;
    
    template <typename A>
    meta::requires_t<ind_array&, Array<A>> operator=(const A& source);
    template <typename U>
    meta::fallback_t<ind_array&, Array<U>> operator=(const U& source);
    ind_array& operator=(const ind_array<T, 1>& source);
    ind_array& operator=(base_array<T, 1>&& source) noexcept;
    
    operator ind_array<const T, 1>() const;
    
    T& operator[](index_t i);
    const T& operator[](index_t i) const;
    
    template <typename Rng, typename U = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    ind_array<T, 1> operator()(const Rng& rng);
    
    ind_array<T, 1> operator()(std::vector<index_t>&& indexes);
    
    ind_array<T, 1> operator()(const base_slice<1>& slice);
    
    T& operator()(index_t i);
    
    template <typename Rng, typename U = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    ind_array<const T, 1> operator()(const Rng& rng) const;
    
    ind_array<const T, 1> operator()(std::vector<index_t>&& indexes) const;
    
    ind_array<const T, 1> operator()(const base_slice<1>& slice) const;
    
    const T& operator()(index_t i) const;
    
    iterator begin();
    
    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;
    
    const_iterator cbegin() const;
    
    const_iterator cend() const;
    
    auto descriptor() const;
    
    auto data() const;
    
    auto extents() const;
    
    auto size() const;
    
    auto length() const;
};
```

1-D Indirect array specialization.

This class is used internally by `jules` to represent a view of an concrete array with arbitrary positions in each dimension.

### Default constructor `jules::ind_array<T, 1>::ind_array`<a id="jules::ind_array-T,1-::ind_array()"></a>

``` cpp
ind_array() = default;
```

*TODO*: Explain why the user should probably not call this function. In C++17, we can provide a helper that generates a view with more security.

### Assignment operator `jules::ind_array<T, 1>::operator=`<a id="jules::ind_array-T,1-::operator=(constA&)"></a>

``` cpp
(1)  template <typename A>
     meta::requires_t<ind_array&, Array<A>> operator=(const A& source);

(2)  template <typename U>
     meta::fallback_t<ind_array&, Array<U>> operator=(const U& source);

(3)  ind_array& operator=(const ind_array<T, 1>& source);

(4)  ind_array& operator=(base_array<T, 1>&& source) noexcept;
```

### Conversion operator `jules::ind_array<T, 1>::operator ind_array<const T, 1>`<a id="jules::ind_array-T,1-::operatorind_array-constT,1-()const"></a>

``` cpp
operator ind_array<const T, 1>() const;
```

Implicitly convertable to hold const values.

### Array subscript operator `jules::ind_array<T, 1>::operator[]`<a id="jules::ind_array-T,1-::operator%5b%5d(index_t)"></a>

``` cpp
(1)  T& operator[](index_t i);

(2)  const T& operator[](index_t i) const;
```

-----

-----
