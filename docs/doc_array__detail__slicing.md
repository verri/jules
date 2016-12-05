# Header file `array/detail/slicing.hpp`<a id="array/detail/slicing.hpp"></a>

``` cpp
#define JULES_ARRAY_DETAIL_SLICING_H

#define CHECK_BOUNDS

#define CHECK_STRIDE

namespace jules
{
    namespace detail
    {
        template <std::size_t N>
        struct slice_accessor;
        
        template <>
        struct slice_accessor<1>;
        
        template <std::size_t N>
        auto extent(const base_slice<N>& slice, index_t i = 0u);
        
        template <std::size_t N>
        auto stride(const base_slice<N>& slice, index_t i = 0u);
        
        template <std::size_t N>
        index_t& extent(base_slice<N>& slice, index_t i = 0u);
        
        template <std::size_t N>
        index_t& stride(base_slice<N>& slice, index_t i = 0u);
        
        auto drop(const std::array<index_t, 1>& array);
        
        template <std::size_t N>
        auto drop(const std::array<index_t, N>& array);
        
        std::array<index_t, 1> undrop(index_t value);
        
        template <std::size_t N>
        auto undrop(const std::array<index_t, N>& array);
        
        template <typename T, std::size_t N, std::size_t ... I>
        std::array<T, N+1> cat_helper(const std::array<T, N>& head, const T& tail, std::index_sequence<I...>);
        
        template <typename T, std::size_t N>
        std::array<T, N+1> cat(const std::array<T, N>& head, const T& tail);
        
        auto seq_size(index_t start, index_t stop, index_t step);
        
        template <std::size_t D, std::size_t N>
        index_t slicing_size(const std::array<index_t, N>&);
        
        template <std::size_t D, std::size_t N, typename ... Args>
        index_t slicing_size(const std::array<index_t, N>& extents, index_t, Args&&... args);
        
        template <std::size_t D, std::size_t N, typename ... Args>
        index_t slicing_size(const std::array<index_t, N>& extents, const base_slice<1>& slice, Args&&... args);
        
        template <std::size_t D, std::size_t N, typename Rng, typename ... Args, typename T = range::range_value_t<Rng>, int _concept_requires_101 = 42, typename std::enable_if<(_concept_requires_101==43)||(range::Range<Rng>()), int>::type=0>
        /*
        */>index_t slicing_size(const std::array<index_t, N>& extents, const Rng& rng, Args&&... args);
        
        template <std::size_t D, std::size_t N>
        void do_slice(base_slice<N>&);
        
        template <std::size_t D, std::size_t N, typename ... Args>
        void do_slice(base_slice<N>& result, index_t i, Args&&... args);
        
        template <std::size_t D, std::size_t N, typename ... Args>
        void do_slice(base_slice<N>& result, const base_slice<1>& slice, Args&&... args);
        
        template <std::size_t N, typename ... Args>
        base_slice<N> default_slicing(const base_slice<N>& source, Args&&... args);
        
        template <std::size_t D, std::size_t N>
        void do_slice(std::array<index_t, N>&, std::vector<index_t>& indexes, const base_slice<N>& slice, std::array<index_t, D> ix);
        
        template <std::size_t D, std::size_t N, typename ... Args>
        void do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const base_slice<N>& slice, std::array<index_t, D> ix, index_t i, Args&&... args);
        
        template <std::size_t D, std::size_t N, typename ... Args>
        void do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const base_slice<N>& slice, std::array<index_t, D> ix, const base_slice<1>& rng_base, Args&&... args);
        
        template <std::size_t D, std::size_t N, typename Rng, typename ... Args, typename T = range::range_value_t<Rng>, int _concept_requires_190 = 42, typename std::enable_if<(_concept_requires_190==43)||(range::Range<Rng>()), int>::type=0>
        /*
        */>void do_slice(std::array<index_t, N>& extents, std::vector<index_t>& indexes, const base_slice<N>& slice, std::array<index_t, D> ix, const Rng& rng, Args&&... args);
        
        template <std::size_t N, typename ... Args>
        std::pair<std::array<index_t, N>, std::vector<index_t>> indirect_slicing(const base_slice<N>& slice, Args&&... args);
    }
}
```

-----
