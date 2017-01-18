---
---

  - ## Arithmetic
    
      - [jules::length(Iter, Sent)](doc_base__numeric.html#jules::length(Iter,Sent) "jules::length(Iter, Sent)") - Returns either the length of a `Range` or the distance of a pair of iterators.
    
      - [jules::length(const Rng &)](doc_base__numeric.html#jules::length(Iter,Sent) "jules::length(const Rng &)") - Returns either the length of a `Range` or the distance of a pair of iterators.
    
      - [jules::max(Iter, Sent, T)](doc_base__numeric.html#jules::max(Iter,Sent,T) "jules::max(Iter, Sent, T)") - Returns either the maximum element in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::max(const Rng &, T)](doc_base__numeric.html#jules::max(Iter,Sent,T) "jules::max(const Rng &, T)") - Returns either the maximum element in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::min(Iter, Sent, T)](doc_base__numeric.html#jules::min(Iter,Sent,T) "jules::min(Iter, Sent, T)") - Returns either the minimum element in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::min(const Rng &, T)](doc_base__numeric.html#jules::min(Iter,Sent,T) "jules::min(const Rng &, T)") - Returns either the minimum element in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::prod(Iter, Sent, T)](doc_base__numeric.html#jules::prod(Iter,Sent,T) "jules::prod(Iter, Sent, T)") - Returns either the product of the elements in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::prod(const Rng &, T)](doc_base__numeric.html#jules::prod(Iter,Sent,T) "jules::prod(const Rng &, T)") - Returns either the product of the elements in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::repeat(const T &)](doc_base__numeric.html#jules::repeat(constT&) "jules::repeat(const T &)") - Repeats `value` `N` times.
    
      - [jules::repeat(index\_t, const T &)](doc_base__numeric.html#jules::repeat(constT&) "jules::repeat(index_t, const T &)") - Repeats `value` `N` times.
    
      - [jules::sum(Iter, Sent, T)](doc_base__numeric.html#jules::sum(Iter,Sent,T) "jules::sum(Iter, Sent, T)") - Returns either the summation of the elements in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::sum(const Rng &, T)](doc_base__numeric.html#jules::sum(Iter,Sent,T) "jules::sum(const Rng &, T)") - Returns either the summation of the elements in a `Range` or in the sequence \[`first`, `last`).

  - ## Array Types
    
      - [jules::base\_array\<T, 1\>](doc_array__array.html#jules::base_array-T,1- "jules::base_array\<T, 1\>") - 1-Dimensional concrete array.
    
      - [jules::base\_array](doc_array__array.html#jules::base_array-T,N- "jules::base_array\<T, N\>") - N-Dimensional concrete array.
    
      - [jules::base\_slice\<1\>](doc_array__slice.html#jules::base_slice-1- "jules::base_slice\<1\>") - 1D-Array specialization for slicing and dimensions representation.
    
      - [jules::base\_slice](doc_array__slice.html#jules::base_slice-N- "jules::base_slice\<N\>") - Array slicing and dimensions representation.
    
      - [jules::ind\_array\<T, 1\>](doc_array__ind_array.html#jules::ind_array-T,1- "jules::ind_array\<T, 1\>") - 1-D Indirect array specialization.
    
      - [jules::ind\_array](doc_array__ind_array.html#jules::ind_array-T,N- "jules::ind_array\<T, N\>") - Array indirect reference.
    
      - [jules::ref\_array\<T, 1\>](doc_array__ref_array.html#jules::ref_array-T,1- "jules::ref_array\<T, 1\>") - 1-D Array reference specialization.
    
      - [jules::ref\_array](doc_array__ref_array.html#jules::ref_array-T,N- "jules::ref_array\<T, N\>") - Array reference.

  - ## Basic Types
    
      - [jules::distance\_t](doc_core__type.html#jules::distance_t "jules::distance_t") - Standard distance type.
    
      - [jules::index\_t](doc_core__type.html#jules::index_t "jules::index_t") - Standard index type.
    
      - [jules::integer](doc_core__type.html#jules::integer "jules::integer") - Standard signed type.
    
      - [jules::numeric](doc_core__type.html#jules::numeric "jules::numeric") - Standard numeric type.
    
      - [jules::string](doc_core__type.html#jules::string "jules::string") - Standard string type.
    
      - [jules::uinteger](doc_core__type.html#jules::uinteger "jules::uinteger") - Standard unsigned type.

  - ## Coercion Rules
    
      - [jules::base\_coercion\_rules](doc_core__type.html#jules::base_coercion_rules-Rules...- "jules::base_coercion_rules\<Rules...\>") - Utility class to combine coercion rules.
    
      - [jules::coercion\_rules](doc_core__type.html#jules::coercion_rules "jules::coercion_rules") - Default class with coercion rules for [numeric](doc_core__type.html#jules::numeric) and [string](doc_core__type.html#jules::string) classes.
    
      - [jules::numeric\_rule](doc_core__type.html#jules::numeric_rule "jules::numeric_rule") - Coercion rules for [numeric type](doc_core__type.html#jules::numeric).
    
      - [jules::string\_rule](doc_core__type.html#jules::string_rule "jules::string_rule") - Coercion rules for [string type](doc_core__type.html#jules::string).

  - ## Logical
    
      - [jules::all(Iter, Sent)](doc_base__numeric.html#jules::all(Iter,Sent) "jules::all(Iter, Sent)") - Checks whether all elements in a `Range` or in the sequence \[`first`, `last`) are true.
    
      - [jules::all(const Rng &)](doc_base__numeric.html#jules::all(Iter,Sent) "jules::all(const Rng &)") - Checks whether all elements in a `Range` or in the sequence \[`first`, `last`) are true.
    
      - [jules::any(Iter, Sent)](doc_base__numeric.html#jules::any(Iter,Sent) "jules::any(Iter, Sent)") - Checks whether any of the elements in a `Range` or in the sequence \[`first`, `last`) are true.
    
      - [jules::any(const Rng &)](doc_base__numeric.html#jules::any(Iter,Sent) "jules::any(const Rng &)") - Checks whether any of the elements in a `Range` or in the sequence \[`first`, `last`) are true.
    
      - [jules::count(Iter, Sent)](doc_base__numeric.html#jules::count(Iter,Sent) "jules::count(Iter, Sent)") - Returns the number of true elements in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::count(const Rng &)](doc_base__numeric.html#jules::count(Iter,Sent) "jules::count(const Rng &)") - Returns the number of true elements in a `Range` or in the sequence \[`first`, `last`).
    
      - [jules::none(Iter, Sent)](doc_base__numeric.html#jules::none(Iter,Sent) "jules::none(Iter, Sent)") - Checks whether all elements in a `Range` or in the sequence \[`first`, `last`) are false.
    
      - [jules::none(const Rng &)](doc_base__numeric.html#jules::none(Iter,Sent) "jules::none(const Rng &)") - Checks whether all elements in a `Range` or in the sequence \[`first`, `last`) are false.
