---
---

# Header file `array/blas.hpp`<a id="array/blas.hpp"></a>

``` cpp
#define JULES_ARRAY_BLAS_H

namespace jules
{
    namespace blas
    {
        template <typename T>
        struct invalid;
        
        template <typename T>
        struct cblas;
        
        template <>
        struct cblas<float>;
        
        template <>
        struct cblas<double>;
        
        template <typename T>
        auto safe_int_cast(T value);
        
        template <typename T>
        matrix<T> product(const matrix<T>& lhs, const matrix<T>& rhs);
    }
}
```

-----
