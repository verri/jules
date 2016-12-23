---
---

# Header file `array/io.hpp`<a id="array/io.hpp"></a>

``` cpp
#define JULES_ARRAY_IO_H

template <typename T>
std::ostream& operator<<(std::ostream& os, const jules::ref_array<T, 1>& array);

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const jules::ref_array<T, N>& array);
```
