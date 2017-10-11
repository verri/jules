// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_ARRAY_H
/// \exclude
#define JULES_ARRAY_ARRAY_H

#include <jules/array/allocator.hpp>
#include <jules/array/blas.hpp>
#include <jules/array/builder.hpp>
#include <jules/array/functional.hpp>
#include <jules/array/io.hpp>
#include <jules/array/math.hpp>
#include <jules/array/numeric.hpp>
#include <jules/array/ref_array.hpp>
#include <jules/base/async.hpp>
#include <jules/base/numeric.hpp>

namespace jules
{

/// N-Dimensional concrete array.
///
/// Basic data-type to represent arrays with 1 or more dimensions.
///
/// Most of the operations are implemented in its base class,
/// consult [jules::ref_array<T, N>]().
///
/// \module Array Types
/// \notes It is recommended to use the alias [jules::matrix]() for 2-dimensional structures
///   and [jules::vector]() for 1-dimensional.
/// \notes Elements are contiguously stored in column-major order.
template <typename T, std::size_t N> class array : public ref_array<T, N>, private array_allocator<T>
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class array;

  template <typename... Dims>
  using requires_dimensions = meta::requires<std::bool_constant<(sizeof...(Dims) == N)>, std::is_convertible<Dims, index_t>...>;

  struct allocate_tag
  {};

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
  ~array() { clear(); }

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
  array() : ref_array<value_type, order>{nullptr, {}} {}

  /// \group constructors
  /// \tparam _
  ///   \exclude
  template <typename... Dims, typename _ = requires_dimensions<Dims...>>
  explicit array(uninitialized_t, Dims... dims) : array(allocate_tag{}, dims...)
  {
    static_assert(std::is_pod_v<value_type>, "Only POD types are allowed to be left uninitialized");
  }

  /// \group constructors
  /// \tparam _
  ///   \exclude
  template <typename... Dims, typename _ = requires_dimensions<Dims...>>
  explicit array(Dims... dims) : array(allocate_tag{}, dims...)
  {
    try {
      this->construct(this->data(), this->size());
    } catch (...) {
      this->deallocate(this->data(), this->size());
      this->data_ = nullptr;
      throw;
    }
  }

  /// \group constructors
  /// \tparam _
  ///   \exclude
  template <typename... Dims, typename _ = requires_dimensions<Dims...>>
  array(const value_type& value, Dims... dims) : array(allocate_tag{}, dims...)
  {
    try {
      this->construct(this->data(), this->size(), value);
    } catch (...) {
      this->deallocate(this->data(), this->size());
      this->data_ = nullptr;
      throw;
    }
  }

  /// \group constructors
  /// \tparam R
  ///   \exclude
  /// \tparam _
  ///   \exclude
  template <typename It, typename... Dims, typename R = range::iterator_value_t<It>, typename _ = requires_dimensions<Dims...>>
  array(It it, Dims... dims) : array(allocate_tag{}, dims...)
  {
    try {
      this->construct(this->data(), it, this->size());
    } catch (...) {
      this->deallocate(this->data(), this->size());
      this->data_ = nullptr;
      throw;
    }
  }

  /// \group constructors
  array(recursive_initializer_list_t<value_type, order> values)
  {
    this->descriptor_ = this->calculate_descriptor(values);
    this->data_ = this->allocate(this->descriptor_.size());

    try {
      this->construct(this->data(), values, this->descriptor_);
    } catch (...) {
      this->deallocate(this->data(), this->size());
      this->data_ = nullptr;
      throw;
    }
  }

  /// \group constructors
  array(const array& source) : ref_array<value_type, order>{this->allocate(source.size()), source.descriptor_}
  {
    try {
      this->construct(this->data(), source.data(), this->size());
    } catch (...) {
      this->deallocate(this->data(), this->size());
      this->data_ = nullptr;
      throw;
    }
  }

  /// \group constructors
  array(array&& source) noexcept
    : ref_array<value_type, order>{std::exchange(source.data_, nullptr), std::move(source.descriptor_)}
  {}

  /// \group constructors
  /// \tparam _
  ///   \exclude
  template <typename Array>
  array(const common_array_base<Array>& source)
    : ref_array<value_type, order>{this->allocate(source.size()), {source.dimensions()}}
  {
    static_assert(Array::order == order, "array order mismatch");
    static_assert(std::is_constructible<value_type, const typename Array::value_type&>::value, "incompatible value types");
    try {
      this->construct(this->data(), source.begin(), this->size());
    } catch (...) {
      this->deallocate(this->data(), this->size());
      this->data_ = nullptr;
      throw;
    }
  }

  /// \group constructors
  template <typename Rng, typename = meta::requires<range::SizedRange<Rng>, std::negation<CommonArray<Rng>>>>
  array(const Rng& rng) : ref_array<value_type, order>{this->allocate(range::size(rng)), {{range::size(rng)}}}
  {
    static_assert(order == 1u, "Only vectors can be initialized from a range");
    static_assert(std::is_constructible<value_type, range::reference_t<range::iterator_t<Rng>>>::value,
                  "incompatible value types");
    try {
      this->construct(this->data(), range::begin(rng), this->size());
    } catch (...) {
      this->deallocate(this->data(), this->size());
      this->data_ = nullptr;
      throw;
    }
  }

  /// \group constructors
  template <typename Iter, typename Sent, typename = meta::requires<range::Sentinel<Sent, Iter>>>
  array(Iter begin, Sent end)
    : ref_array<value_type, order>{this->allocate(range::distance(begin, end)),
                                   {{static_cast<size_type>(range::distance(begin, end))}}}
  {
    static_assert(order == 1u, "Only vectors can be initialized from a pair of iterators");
    static_assert(std::is_constructible<value_type, range::reference_t<Iter>>::value, "incompatible value types");
    try {
      this->construct(this->data(), begin, this->size());
    } catch (...) {
      this->deallocate(this->data(), this->size());
      this->data_ = nullptr;
      throw;
    }
  }

  /// \group constructors
  array(array_builder<value_type, order> builder) : ref_array<value_type, order>{builder.release(), {builder.extents()}} {}

  /// \group assignment Assignment
  ///
  /// (1) Copy assignment.
  ///
  /// (2) Move assignment.
  ///
  /// (3) Assignment from array-like structures.
  ///
  /// \requires `A` shall be `Array`
  auto operator=(const array& source) -> array&
  {
    DEBUG_ASSERT(this != &source, debug::default_module, debug::level::invalid_argument, "self assignment");

    auto new_data = this->allocate(source.size());
    auto success = false;

    JULES_DEFER({
      if (success) {
        clear();
        this->data_ = new_data;
        this->descriptor_ = source.descriptor_;
      }
    });

    this->construct(new_data, source.data(), source.size());
    success = true;

    return *this;
  }

  /// \group assignment
  auto operator=(array&& source) noexcept -> array&
  {
    DEBUG_ASSERT(this != &source, debug::default_module, debug::level::invalid_argument, "self assignment");
    std::swap(this->data_, source.data_);
    std::swap(this->descriptor_, source.descriptor_);
    return *this;
  }

  /// \group assignment
  /// \synopsis_return array&
  template <typename Array> auto operator=(const common_array_base<Array>& source) -> array&
  {
    static_assert(Array::order == order, "array order mismatch");
    static_assert(std::is_assignable<value_type&, typename Array::value_type>::value, "incompatible assignment");

    auto new_data = this->allocate(source.size());
    auto success = false;

    JULES_DEFER({
      if (success) {
        clear();
        this->data_ = new_data;
        this->descriptor_ = {source.dimensions()};
      }
    });

    this->construct(new_data, source.begin(), source.size());
    success = true;

    return *this;
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
  template <typename... Dims, typename _ = requires_dimensions<Dims...>>
  explicit array(allocate_tag, Dims... dims) : ref_array<value_type, order>{this->allocate(prod_args(dims...)), {{{dims...}}}}
  {}

  /// \exclude
  auto clear() noexcept
  {
    if (this->data()) {
      this->destroy(this->data(), this->size());
      this->deallocate(this->data(), this->size());
    }
  }

  /// \exclude
  static auto calculate_descriptor(recursive_initializer_list_t<value_type, order> values) -> descriptor<order>
  {
    auto extents = std::array<index_t, order>();

    calculate_descriptor_fill(extents, values, 0u);
    DEBUG_ASSERT(calculate_descriptor_check(extents, values, 0u), debug::default_module, debug::level::boundary_check,
                 "invalid initializer");

    return {extents};
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

template <typename Array> array(const common_array_base<Array>&)->array<typename Array::value_type, Array::order>;

template <typename T, std::size_t N> auto ref(array<T, N>& a) -> ref_array<T, N> { return {a.data(), {a.dimensions()}}; }

template <typename T, std::size_t N> auto ref(const array<T, N>& a) -> ref_array<const T, N>
{
  return {a.data(), {a.dimensions()}};
}

/// \group array_alias Array Aliasing
/// (1) Matrix aliasing.
///
/// (2) Vector aliasing.
template <typename T = jules::numeric> using matrix = array<T, 2>;

/// \group array_alias
template <typename T = jules::numeric> using vector = array<T, 1>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
