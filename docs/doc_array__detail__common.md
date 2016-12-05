# Header file `array/detail/common.hpp`<a id="array/detail/common.hpp"></a>

``` cpp
#define JULES_ARRAY_DETAIL_COMMON_H

namespace jules
{
    namespace detail
    {
        template <std::size_t N, typename ... Types>
        using n_indexes_enabler = std::enable_if_t<N==sizeof...(Types)&&all_args(std::is_convertible<Types, uint>::value...)>;
        
        template <typename T>
        constexpr auto index_or_slice();
        
        template <typename Return, typename ... Args>
        using element_request = std::enable_if_t<all_args(std::is_convertible<Args, uint>::value...), Return>;
        
        template <typename Return, typename ... Args>
        using slice_request = std::enable_if_t<all_args(index_or_slice<Args>()...)&&!all_args(std::is_convertible<Args, uint>::value...), Return>;
        
        template <typename Return, typename ... Args>
        using indirect_request = std::enable_if_t<!all_args(index_or_slice<Args>()...), Return>;
    }
    
    template <typename T>
    struct is_array;
    
    template <typename T, std::size_t N>
    struct is_array<base_array<T, N>>;
    
    template <typename T, std::size_t N>
    struct is_array<ref_array<T, N>>;
    
    template <typename T, std::size_t N>
    struct is_array<ind_array<T, N>>;
    
    template <typename It, typename F, std::size_t N>
    struct is_array<unary_expr_array<It, F, N>>;
    
    template <typename LhsIt, typename RhsIt, typename F, std::size_t N>
    struct is_array<binary_expr_array<LhsIt, RhsIt, F, N>>;
    
    template <typename T>
    constexpr auto is_array_v();
    
    template <typename R, typename T>
    using array_request = std::enable_if_t<is_array_v<T>(), R>;
    
    template <typename R, typename T>
    using array_fallback = std::enable_if_t<!is_array_v<T>(), R>;
    
    template <std::size_t M, typename It, typename F>
    unary_expr_array<It, F, M> make_expr_array(It first, It last, const F& f, typename base_slice<M>::extent_type extent);
    
    template <std::size_t M, typename LhsIt, typename RhsIt, typename F>
    binary_expr_array<LhsIt, RhsIt, F, M> make_expr_array(LhsIt lhs_first, LhsIt lhs_last, RhsIt rhs_first, RhsIt rhs_last, const F& f, typename base_slice<M>::extent_type extent);
}
```

-----
