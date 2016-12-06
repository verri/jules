---
---

# Header file `array/binary_expr_array.hpp`<a id="array/binary_expr_array.hpp"></a>

``` cpp
#define JULES_ARRAY_BINARY_EXPR_ARRAY_H

namespace jules
{
    template <typename LhsIt, typename RhsIt, typename Op, size_t N>
    class binary_expr_array;
    
    template <typename Lhs, typename Rhs, typename Op, std::size_t N>
    base_array<typename binary_expr_array<Lhs, Rhs, Op, N>::value_type, N> eval(const binary_expr_array<Lhs, Rhs, Op, N>& source);
}
```
