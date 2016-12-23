---
---

# Header file `base/numeric.hpp`<a id="base/numeric.hpp"></a>

``` cpp
#define JULES_BASE_NUMERIC_H

namespace jules
{
    namespace detail
    {
        template <typename T, std::size_t ... I>
        constexpr std::array<T, sizeof...(I)> repeat_impl(const T& value, std::index_sequence<I...>);
        
        struct forward_arithmetic;
    }
    
    template <std::size_t N, typename T>
    constexpr auto repeat(const T& value);
    
    template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto max(Iter first, Sent last, T start = -numeric_traits<T>::infinity());
    
    template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    auto max(const Rng& rng, T start = -numeric_traits<T>::infinity());
    
    template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto prod(Iter first, Sent last, T start = numeric_traits<T>::multiplicative_identity());
    
    template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    auto prod(const Rng& rng, T start = numeric_traits<T>::multiplicative_identity());
    
    constexpr detail::forward_arithmetic prod_args();
    
    template <typename T, typename ... Args>
    constexpr auto prod_args(const T& arg, Args&&... args);
    
    template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto sum(Iter first, Sent last, T start = numeric_traits<T>::additive_identity());
    
    template <typename Rng, typename T = range::range_value_t<Rng>, typename = meta::requires<range::Range<Rng>>>
    auto sum(const Rng& rng, T start = numeric_traits<T>::additive_identity());
    
    constexpr detail::forward_arithmetic sum_args();
    
    template <typename T, typename ... Args>
    constexpr auto sum_args(const T& arg, Args&&... args);
    
    constexpr auto all_args();
    
    template <typename ... Args>
    constexpr auto all_args(bool arg, Args&&... args);
    
    template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
    auto all(Iter first, Sent last);
    
    template <typename Rng, typename = meta::requires<range::Range<Rng>>>
    auto all(const Rng& rng);
    
    constexpr auto any_args();
    
    template <typename ... Args>
    constexpr auto any_args(bool arg, Args&&... args);
    
    template <typename T>
    constexpr auto square(const T& value);
    
    template <typename T>
    auto normal_pdf(const T& x, const T& mu, const T& sigma);
}
```

-----
