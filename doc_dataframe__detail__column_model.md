---
---

# Header file `dataframe/detail/column_model.hpp`<a id="dataframe/detail/column_model.hpp"></a>

``` cpp
#define JULES_DATAFRAME_DETAIL_COLUMN_MODEL_H

namespace jules
{
    namespace detail
    {
        template <typename Eraser, typename Coercion, std::size_t I>
        class generate_virtual_coercions;
        
        template <typename Eraser, typename Coercion>
        class generate_virtual_coercions<Eraser, Coercion, 0>;
        
        template <typename Coercion>
        class column_interface;
        
        template <typename T, typename U, typename Coercion, std::size_t I, typename Enabler = void>
        class specific_concrete_coercion;
        
        template <typename T, typename U, typename Coercion, std::size_t I>
        class specific_concrete_coercion<T, U, Coercion, I, std::enable_if_t<std::is_same<decltype(Coercion::template rule<I>::coerce_from(std::declval<T>())), U>::value&&!std::is_convertible<T, U>::value>>;
        
        template <typename T, typename U, typename Coercion, std::size_t I>
        class specific_concrete_coercion<T, U, Coercion, I, std::enable_if_t<std::is_convertible<T, U>::value>>;
        
        template <typename T, typename Coercion, std::size_t I>
        class generate_concrete_coercions;
        
        template <typename T, typename Coercion>
        class generate_concrete_coercions<T, Coercion, 0>;
        
        template <typename T, typename Coercion>
        class column_model;
    }
}
```

-----
