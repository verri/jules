---
---

# Header file `array/numeric.hpp`<a id="array/numeric.hpp"></a>

``` cpp
#define JULES_ARRAY_NUMERIC_H

namespace jules
{
    std::vector<index_t> seq(const index_t start, const index_t stop, const distance_t step = 1);
    
    template <typename T, typename A>
    meta::requires_t<vector<T>, Array<std::decay_t<A>>> to_vector(A&& array);
    
    template <typename T, typename Rng, typename = meta::requires<range::Range<std::decay_t<Rng>>>>
    meta::fallback_t<vector<T>, Array<std::decay_t<Rng>>> to_vector(Rng&& rng);
    
    template <typename A>
    meta::requires_t<vector<typename A::value_type>, Array<std::decay_t<A>>> as_vector(A&& array);
    
    template <typename Rng, typename R = range::range_value_t<std::decay_t<Rng>>, typename = meta::requires<range::Range<std::decay_t<Rng>>>>
    meta::fallback_t<vector<R>, Array<std::decay_t<Rng>>> as_vector(Rng&& rng);
    
    template <typename ... Args, typename = std::enable_if_t<(sizeof...(Args)>1)>>
    auto as_vector(Args&&... args);
    
    template <typename A, typename T = typename A::value_type, typename = meta::requires<Array<A>>>
    auto normal_pdf(const A& array, T mu, T sigma);
}
```
