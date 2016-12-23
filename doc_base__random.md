---
---

# Header file `base/random.hpp`<a id="base/random.hpp"></a>

``` cpp
#define JULES_BASE_RANDOM_H

namespace jules
{
    thread_local auto random_engine = std::mt19937{};
    
    template <typename Dist>
    auto sample(Dist& dist);
    
    template <typename Dist>
    auto sample(index_t n, Dist& dist);
    
    auto bernoulli_sample(index_t n, numeric p);
    
    auto bernoulli_sample(numeric p);
    
    auto uniform_index_sample(index_t end);
    
    template <typename Rng, typename U = range::range_value_t<std::decay_t<Rng>>, typename = meta::requires<range::Range<Rng>>>
    auto discrete_index_sample(Rng&& rng);
}
```
