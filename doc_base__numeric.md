---
---

# Header file `base/numeric.hpp`<a id="base/numeric.hpp"></a>

``` cpp
#define JULES_BASE_NUMERIC_H

namespace jules
{
    template <std::size_t N, typename T>
    constexpr auto repeat(const T& value);
    template <typename T>
    auto repeat(index_t N, const T& value);
    
    template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto length(Iter first, Sent last);
    template <typename Rng, typename = meta::requires<range::Range<Rng>>>
    auto length(const Rng& rng);
    
    template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto max(Iter first, Sent last, T start = numeric_traits<T>::unbounded_min());
    template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    auto max(const Rng& rng, T start = numeric_traits<T>::unbounded_min());
    
    template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto min(Iter first, Sent last, T start = numeric_traits<T>::unbounded_max());
    template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    auto min(const Rng& rng, T start = numeric_traits<T>::unbounded_max());
    
    template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto prod(Iter first, Sent last, T start = numeric_traits<T>::multiplicative_identity());
    template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    auto prod(const Rng& rng, T start = numeric_traits<T>::multiplicative_identity());
    
    template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto sum(Iter first, Sent last, T start = numeric_traits<T>::additive_identity());
    template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    auto sum(const Rng& rng, T start = numeric_traits<T>::additive_identity());
    
    template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto count(Iter first, Sent last);
    template <typename Rng, typename = meta::requires<range::Range<Rng>>>
    auto count(const Rng& rng);
    
    template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto all(Iter first, Sent last);
    template <typename Rng, typename = meta::requires<range::Range<Rng>>>
    auto all(const Rng& rng);
    
    template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto none(Iter first, Sent last);
    template <typename Rng, typename = meta::requires<range::Range<Rng>>>
    auto none(const Rng& rng);
    
    template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto any(Iter first, Sent last);
    template <typename Rng, typename = meta::requires<range::Range<Rng>>>
    auto any(const Rng& rng);
    
    constexpr /*unspecified*/ prod_args();
    
    template <typename T, typename ... Args>
    constexpr auto prod_args(const T& arg, Args&&... args);
    
    constexpr /*unspecified*/ sum_args();
    
    template <typename T, typename ... Args>
    constexpr auto sum_args(const T& arg, Args&&... args);
    
    constexpr auto all_args();
    
    template <typename ... Args>
    constexpr auto all_args(bool arg, Args&&... args);
    
    constexpr auto any_args();
    
    template <typename ... Args>
    constexpr auto any_args(bool arg, Args&&... args);
    
    template <typename T>
    constexpr auto square(const T& value);
    
    template <typename T>
    auto normal_pdf(const T& x, const T& mu, const T& sigma);
}
```

## Function template `jules::repeat` \[Arithmetic\]<a id="jules::repeat(constT&)"></a>

``` cpp
(1)  template <std::size_t N, typename T>
     constexpr auto repeat(const T& value);

(2)  template <typename T>
     auto repeat(index_t N, const T& value);
```

Repeats `value` `N` times.

*Returns*: [std::array](http://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=std::array) and [std::vector](http://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=std::vector) containing the repeated value if `N` is a template argument or not, respectively.

## Function template `jules::length` \[Arithmetic\]<a id="jules::length(Iter,Sent)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto length(Iter first, Sent last);

(2)  template <typename Rng, typename = meta::requires<range::Range<Rng>>>
     auto length(const Rng& rng);
```

Returns either the length of a `Range` or the distance of a pair of iterators.

*Notes*: The value may be negative if random-access iterators are used and `first` is reachable from `last`.

## Function template `jules::max` \[Arithmetic\]<a id="jules::max(Iter,Sent,T)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto max(Iter first, Sent last, T start = numeric_traits<T>::unbounded_min());

(2)  template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
     auto max(const Rng& rng, T start = numeric_traits<T>::unbounded_min());
```

Returns either the maximum element in a `Range` or in the sequence \[`first`, `last`).

*Notes*: [jules::numeric\_traits\<T\>](doc_core__type.html#core/type.hpp) must implement `unbounded_min`.

*Notes*: If empty, returns [jules::numeric\_traits\<T\>::unbounded\_min](doc_core__type.html#core/type.hpp).

## Function template `jules::min` \[Arithmetic\]<a id="jules::min(Iter,Sent,T)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto min(Iter first, Sent last, T start = numeric_traits<T>::unbounded_max());

(2)  template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
     auto min(const Rng& rng, T start = numeric_traits<T>::unbounded_max());
```

Returns either the minimum element in a `Range` or in the sequence \[`first`, `last`).

*Notes*: [jules::numeric\_traits\<T\>](doc_core__type.html#core/type.hpp) must implement `unbounded_max`.

*Notes*: If empty, returns [jules::numeric\_traits\<T\>::unbounded\_max](doc_core__type.html#core/type.hpp).

## Function template `jules::prod` \[Arithmetic\]<a id="jules::prod(Iter,Sent,T)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto prod(Iter first, Sent last, T start = numeric_traits<T>::multiplicative_identity());

(2)  template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
     auto prod(const Rng& rng, T start = numeric_traits<T>::multiplicative_identity());
```

Returns either the product of the elements in a `Range` or in the sequence \[`first`, `last`).

*Notes*: [jules::numeric\_traits\<T\>](doc_core__type.html#core/type.hpp) must implement `multiplicative_identity`.

*Notes*: If empty, returns [jules::numeric\_traits\<T\>::multiplicative\_identity](doc_core__type.html#core/type.hpp).

## Function template `jules::sum` \[Arithmetic\]<a id="jules::sum(Iter,Sent,T)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto sum(Iter first, Sent last, T start = numeric_traits<T>::additive_identity());

(2)  template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
     auto sum(const Rng& rng, T start = numeric_traits<T>::additive_identity());
```

Returns either the summation of the elements in a `Range` or in the sequence \[`first`, `last`).

*Notes*: [jules::numeric\_traits\<T\>](doc_core__type.html#core/type.hpp) must implement `additive_identity`.

*Notes*: If empty, returns [jules::numeric\_traits\<T\>::additive\_identity](doc_core__type.html#core/type.hpp).

## Function template `jules::count` \[Logical\]<a id="jules::count(Iter,Sent)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto count(Iter first, Sent last);

(2)  template <typename Rng, typename = meta::requires<range::Range<Rng>>>
     auto count(const Rng& rng);
```

Returns the number of true elements in a `Range` or in the sequence \[`first`, `last`).

## Function template `jules::all` \[Logical\]<a id="jules::all(Iter,Sent)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto all(Iter first, Sent last);

(2)  template <typename Rng, typename = meta::requires<range::Range<Rng>>>
     auto all(const Rng& rng);
```

Checks whether all elements in a `Range` or in the sequence \[`first`, `last`) are true.

*Notes*: If empty, returns true.

## Function template `jules::none` \[Logical\]<a id="jules::none(Iter,Sent)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto none(Iter first, Sent last);

(2)  template <typename Rng, typename = meta::requires<range::Range<Rng>>>
     auto none(const Rng& rng);
```

Checks whether all elements in a `Range` or in the sequence \[`first`, `last`) are false.

*Notes*: If empty, returns true.

## Function template `jules::any` \[Logical\]<a id="jules::any(Iter,Sent)"></a>

``` cpp
(1)  template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
     auto any(Iter first, Sent last);

(2)  template <typename Rng, typename = meta::requires<range::Range<Rng>>>
     auto any(const Rng& rng);
```

Checks whether any of the elements in a `Range` or in the sequence \[`first`, `last`) are true.

*Notes*: If empty, returns false.

-----
