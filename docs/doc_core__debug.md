# Header file `core/debug.hpp`<a id="core/debug.hpp"></a>

``` cpp
#define JULES_CORE_DEBUG_H

#define JULES_DEBUG_LEVEL

namespace jules
{
    namespace debug
    {
        namespace level
        {
            constexpr auto unreachable = debug_assert::level<1u>{};
            
            constexpr auto invalid_argument = debug_assert::level<2u>{};
            
            constexpr auto invalid_state = debug_assert::level<2u>{};
            
            constexpr auto boundary_check = debug_assert::level<3u>{};
            
            constexpr auto extents_check = debug_assert::level<3u>{};
        }
        
        struct module;
    }
}
```

## Namespace `jules::debug::level`<a id="jules::debug::level"></a>

``` cpp
namespace level
{
    constexpr auto unreachable = debug_assert::level<1u>{};
    
    constexpr auto invalid_argument = debug_assert::level<2u>{};
    
    constexpr auto invalid_state = debug_assert::level<2u>{};
    
    constexpr auto boundary_check = debug_assert::level<3u>{};
    
    constexpr auto extents_check = debug_assert::level<3u>{};
}
```

Debug level for each kind of checking.

-----

-----
