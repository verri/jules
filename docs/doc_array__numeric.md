---
---

# Header file `array/numeric.hpp`<a id="array/numeric.hpp"></a>

``` cpp
#define JULES_ARRAY_NUMERIC_H

namespace jules
{
    vector<index_t> seq(index_t start, index_t stop, index_t step = 1u);
    
    template <typename T, typename Rng, int _concept_requires_34 = 42, typename std::enable_if<(_concept_requires_34==43)||(range::Range<Rng>()), int>::type=0>
    /*
    */>vector<T> to_vector(const Rng& rng);
    
    template <typename Rng, typename R = range::range_value_t<Rng>, int _concept_requires_41 = 42, typename std::enable_if<(_concept_requires_41==43)||(range::Range<Rng>()), int>::type=0>
    /*
    */>vector<R> as_vector(const Rng& rng);
    
    template <typename ... Args, typename = std::enable_if_t<(sizeof...(Args)>1)>>
    auto as_vector(Args&&... args);
}
```
