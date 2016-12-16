// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include <jules/array/detail/allocator.hpp>
#include <jules/array/detail/common.hpp>
#include <jules/array/ref_array.hpp>
#include <jules/base/async.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/type.hpp>

#include <type_traits>

/// TODO: XXΧ: There are no strong guarantees if an exceptions occurs. It possibly will
/// cause a memory leak.

namespace jules
{

/// N-Dimensional concrete array.
///
/// Basic data-type to represent arrays with 2 or more dimensions.
/// For more operations over this class, consult [jules::ref_array<T, N>]().
///
/// \module N-Dimensional Array
/// \notes Using the aliasing [jules::matrix]() for 2D structures and [jules::ndarray]()
/// for higher dimensions is recommended to keep portability between versions.
/// \notes Elements are contiguously stored in column-major order.
template <typename T, std::size_t N> class base_array : public ref_array<T, N>, private detail::array_allocator<T>
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class base_array;

public:
  /// The type of the elements.
  using value_type = T;

  /// The order of dimensionality.
  static constexpr auto order = N;

  /// `RandomAccessIterator` over contiguous elements.
  using iterator = T*;

  /// Constant `RandomAccessIterator` over contiguous elements.
  using const_iterator = const T*;

  /// Unsigned integer type that can store the dimensions of the array.
  using size_type = index_t;

  /// Signed integer type that can store differences between sizes.
  using difference_type = distance_t;

  ~base_array() { clear(this->data(), this->size()); }

  /// \group Constructors
  /// Constructs a new array from a variety of data sources.
  ///
  /// 1) Default constructor. Constructs an empty array.
  ///
  /// 2) Explicit constructor with dimensions `dims`.  Elements are default initialized.
  ///
  /// 3) Constructs the array with copies of elements with value `value` and dimensions `dims`.
  ///
  /// 4) Constructs the array with dimensions `dims`.  Elements are initialized with
  ///  elements from iterating `iter`.
  ///
  /// 5) Constructs the array from a `recursive_initialized_list`.
  ///
  /// 6) Copy constructor.
  ///
  /// 7) Move constructor.
  ///
  /// 8) Converting constructor.  Constructs a array from other array-like structures,
  ///   e.g. expression arrays or sliced arrays.
  ///
  /// \notes (2-4) For constructors that receive the array dimensions, the number of arguments
  ///   must be exactly the dimensionality [jules::base_array<T, N>::order]().
  /// \notes (5) Dimensions are inferred from the list, so it must have the same number of
  ///   elements in each dimension.
  base_array() : ref_array<T, N>{nullptr, {}} {}

  /// \group Constructors
  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
  explicit base_array(Dims... dims) : ref_array<T, N>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), this->size());
  }

  /// \group Constructors
  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
  base_array(const T& value, Dims... dims) : ref_array<T, N>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), this->size(), value);
  }

  /// \group Constructors
  template <typename Iter, typename... Dims, typename R = range::iterator_value_t<Iter>,
            typename = detail::n_indexes_enabler<N, Dims...>>
  base_array(Iter iter, Dims... dims) : ref_array<T, N>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
    static_assert(std::is_convertible<R, T>::value, "iterator values are not compatible");
    this->create(detail::trivial_dispatch<T>(), this->data(), iter, this->size());
  }

  /// \group Constructors
  base_array(recursive_initializer_list_t<T, N> values)
  {
    this->descriptor_ = this->calculate_descriptor(values);
    this->data_ = this->allocate(this->descriptor_.size());
    this->create(detail::trivial_dispatch<T>(), this->data(), values, this->descriptor_);
  }

  /// \group Constructors
  base_array(const base_array& source) : ref_array<T, N>{this->allocate(source.size()), source.descriptor_}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), source.data(), this->size());
  }

  /// \group Constructors
  base_array(base_array&& source) noexcept : ref_array<T, N>{move_ptr(source.data_), std::move(source.descriptor_)} {}

  /// \group Constructors
  template <typename Array, typename = array_request<void, Array>>
  base_array(const Array& source) : ref_array<T, N>{this->allocate(source.size()), {0u, source.extents()}}
  {
    static_assert(Array::order == N, "array order mismatch");
    static_assert(std::is_constructible<T, const typename Array::value_type&>::value, "incompatible value types");
    this->create(detail::trivial_dispatch<T>(), this->data(), source.begin(), this->size());
  }

  /// \group Assignment
  ///
  /// 1) Copy assignment.
  ///
  /// 2) Move assignment.
  ///
  /// 3) Assignment from array-like structures.
  auto operator=(const base_array& source) -> base_array&
  {
    DEBUG_ASSERT(this != &source, debug::module{}, debug::level::invalid_argument, "self assignment");
    clear(this->data(), this->size());
    this->data_ = this->allocate(source.size());
    this->descriptor_ = source.descriptor_;
    this->create(detail::trivial_dispatch<T>(), this->data(), source.data(), source.size());
    return *this;
  }

  /// \group Assignment
  auto operator=(base_array&& source) noexcept -> base_array&
  {
    DEBUG_ASSERT(this != &source, debug::module{}, debug::level::invalid_argument, "self assignment");
    clear(this->data(), this->size());
    this->data_ = move_ptr(source.data_);
    this->descriptor_ = std::move(source.descriptor_);
    return *this;
  }

  /// \group Assignment
  template <typename Array> auto operator=(const Array& source) -> array_request<base_array&, Array>
  {
    static_assert(Array::order == N, "array order mismatch");
    static_assert(std::is_assignable<T&, typename Array::value_type>::value, "incompatible assignment");

    auto old_data = this->data();
    auto old_size = this->size();

    this->data_ = this->allocate(source.size());
    this->descriptor_ = {0, source.extents()};
    this->create(detail::trivial_dispatch<T>(), this->data(), source.begin(), source.size());

    clear(old_data, old_size);

    return *this;
  }

  /// \group Fill
  /// Fills the array.
  auto fill(const T& value)
  {
    for (auto& elem : *this)
      elem = value;
  }

  /// \group Fill
  template <typename... Args> auto fill(in_place_t, Args&&... args)
  {
    DEBUG_ASSERT(this->data(), debug::module{}, debug::level::invalid_state, "array is empty");
    detail::array_allocator<T>::destroy(detail::trivial_dispatch<T>(), this->data(), this->size());
    detail::array_allocator<T>::create(detail::trivial_dispatch<T>(), this->data(), this->size(), std::forward<Args>(args)...);
  }

  /// \group Begin
  /// Returns an iterator to the first element of the array.
  auto begin() -> iterator { return this->data(); }

  /// \group End
  /// Returns an iterator to the element following the last element of the container.
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
  static auto clear(T* data, index_t size)
  {
    if (data) {
      detail::array_allocator<T>::destroy(detail::trivial_dispatch<T>(), data, size);
      detail::array_allocator<T>::deallocate(data, size);
    }
  }

  static auto calculate_descriptor(recursive_initializer_list_t<T, N> values) -> base_slice<N>
  {
    std::array<index_t, N> extents;

    calculate_descriptor_fill(extents, values, 0u);
    DEBUG_ASSERT(calculate_descriptor_check(extents, values, 0u), debug::module{}, debug::level::boundary_check,
                 "invalid initializer");

    return {0u, extents};
  }

  template <typename List> static auto calculate_descriptor_fill(std::array<index_t, N>& extents, List values, index_t pos)
  {
    DEBUG_ASSERT(values.size() > 0, debug::module{}, debug::level::boundary_check, "invalid initializer");
    extents[pos] = values.size();
    calculate_descriptor_fill(extents, *values.begin(), pos + 1);
  }

  static auto calculate_descriptor_fill(std::array<index_t, N>&, const T&, index_t) {}

  template <typename List> static auto calculate_descriptor_check(const std::array<index_t, N>& extents, List values, index_t pos)
  {
    if (values.size() != extents[pos])
      return false;

    for (auto subvalues : values)
      if (!calculate_descriptor_check(extents, subvalues, pos + 1))
        return false;

    return true;
  }

  static constexpr auto calculate_descriptor_check(const std::array<index_t, N>&, const T&, index_t) { return true; }
};

/// 1-Dimensional concrete array.
///
/// Basic data-type to represent arrays with 1 dimension.
/// For more operations over this class, consult [jules::ref_array<T, 1>]().
///
/// \module N-Dimensional Array
/// \notes Using the aliasing [jules::vector]() is recommended to keep portability between versions.
/// \notes Elements are contiguously stored.
template <typename T> class base_array<T, 1> : public ref_array<T, 1>, private detail::array_allocator<T>
{
  template <typename, std::size_t> friend class base_array;

public:
  /// The type of the elements.
  using value_type = T;

  /// The order of dimensionality.
  static constexpr auto order = 1;

  /// `RandomAccessIterator` over contiguous elements.
  using iterator = T*;

  /// Constant `RandomAccessIterator` over contiguous elements.
  using const_iterator = const T*;

  /// Unsigned integer type that can store the dimensions of the array.
  using size_type = index_t;

  /// Signed integer type that can store differences between sizes.
  using difference_type = distance_t;

  ~base_array() { clear(this->data(), this->size()); }

  /// \group Constructors
  /// Constructs a new vector from a variety of data sources.
  ///
  /// 1) Default constructor. Constructs an empty vector.
  ///
  /// 2) Explicit constructor with size `length`.  Elements are default initialized.
  ///
  /// 3) Constructs the vector with copies of elements with value `value` and length `length`.
  ///
  /// 4) Constructs the container with the contents of the range [`first`, `last`).
  ///
  /// 5) Constructs the vector from a `initialized_list`.
  ///
  /// 6) Constructs the vector from a range.
  ///
  /// 7) Copy constructor.
  ///
  /// 8) Move constructor.
  ///
  /// 9) Converting constructor.  Constructs a vector from other vector-like structures,
  ///   e.g. expression arrays or sliced arrays.
  base_array() : ref_array<T, 1>{nullptr, {}} {}

  /// \group Constructors
  explicit base_array(index_t length) : ref_array<T, 1>{this->allocate(length), {0u, length}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), this->size());
  }

  /// \group Constructors
  base_array(const T& value, index_t length) : ref_array<T, 1>{this->allocate(length), {0u, length}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), this->size(), value);
  }

  /// \group Constructors
  template <typename Iter, typename U = range::iterator_value_t<Iter>>
  base_array(Iter first, Iter last)
    : ref_array<T, 1>{this->allocate(range::distance(first, last)), {0u, static_cast<index_t>(range::distance(first, last))}}
  {
    static_assert(std::is_constructible<T, const U&>::value, "incompatible value types");
    this->create(detail::trivial_dispatch<T>(), this->data(), first, this->size());
  }

  /// \group Constructors
  base_array(std::initializer_list<T> values) : base_array(values.begin(), values.end()) {}

  /// \group Constructors
  template <typename Rng, typename U = range::range_value_t<std::decay_t<Rng>>,
            CONCEPT_REQUIRES_(range::Range<std::decay_t<Rng>>()), typename = array_fallback<void, std::decay_t<Rng>>>
  base_array(Rng&& rng)
    : ref_array<T, 1>{this->allocate(range::size(std::forward<Rng>(rng))), {0u, range::size(std::forward<Rng>(rng))}}
  {
    static_assert(std::is_constructible<T, const U&>::value, "incompatible value types");
    this->create(detail::trivial_dispatch<T>(), this->data(), range::begin(std::forward<Rng>(rng)), this->size());
  }

  /// \group Constructors
  base_array(const base_array& source) : ref_array<T, 1>{this->allocate(source.size()), source.descriptor_}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), source.data(), this->size());
  }

  /// \group Constructors
  base_array(base_array&& source) noexcept : ref_array<T, 1>{move_ptr(source.data_), std::move(source.descriptor_)} {}

  /// \group Constructors
  template <typename Array, typename = array_request<void, Array>>
  base_array(const Array& source) : ref_array<T, 1>{this->allocate(source.size()), {0u, source.extents()}}
  {
    static_assert(Array::order == 1, "array order mismatch");
    static_assert(std::is_constructible<T, const typename Array::value_type&>::value, "incompatible value types");
    this->create(detail::trivial_dispatch<T>(), this->data(), source.begin(), this->size());
  }

  /// \group Assignment
  ///
  /// 1) Copy assignment.
  ///
  /// 2) Move assignment.
  ///
  /// 3) Assignment from array-like structures.
  auto operator=(const base_array& source) & -> base_array&
  {
    DEBUG_ASSERT(this != &source, debug::module{}, debug::level::invalid_argument, "self assignment");
    clear(this->data(), this->size());
    this->data_ = this->allocate(source.size());
    this->descriptor_ = source.descriptor_;
    this->create(detail::trivial_dispatch<T>(), this->data(), source.data(), source.size());
    return *this;
  }

  /// \group Assignment
  auto operator=(base_array&& source) & noexcept -> base_array&
  {
    DEBUG_ASSERT(this != &source, debug::module{}, debug::level::invalid_argument, "self assignment");
    clear(this->data(), this->size());
    this->data_ = move_ptr(source.data_);
    this->descriptor_ = std::move(source.descriptor_);
    return *this;
  }

  /// \group Assignment
  template <typename Array> auto operator=(const Array& source) & -> array_request<base_array&, Array>
  {
    static_assert(Array::order == 1, "array order mismatch");
    static_assert(std::is_assignable<T&, typename Array::value_type>::value, "incompatible assignment");

    auto old_data = this->data();
    auto old_size = this->size();

    this->data_ = this->allocate(source.size());
    this->descriptor_ = {0u, source.extents()};
    this->create(detail::trivial_dispatch<T>(), this->data(), source.begin(), source.size());

    clear(old_data, old_size);

    return *this;
  }

  /// \group Fill
  /// Fills the array.
  auto fill(const T& value)
  {
    for (auto& elem : *this)
      elem = value;
  }

  /// \group Fill
  template <typename... Args> auto fill(in_place_t, Args&&... args)
  {
    DEBUG_ASSERT(this->data(), debug::module{}, debug::level::invalid_state, "array is empty");
    detail::array_allocator<T>::destroy(detail::trivial_dispatch<T>(), this->data(), this->size());
    detail::array_allocator<T>::create(detail::trivial_dispatch<T>(), this->data(), this->size(), std::forward<Args>(args)...);
  }

  /// \group Begin
  /// Returns an iterator to the first element of the array.
  auto begin() -> iterator { return this->data(); }

  /// \group End
  /// Returns an iterator to the element following the last element of the container.
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
  static auto clear(T* data, index_t size)
  {
    if (data) {
      detail::array_allocator<T>::destroy(detail::trivial_dispatch<T>(), data, size);
      detail::array_allocator<T>::deallocate(data, size);
    }
  }
};

/// N-Dimensional array aliasing.  Data type defaults to [jules::numeric]().
template <std::size_t N, typename T = jules::numeric> using ndarray = base_array<T, N>;

/// Matrix aliasing.  Data type defaults to [jules::numeric]().
template <typename T = jules::numeric> using matrix = base_array<T, 2>;

/// Vector aliasing.  Data type defaults to [jules::numeric]().
template <typename T = jules::numeric> using vector = base_array<T, 1>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
