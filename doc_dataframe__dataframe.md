---
---

# Header file `dataframe/dataframe.hpp`<a id="dataframe/dataframe.hpp"></a>

``` cpp
#define JULES_DATAFRAME_DATAFRAME_H

namespace jules
{
    template <typename Coercion>
    class base_dataframe;
    
    using dataframe = base_dataframe<coercion_rules>;
    
    template <typename Coercion>
    std::ostream& operator<<(std::ostream& os, const base_dataframe<Coercion>& df);
    
    template <typename Coercion>
    std::istream& operator>>(std::istream& is, base_dataframe<Coercion>& df);
}
```
