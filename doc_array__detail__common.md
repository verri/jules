---
---

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
    struct Array;
    
    template <typename T, std::size_t N>
    struct Array<base_array<T, N>>;
    
    template <typename T, std::size_t N>
    struct Array<contiguous_array<T, N>>;
    
    template <typename T, std::size_t N>
    struct Array<ref_array<T, N>>;
    
    template <typename T, std::size_t N>
    struct Array<ind_array<T, N>>;
    
    template <typename It, typename F, std::size_t N>
    struct Array<unary_expr_array<It, F, N>>;
    
    template <typename LhsIt, typename RhsIt, typename F, std::size_t N>
    struct Array<binary_expr_array<LhsIt, RhsIt, F, N>>;
    
    template <std::size_t M, typename It, typename F>
    unary_expr_array<It, std::decay_t<F>, M> make_expr_array(It first, It last, F&& f, typename base_slice<M>::extent_type extent);
    
    template <std::size_t M, typename LhsIt, typename RhsIt, typename F>
    binary_expr_array<LhsIt, RhsIt, std::decay_t<F>, M> make_expr_array(LhsIt lhs_first, LhsIt lhs_last, RhsIt rhs_first, RhsIt rhs_last, F&& f, typename base_slice<M>::extent_type extent);
}
```

-----
