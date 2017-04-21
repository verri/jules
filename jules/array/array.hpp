// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_ARRAY_H
/// \exclude
#define JULES_ARRAY_ARRAY_H

#include <jules/array/detail/allocator.hpp>
#include <jules/array/detail/common.hpp>
#include <jules/array/ref_array.hpp>
#include <jules/base/async.hpp>
#include <jules/base/const_vector.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/meta.hpp>
#include <jules/core/type.hpp>

#include <type_traits>

/// TODO: XXΧ: There are no strong guarantees if an exceptions occurs. It possibly will
/// cause a memory leak.
/// TODO: Everytime a Range is given, the size must be known a priori.  A better error
/// message is needed.

namespace jules
{

/// N-Dimensional concrete array.
///
/// Basic data-type to represent arrays with 2 or more dimensions.
/// (1-D array is specialized, see [jules::base_array<T, 1>]().)
///
/// Most of the operations are implemented in its base class,
/// consult [jules::ref_array<T, N>]().
///
/// \module Array Types
/// \notes It is recommended to use the alias [jules::matrix]() for 2-dimensional structures
///   and [jules::ndarray]() for higher dimensions.
/// \notes Elements are contiguously stored in column-major order.
template <typename T, std::size_t N> class base_array : public ref_array<T, N>, private detail::array_allocator<T>
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class base_array;

public:
  /// \group member_types Class Types and Constants
  ///
  /// (1) The order of dimensionality.
  ///
  /// (2) The type of the elements.
  ///
  /// (3) `RandomAccessIterator` over contiguous elements.
  ///
  /// (4) Constant `RandomAccessIterator` over contiguous elements.
  ///
  /// (5) Unsigned integer type that can store the dimensions of the array.
  ///
  /// (6) Signed integer type that can store differences between sizes.
  static constexpr auto order = N;

  /// \group member_types
  using value_type = T;

  /// \group member_types
  using iterator = value_type*;

  /// \group member_types
  using const_iterator = const value_type*;

  /// \group member_types
  using size_type = index_t;

  /// \group member_types
  using difference_type = distance_t;

  /// \group destructor Destructor
  ~base_array() { clear(this->data(), this->size()); }

  /// \group constructors Constructors
  /// Constructs a new array from a variety of data sources.
  ///
  /// (1) Default constructor. Constructs an empty array.
  //
  /// (2) Explicit construction with dimensions `dims`. Elements are **uninitialized**.
  ///
  /// (3) Explicit constructor with dimensions `dims`.  Elements are value-initialized.
  ///
  /// (4) Constructs the array with copies of elements with value `value` and dimensions `dims`.
  ///
  /// (5) Constructs the array with dimensions `dims`.  Elements are initialized by iterating `iter`.
  ///
  /// (6) Constructs the array from a `recursive_initialized_list`.
  ///
  /// (7) Copy constructor.
  ///
  /// (8) Move constructor.
  ///
  /// (9) Converting constructor.  Constructs a array from other array-like structures,
  ///   e.g. expression arrays or sliced arrays.
  ///
  /// \requires `Iter` shall be `OutputIterator`
  /// \requires `A` shall be `Array`
  /// \requires `sizeof...(Dims)` shall be `order`
  ///
  /// \notes (5) Dimensions are inferred from the list, so it must have the same number of
  ///   elements in each dimension.
  base_array() : ref_array<value_type, order>{nullptr, {}} {}

  /// \group constructors
  /// \tparam _
  ///   \exclude
  template <typename... Dims, typename _ = detail::n_indexes_enabler<order, Dims...>>
  explicit base_array(uninitialized_t, Dims... dims)
    : ref_array<value_type, order>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
  }

  /// \group constructors
  /// \tparam _
  ///   \exclude
  template <typename... Dims, typename _ = detail::n_indexes_enabler<order, Dims...>>
  explicit base_array(Dims... dims) : base_array(uninitialized, dims...)
  {
    this->create(this->data(), this->size());
  }

  /// \group constructors
  /// \tparam _
  ///   \exclude
  template <typename... Dims, typename _ = detail::n_indexes_enabler<order, Dims...>>
  base_array(const value_type& value, Dims... dims) : base_array(uninitialized, dims...)
  {
    this->create(this->data(), this->size(), value);
  }

  /// \group constructors
  /// \tparam R
  ///   \exclude
  /// \tparam _1
  ///   \exclude
  /// \tparam _2
  ///   \exclude
  template <typename Iter, typename... Dims, typename R = range::iterator_value_t<Iter>,
            typename _1 = detail::n_indexes_enabler<order, Dims...>, typename _2 = meta::requires<range::Iterator<Iter>>>
  base_array(Iter iter, Dims... dims) : base_array(uninitialized, dims...)
  {
    static_assert(std::is_convertible<R, value_type>::value, "iterator values are not compatible");
    this->create(this->data(), iter, this->size());
  }

  /// \group constructors
  base_array(recursive_initializer_list_t<value_type, order> values)
  {
    this->descriptor_ = this->calculate_descriptor(values);
    this->data_ = this->allocate(this->descriptor_.size());
    this->create(this->data(), values, this->descriptor_);
  }

  /// \group constructors
  base_array(const base_array& source) : ref_array<value_type, order>{this->allocate(source.size()), source.descriptor_}
  {
    this->create(this->data(), source.data(), this->size());
  }

  /// \group constructors
  base_array(base_array&& source) noexcept : ref_array<value_type, order>{move_ptr(source.data_), std::move(source.descriptor_)}
  {
  }

  /// \group constructors
  /// \tparam _
  ///   \exclude
  template <typename A, typename _ = meta::requires<Array<A>>>
  base_array(const A& source) : ref_array<value_type, order>{this->allocate(source.size()), {0u, source.extents()}}
  {
    static_assert(A::order == order, "array order mismatch");
    static_assert(std::is_constructible<value_type, const typename A::value_type&>::value, "incompatible value types");
    this->create(this->data(), source.begin(), this->size());
  }

  /// \group assignment Assignment
  ///
  /// (1) Copy assignment.
  ///
  /// (2) Move assignment.
  ///
  /// (3) Assignment from array-like structures.
  ///
  /// \requires `A` shall be `Array`
  auto operator=(const base_array& source) -> base_array&
  {
    DEBUG_ASSERT(this != &source, debug::default_module, debug::level::invalid_argument, "self assignment");
    clear(this->data(), this->size());
    this->data_ = this->allocate(source.size());
    this->descriptor_ = source.descriptor_;
    this->create(this->data(), source.data(), source.size());
    return *this;
  }

  /// \group assignment
  auto operator=(base_array&& source) noexcept -> base_array&
  {
    DEBUG_ASSERT(this != &source, debug::default_module, debug::level::invalid_argument, "self assignment");
    clear(this->data(), this->size());
    this->data_ = move_ptr(source.data_);
    this->descriptor_ = std::move(source.descriptor_);
    return *this;
  }

  /// \group assignment
  /// \synopsis_return base_array&
  template <typename A> auto operator=(const A& source) -> meta::requires_t<base_array&, Array<A>>
  {
    static_assert(A::order == order, "array order mismatch");
    static_assert(std::is_assignable<value_type&, typename A::value_type>::value, "incompatible assignment");

    auto old_data = this->data();
    auto old_size = this->size();

    this->data_ = this->allocate(source.size());
    this->descriptor_ = {0, source.extents()};
    this->create(this->data(), source.begin(), source.size());

    clear(old_data, old_size);

    return *this;
  }

  /// \group Fill
  /// Fills the array.
  auto fill(const value_type& value) -> void
  {
    for (auto& elem : *this)
      elem = value;
  }

  /// \group Fill
  template <typename... Args> auto fill(in_place_t, Args&&... args) -> void
  {
    DEBUG_ASSERT(this->data(), debug::default_module, debug::level::invalid_state, "array is empty");
    this->destroy(this->data(), this->size());
    this->create(this->data(), this->size(), std::forward<Args>(args)...);
  }

  /// \group Begin
  /// Returns an iterator to the first element of the array.
  auto begin() -> iterator { return this->data(); }

  /// \group End
  /// Returns an iterator to the element past the last element of the container.
  auto end() -> iterator { return this->data() + this->size(); }

  /// \group Begin
  auto begin() const -> const_iterator { return this->cbegin(); }

  /// \group End
  auto end() const -> const_iterator { return this->cend(); }

  /// \group Begin
  auto cbegin() const -> const_iterator { return this->data(); }

  /// \group End
  auto cend() const -> const_iterator { return this->data() + this->size(); }

  /// \group Data
  /// Returns pointer to the underlying array serving as element storage.
  auto data() -> value_type* { return this->data_; }

  /// \group Data
  auto data() const -> const value_type* { return this->data_; }

private:
  /// \exclude
  static auto clear(value_type* data, index_t size)
  {
    if (data) {
      base_array::destroy(data, size);
      base_array::deallocate(data, size);
    }
  }

  /// \exclude
  static auto calculate_descriptor(recursive_initializer_list_t<value_type, order> values) -> base_slice<order>
  {
    std::array<index_t, order> extents;

    calculate_descriptor_fill(extents, values, 0u);
    DEBUG_ASSERT(calculate_descriptor_check(extents, values, 0u), debug::default_module, debug::level::boundary_check,
                 "invalid initializer");

    return {0u, extents};
  }

  /// \exclude
  template <typename List> static auto calculate_descriptor_fill(std::array<index_t, order>& extents, List values, index_t pos)
  {
    DEBUG_ASSERT(values.size() > 0, debug::default_module, debug::level::boundary_check, "invalid initializer");
    extents[pos] = values.size();
    calculate_descriptor_fill(extents, *values.begin(), pos + 1);
  }

  /// \exclude
  static auto calculate_descriptor_fill(std::array<index_t, order>&, const value_type&, index_t) {}

  /// \exclude
  template <typename List>
  static auto calculate_descriptor_check(const std::array<index_t, order>& extents, List values, index_t pos)
  {
    if (values.size() != extents[pos])
      return false;

    for (auto subvalues : values)
      if (!calculate_descriptor_check(extents, subvalues, pos + 1))
        return false;

    return true;
  }

  /// \exclude
  static constexpr auto calculate_descriptor_check(const std::array<index_t, order>&, const value_type&, index_t) { return true; }
};

/// 1-Dimensional concrete array.
///
/// Basic data-type to represent arrays with 1 dimension.
/// For more operations over this class, consult the base class [jules::ref_array<T, 1>]().
///
/// \module Array Types
/// \notes Using the aliasing [jules::vector]() is recommended.
/// \notes Elements are contiguously stored.
template <typename T> class base_array<T, 1> : public ref_array<T, 1>, private detail::array_allocator<T>
{
  template <typename, std::size_t> friend class base_array;

public:
  /// \group member_types Class Types and Constants
  ///
  /// (1) The order of dimensionality.
  ///
  /// (2) The type of the elements.
  ///
  /// (3) `RandomAccessIterator` over contiguous elements.
  ///
  /// (4) Constant `RandomAccessIterator` over contiguous elements.
  ///
  /// (5) Unsigned integer type that can store the dimensions of the array.
  ///
  /// (6) Signed integer type that can store differences between sizes.
  static constexpr auto order = std::size_t{1u};

  /// \group member_types Class Types and Constants
  using value_type = T;

  /// \group member_types Class Types and Constants
  using iterator = value_type*;

  /// \group member_types
  using const_iterator = const value_type*;

  /// \group member_types
  using size_type = index_t;

  /// \group member_types
  using difference_type = distance_t;

  ~base_array() { clear(this->data(), this->size()); }

  /// \group constructors Constructors
  /// Constructs a new vector from a variety of data sources.
  ///
  /// (1) Default constructor. Constructs an empty vector.
  ///
  /// (2) Explicit construction with size `length`. Elements are **uninitialized**.
  ///
  /// (3) Explicit constructor with size `length`.  Elements are value-initialized.
  ///
  /// (4) Constructs the vector with copies of elements with value `value` and length `length`.
  ///
  /// (5) Constructs the container with the contents of the range [`first`, `last`).
  ///
  /// (6) Constructs the vector from a `initialized_list`.
  ///
  /// (7) Constructs the vector from a range.
  ///
  /// (8) Copy constructor.
  ///
  /// (9) Move constructor.
  ///
  /// (10) Converting constructor.  Constructs a vector from other vector-like structures,
  ///   e.g. expression arrays or sliced arrays.
  ///
  /// \requires `Sent` shall be `Sentinel<Iter>`
  /// \requires `Rng` shall be `Range` but not `Array`
  /// \requires `A` shall be `Array`
  base_array() : ref_array<value_type, 1>{nullptr, {}} {}

  /// \group constructors
  explicit base_array(uninitialized_t, index_t length) : ref_array<value_type, 1>{this->allocate(length), {0u, length}} {}

  /// \group constructors
  explicit base_array(index_t length) : base_array(uninitialized, length) { this->create(this->data(), this->size()); }

  /// \group constructors
  base_array(const value_type& value, index_t length) : base_array(uninitialized, length)
  {
    this->create(this->data(), this->size(), value);
  }

  /// \group constructors
  /// \tparam U
  ///   \exclude
  /// \tparam _
  ///   \exclude
  template <typename Iter, typename Sent, typename U = range::iterator_value_t<Iter>,
            typename _ = meta::requires<range::Sentinel<Sent, Iter>>>
  base_array(Iter first, Sent last)
    : ref_array<value_type, 1>{this->allocate(range::distance(first, last)),
                               {0u, static_cast<index_t>(range::distance(first, last))}}
  {
    static_assert(std::is_constructible<value_type, const U&>::value, "incompatible value types");
    this->create(this->data(), first, this->size());
  }

  /// \group constructors
  base_array(std::initializer_list<value_type> values) : base_array(values.begin(), values.end()) {}

  /// \group constructors
  /// \tparam U
  ///   \exclude
  /// \tparam _
  ///   \exclude
  template <typename Rng, typename U = range::range_value_t<std::decay_t<Rng>>,
            typename _ = meta::requires<range::Range<std::decay_t<Rng>>, meta::negation<Array<std::decay_t<Rng>>>>>
  base_array(Rng&& rng) : base_array(uninitialized, range::size(rng))
  {
    static_assert(std::is_constructible<value_type, const U&>::value, "incompatible value types");
    // TODO: C++17
    // if contexpr (std::is_rvalue_v<Rng>)
    //   this->create(this->data(), range::make_move_iterator(range::begin(rng)), this->size());
    // else
    this->create(this->data(), range::begin(rng), this->size());
  }

  /// \group constructors Constructors
  base_array(const base_array& source) : ref_array<value_type, 1>{this->allocate(source.size()), source.descriptor_}
  {
    this->create(this->data(), source.data(), this->size());
  }

  /// \group constructors Constructors
  base_array(base_array&& source) noexcept : ref_array<value_type, 1>{move_ptr(source.data_), std::move(source.descriptor_)} {}

  /// \group constructors Constructors
  /// \tparam _
  ///   \exclude
  template <typename A, typename _ = meta::requires<Array<A>>>
  base_array(const A& source) : ref_array<value_type, 1>{this->allocate(source.size()), {0u, source.extents()}}
  {
    static_assert(A::order == 1, "array order mismatch");
    static_assert(std::is_constructible<value_type, const typename A::value_type&>::value, "incompatible value types");
    this->create(this->data(), source.begin(), this->size());
  }

  /// \group assignment Assignment
  ///
  /// (1) Copy assignment.
  ///
  /// (2) Move assignment.
  ///
  /// (3) Assignment from array-like structures.
  ///
  /// \requires `A` shall be `Array`
  auto operator=(const base_array& source) & -> base_array&
  {
    DEBUG_ASSERT(this != &source, debug::default_module, debug::level::invalid_argument, "self assignment");
    clear(this->data(), this->size());
    this->data_ = this->allocate(source.size());
    this->descriptor_ = source.descriptor_;
    this->create(this->data(), source.data(), source.size());
    return *this;
  }

  /// \group assignment
  auto operator=(base_array&& source) & noexcept -> base_array&
  {
    DEBUG_ASSERT(this != &source, debug::default_module, debug::level::invalid_argument, "self assignment");
    clear(this->data(), this->size());
    this->data_ = move_ptr(source.data_);
    this->descriptor_ = std::move(source.descriptor_);
    return *this;
  }

  /// \group assignment
  /// \tparam _
  ///   \exclude
  template <typename A, typename _ = meta::requires<Array<A>>> auto operator=(const A& source) & -> base_array&
  {
    static_assert(A::order == 1, "array order mismatch");
    static_assert(std::is_assignable<value_type&, typename A::value_type>::value, "incompatible assignment");

    auto old_data = this->data();
    auto old_size = this->size();

    this->data_ = this->allocate(source.size());
    this->descriptor_ = {0u, source.extents()};
    this->create(this->data(), source.begin(), source.size());

    clear(old_data, old_size);

    return *this;
  }

  /// \group Fill
  /// Fills the array.
  auto fill(const value_type& value) -> void
  {
    for (auto& elem : *this)
      elem = value;
  }

  /// \group Fill
  template <typename... Args> auto fill(in_place_t, Args&&... args) -> void
  {
    DEBUG_ASSERT(this->data(), debug::default_module, debug::level::invalid_state, "array is empty");
    this->destroy(this->data(), this->size());
    this->create(this->data(), this->size(), std::forward<Args>(args)...);
  }

  /// \group Indexing
  /// Optimized indirect indexing from temporary indexes.
  auto operator()(const_vector<index_t> indexes) -> ind_array<value_type, 1>
  {
    return {this->data_, indexes.size(), std::move(indexes)};
  }

  /// \group Indexing
  auto operator()(const_vector<index_t> indexes) const -> ind_array<const value_type, 1>
  {
    return {this->data_, indexes.size(), std::move(indexes)};
  }

  /// \exclude
  using ref_array<value_type, 1>::operator();

  /// \group Begin
  /// Returns an iterator to the first element of the array.
  auto begin() -> iterator { return this->data(); }

  /// \group End
  /// Returns an iterator to the element past the last element of the container.
  auto end() -> iterator { return this->data() + this->size(); }

  /// \group Begin
  auto begin() const -> const_iterator { return this->cbegin(); }

  /// \group End
  auto end() const -> const_iterator { return this->cend(); }

  /// \group Begin
  auto cbegin() const -> const_iterator { return this->data(); }

  /// \group End
  auto cend() const -> const_iterator { return this->data() + this->size(); }

  /// \group Data
  /// Returns pointer to the underlying array serving as element storage.
  auto data() -> value_type* { return this->data_; }

  /// \group Data
  auto data() const -> const value_type* { return this->data_; }

private:
  /// \exclude
  static auto clear(value_type* data, index_t size)
  {
    if (data) {
      base_array::destroy(data, size);
      base_array::deallocate(data, size);
    }
  }
};

/// \group array_alias Array Aliasing
/// (1) N-Dimensional array aliasing.
///
/// (2) Matrix aliasing.
///
/// (3) Vector aliasing.
template <std::size_t N, typename T = jules::numeric> using ndarray = base_array<T, N>;

/// \group array_alias
template <typename T = jules::numeric> using matrix = base_array<T, 2>;

/// \group array_alias
template <typename T = jules::numeric> using vector = base_array<T, 1>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
