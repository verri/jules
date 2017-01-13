---
---

# Header file `dataframe/column.hpp`<a id="dataframe/column.hpp"></a>

``` cpp
#define JULES_DATAFRAME_COLUMN_H

namespace jules
{
    template <typename T, typename C>
    base_column<C> to_column(const base_column<C>& column);
    
    template <typename T, typename C>
    base_column<C> to_column(base_column<C>&& column);
    
    template <typename Coercion>
    class base_column;
    
    using column = base_column<coercion_rules>;
}
```
