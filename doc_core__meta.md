---
---

# Header file `core/meta.hpp`<a id="core/meta.hpp"></a>

``` cpp
#define JULES_CORE_META_H

namespace jules
{
    namespace meta
    {
        template <typename>
        using void_t = void;
        
        template <typename>
        struct conjunction;
        
        template <typename B>
        struct conjunction<B>;
        
        template <typename B, typename ... Bs>
        struct conjunction<B, Bs...>;
        
        template <bool B>
        using bool_constant = std::integral_constant<bool, B>;
        
        template <typename B>
        struct negation;
        
        template <typename T, template <typename> class Expression, typename = void_t<>>
        struct compiles;
        
        template <typename T, template <typename> class Expression>
        struct compiles<T, Expression, void_t<Expression<T>>>;
        
        template <typename ... Checks>
        using requires = std::enable_if_t<conjunction<Checks...>::value>;
        
        template <typename ... Checks>
        using fallback = std::enable_if_t<conjunction<negation<Checks...>>::value>;
        
        template <typename R, typename ... Checks>
        using requires_t = std::enable_if_t<conjunction<Checks...>::value, R>;
        
        template <typename R, typename ... Checks>
        using fallback_t = std::enable_if_t<conjunction<negation<Checks...>>::value, R>;
    }
}
```

-----
