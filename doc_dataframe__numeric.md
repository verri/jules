---
---

# Header file `dataframe/numeric.hpp`<a id="dataframe/numeric.hpp"></a>

``` cpp
#define JULES_DATAFRAME_NUMERIC_H

namespace jules
{
    template <typename T, typename C>
    base_column<C> to_column(const base_column<C>& column);
    
    template <typename T, typename C>
    base_column<C> to_column(base_column<C>&& column);
    
    template <typename T, typename C>
    contiguous_array<T, 1> to_view(base_column<C>& column);
    
    template <typename T, typename C>
    contiguous_array<const T, 1> to_view(const base_column<C>& column);
    
    template <typename T, typename C>
    auto to_view(const base_column<C>&& column) = delete;
    
    template <typename T, typename C>
    vector<T> to_vector(const base_column<C>& column);
}
```
