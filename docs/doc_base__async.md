---
---

# Header file `base/async.hpp`<a id="base/async.hpp"></a>

``` cpp
#define JULES_BASE_ASYNC_H

namespace jules
{
    namespace detail
    {
        template <typename F>
        struct defer_helper;
    }
    
    template <typename F>
    auto defer(F&& f);
    
    template <typename T>
    T* move_ptr(T*& pointer);
}
```

-----
