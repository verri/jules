---
---

# Header file `array/ref_array.hpp`<a id="array/ref_array.hpp"></a>

``` cpp
#define JULES_ARRAY_REF_ARRAY_H

namespace jules
{
    template <typename T, std::size_t N>
    class ref_array;
    
    template <typename T>
    class ref_array<T, 1>;
}
```

## Class template `jules::ref_array` \[N-Dimensional Array\]<a id="jules::ref_array-T,N-"></a>

``` cpp
template <typename T, std::size_t N>
class ref_array
{
public:
    using value_type = T;
    
    static constexpr auto order = N;
    
    using size_type = index_t;
    
    using difference_type = distance_t;
    
    using iterator = detail::iterator_from_indexes<T, typename base_slice<N>::iterator>;
    
    using const_iterator = detail::iterator_from_indexes<const T, typename base_slice<N>::iterator>;
    
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
    
    auto descriptor() const;
    
    auto data() const;
    
    auto extents() const;
    
    auto size() const;
    
    auto row_count() const;
    
    auto column_count() const;
    
protected:
    T* data_;
    
    base_slice<N> descriptor_;
};
```

Array reference.

This class is used internally by `jules` to represent a view of an concrete array.

### Default constructor `jules::ref_array::ref_array`<a id="jules::ref_array-T,N-::ref_array()"></a>

``` cpp
ref_array() = default;
```

*TODO*: Explain why the user should probably not call these functions. In C++17, we can provide a helper that generates a view with more security.

### Function template `jules::ref_array::operator=`<a id="jules::ref_array-T,N-"></a>

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

### Array subscript operator `jules::ref_array::operator[]`<a id="jules::ref_array-T,N-"></a>

``` cpp
(1)  ref_array<T, N-1> operator[](index_t i);

(2)  ref_array<const T, N-1> operator[](index_t i) const;
```

-----

## Class template `jules::ref_array<T, 1>` \[N-Dimensional Array\]<a id="jules::ref_array-T,1-"></a>

``` cpp
template <typename T>
class ref_array<T, 1>
{
public:
    using value_type = T;
    
    static constexpr auto order = 1;
    
    using size_type = index_t;
    
    using difference_type = distance_t;
    
    using iterator = detail::iterator_from_indexes<T, typename base_slice<1>::iterator>;
    
    using const_iterator = detail::iterator_from_indexes<const T, typename base_slice<1>::iterator>;
    
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
    
    auto descriptor() const;
    
    auto data() const;
    
    auto extents() const;
    
    auto size() const;
    
    auto length() const;
    
protected:
    T* data_;
    
    base_slice<1> descriptor_;
};
```

1-D Array reference specialization.

This class is used internally by `jules` to represent a view of an concrete array.

### Default constructor `jules::ref_array<T, 1>::ref_array`<a id="jules::ref_array-T,1-::ref_array()"></a>

``` cpp
ref_array() = default;
```

*TODO*: Explain why the user should probably not call this function. In C++17, we can provide a helper that generates a view with more security.

### Function template `jules::ref_array<T, 1>::operator=`<a id="jules::ref_array-T,1-"></a>

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

### Array subscript operator `jules::ref_array<T, 1>::operator[]`<a id="jules::ref_array-T,1-"></a>

``` cpp
(1)  T& operator[](index_t i);

(2)  const T& operator[](index_t i) const;
```

-----

-----
