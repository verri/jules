---
---

# Header file `core/range.hpp`<a id="core/range.hpp"></a>

``` cpp
#define JULES_CORE_RANGE_H

namespace ranges
{
    inline namespace v3
    {
        namespace view
        {
            template <typename T>
            & numbers(T start, T step = {});
            
            template <typename T>
            auto numbers_n(T start, ::jules::index_t n, T step = {});
            
            template <typename T>
            auto interval(T start, T end, T step);
            
            template <typename T>
            auto interval_n(T start, T end, ::jules::index_t n);
        }
    }
}

namespace jules
{
    namespace range
    {
        using namespace ranges::v3;
    }
}
```

-----

-----

-----
