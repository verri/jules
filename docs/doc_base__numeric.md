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
    
    template <typename Iter, typename Sent, int _concept_requires_44 = 42, typename std::enable_if<(_concept_requires_44==43)||(range::Sentinel<Sent, Iter>()), int>::type=0>
    /*
    */> max(Iter first, Sent last);
    
    template <typename Rng, int _concept_requires_49 = 42, typename std::enable_if<(_concept_requires_49==43)||(range::Range<Rng>()), int>::type=0>
    /*
    */> max(const Rng& rng);
    
    template <typename Iter, typename Sent, typename T = range::iterator_value_t<Iter>, int _concept_requires_55 = 42, typename std::enable_if<(_concept_requires_55==43)||(range::Sentinel<Sent, Iter>()), int>::type=0>
    /*
    */> prod(Iter first, Sent last, const T& start = static_cast<T>(1u));
    
    template <typename Rng, typename T = range::range_value_t<Rng>, int _concept_requires_61 = 42, typename std::enable_if<(_concept_requires_61==43)||(range::Range<Rng>()), int>::type=0>
    /*
    */> prod(Rng&& rng, const T& start = static_cast<T>(1u));
    
    constexpr detail::forward_arithmetic prod_args();
    
    template <typename T, typename ... Args>
    constexpr auto prod_args(const T& arg, Args&&... args);
    
    constexpr auto all_args();
    
    template <typename ... Args>
    constexpr auto all_args(bool arg, Args&&... args);
    
    constexpr auto any_args();
    
    template <typename ... Args>
    constexpr auto any_args(bool arg, Args&&... args);
}
```

-----
