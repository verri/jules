---
---

# Header file `core/type.hpp`<a id="core/type.hpp"></a>

``` cpp
#define JULES_CORE_TYPE_H

namespace jules
{
    using numeric = double;
    
    using string = std::string;
    
    using uinteger = std::uint32_t;
    
    using index_t = std::size_t;
    
    using integer = std::int64_t;
    
    using distance_t = std::ptrdiff_t;
    
    struct numeric_rule;
    
    struct string_rule;
    
    template <typename ... Rules>
    class base_coercion_rules;
    
    using coercion_rules = base_coercion_rules<numeric_rule, string_rule>;
    
    namespace detail
    {
        struct trivial_tag;
        
        struct non_trivial_tag;
        
        template <typename T, typename = void>
        struct trivial_dispatch_helper;
        
        template <typename T>
        struct trivial_dispatch_helper<T, std::enable_if_t<std::is_trivial<T>::value>>;
        
        template <typename T>
        auto trivial_dispatch();
    }
    
    template <typename T, std::size_t N>
    struct recursive_initializer_list;
    
    template <typename T>
    struct recursive_initializer_list<T, 0>;
    
    template <typename T, std::size_t N>
    using recursive_initializer_list_t = typename recursive_initializer_list<T, N>::type;
    
    struct in_place_t;
    
    constexpr in_place_t{} in_place;
    
    template <typename T>
    struct numeric_traits;
}
```

## Type alias `jules::numeric` \[Basic Types\]<a id="jules::numeric"></a>

``` cpp
using numeric = double;
```

Standard numeric type.

## Type alias `jules::string` \[Basic Types\]<a id="jules::string"></a>

``` cpp
using string = std::string;
```

Standard string type.

It is the C++ standard [std::string](http://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=std::string).

## Type alias `jules::uinteger` \[Basic Types\]<a id="jules::uinteger"></a>

``` cpp
using uinteger = std::uint32_t;
```

Standard unsigned type.

## Type alias `jules::index_t` \[Basic Types\]<a id="jules::index_t"></a>

``` cpp
using index_t = std::size_t;
```

Standard index type.

## Type alias `jules::integer` \[Basic Types\]<a id="jules::integer"></a>

``` cpp
using integer = std::int64_t;
```

Standard signed type.

## Type alias `jules::distance_t` \[Basic Types\]<a id="jules::distance_t"></a>

``` cpp
using distance_t = std::ptrdiff_t;
```

Standard distance type.

## Struct `jules::numeric_rule` \[Coercion Rules\]<a id="jules::numeric_rule"></a>

``` cpp
struct numeric_rule
{
    using type = numeric;
    
    static type coerce_from(const string& value);
};
```

Coercion rules for [numeric type](doc_core__type.html#jules::numeric).

## Struct `jules::string_rule` \[Coercion Rules\]<a id="jules::string_rule"></a>

``` cpp
struct string_rule
{
    using type = std::string;
    
    static type coerce_from(const numeric& value);
};
```

Coercion rules for [string type](doc_core__type.html#jules::string).

## Class template `jules::base_coercion_rules` \[Coercion Rules\]<a id="jules::base_coercion_rules-Rules...-"></a>

``` cpp
template <typename ... Rules>
class base_coercion_rules
{
public:
    template <std::size_t I>
    using type = typename std::tuple_element<I, types>::type;
    
    static constexpr auto type_count();
    
    template <std::size_t I>
    using rule = typename std::tuple_element<I, rules>::type;
    
    static constexpr auto rule_count();
};
```

Utility class to combine coercion rules.

### Alias template `jules::base_coercion_rules::type`<a id="jules::base_coercion_rules-Rules...-::type-I-"></a>

``` cpp
template <std::size_t I>
using type = typename std::tuple_element<I, types>::type;
```

Access the `I`-th type.

### Function `jules::base_coercion_rules::type_count`<a id="jules::base_coercion_rules-Rules...-::type_count()"></a>

``` cpp
static constexpr auto type_count();
```

Retrieves the number of types.

### Alias template `jules::base_coercion_rules::rule`<a id="jules::base_coercion_rules-Rules...-::rule-I-"></a>

``` cpp
template <std::size_t I>
using rule = typename std::tuple_element<I, rules>::type;
```

Access the `I`-th rule.

### Function `jules::base_coercion_rules::rule_count`<a id="jules::base_coercion_rules-Rules...-::rule_count()"></a>

``` cpp
static constexpr auto rule_count();
```

Retrieves the number of rules.

-----

## Type alias `jules::coercion_rules` \[Coercion Rules\]<a id="jules::coercion_rules"></a>

``` cpp
using coercion_rules = base_coercion_rules<numeric_rule, string_rule>;
```

Default class with coercion rules for [numeric](doc_core__type.html#jules::numeric) and [string](doc_core__type.html#jules::string) classes.

-----
