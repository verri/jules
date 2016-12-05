# Header file `array/unary_expr_array.hpp`<a id="array/unary_expr_array.hpp"></a>

``` cpp
#define JULES_ARRAY_UNARY_EXPR_ARRAY_H

namespace jules
{
    template <typename It, typename Op, std::size_t N>
    class unary_expr_array;
    
    template <typename It, typename Op, std::size_t N>
    base_array<typename unary_expr_array<It, Op, N>::value_type, N> eval(const unary_expr_array<It, Op, N>& source);
}
```
