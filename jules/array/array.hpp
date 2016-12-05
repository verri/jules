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

namespace jules
{

template <typename T, std::size_t N> class base_array : public ref_array<T, N>, private detail::array_allocator<T>
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class base_array;

public:
  using value_type = T;
  static constexpr auto order = N;

  using iterator = T*;
  using const_iterator = const T*;

  using size_type = index_t;
  using difference_type = distance_t;

  ~base_array() { clear(); }

  base_array() : ref_array<T, N>{nullptr, {}} {}

  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
  explicit base_array(Dims... dims) : ref_array<T, N>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), this->size());
  }

  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
  base_array(const T& value, Dims... dims) : ref_array<T, N>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), this->size(), value);
  }

  template <typename Iter, typename... Dims, typename R = typename std::iterator_traits<Iter>::value_type,
            typename = detail::n_indexes_enabler<N, Dims...>, typename = std::enable_if_t<std::is_convertible<R, T>::value>>
  base_array(Iter iter, Dims... dims) : ref_array<T, N>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), iter, this->size());
  }

  base_array(recursive_initializer_list_t<T, N> values)
  {
    this->descriptor_ = this->calculate_descriptor(values);
    this->data_ = this->allocate(this->descriptor_.size());
    this->create(detail::trivial_dispatch<T>(), this->data(), values, this->descriptor_);
  }

  base_array(const base_array& source) : ref_array<T, N>{this->allocate(source.size()), source.descriptor_}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), source.data(), this->size());
  }

  base_array(base_array&& source) noexcept : ref_array<T, N>{move_ptr(source.data_), std::move(source.descriptor_)} {}

  template <typename Array, typename = array_request<void, Array>>
  base_array(const Array& source) : ref_array<T, N>{this->allocate(source.size()), {0u, source.extents()}}
  {
    static_assert(Array::order == N, "array order mismatch");
    static_assert(std::is_constructible<T, const typename Array::value_type&>::value, "incompatible value types");
    this->create(detail::trivial_dispatch<T>(), this->data(), source.begin(), this->size());
  }

  auto operator=(const base_array& source) -> base_array&
  {
    this->clear();
    this->data_ = this->allocate(source.size());
    this->descriptor_ = source.descriptor_;
    this->create(detail::trivial_dispatch<T>(), this->data(), source.data(), source->size());
    return *this;
  }

  auto operator=(base_array&& source) noexcept -> base_array&
  {
    this->clear();
    this->data_ = move_ptr(source.data_);
    this->descriptor_ = std::move(source.descriptor_);
    return *this;
  }

  template <typename Array> auto operator=(const Array& source) -> array_request<base_array&, Array>
  {
    static_assert(Array::order == N, "array order mismatch");
    static_assert(std::is_assignable<T&, typename Array::value_type>::value, "incompatible assignment");
    this->clear();
    this->data_ = this->allocate(source.size());
    this->descriptor_ = {0, source.extents()};
    this->create(detail::trivial_dispatch<T>(), this->data(), source.begin(), source.size());
    return *this;
  }

  auto begin() -> iterator { return this->data(); }
  auto end() -> iterator { return this->data() + this->size(); }

  auto begin() const -> const_iterator { return this->cbegin(); }
  auto end() const -> const_iterator { return this->cend(); }

  auto cbegin() const -> const_iterator { return this->data(); }
  auto cend() const -> const_iterator { return this->data() + this->size(); }

  auto data() { return this->data_; }
  auto data() const { return this->data_; }

private:
  void clear()
  {
    if (this->data()) {
      this->destroy(detail::trivial_dispatch<T>(), this->data(), this->size());
      this->deallocate(this->data(), this->size());
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

template <typename T> class base_array<T, 1> : public ref_array<T, 1>, private detail::array_allocator<T>
{
  template <typename, std::size_t> friend class base_array;

public:
  using value_type = T;
  static constexpr auto order = 1;

  using iterator = T*;
  using const_iterator = const T*;

  using size_type = index_t;
  using difference_type = distance_t;

  ~base_array() { clear(); }

  base_array() : ref_array<T, 1>{nullptr, {}} {}

  explicit base_array(index_t length) : ref_array<T, 1>{this->allocate(length), {0u, length}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), this->size());
  }

  base_array(const T& value, index_t length) : ref_array<T, 1>{this->allocate(length), {0u, length}}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), this->size(), value);
  }

  base_array(std::initializer_list<T> values) : base_array(values.begin(), values.end()) {}

  template <typename Iter, typename U = range::iterator_value_t<Iter>, CONCEPT_REQUIRES_(range::RandomAccessIterator<Iter>())>
  base_array(Iter first, Iter last) : ref_array<T, 1>{this->allocate(last - first), {0u, static_cast<index_t>(last - first)}}
  {
    static_assert(std::is_constructible<T, const U&>::value, "incompatible value types");
    this->create(detail::trivial_dispatch<T>(), this->data(), first, this->size());
  }

  template <typename Rng, typename U = range::range_value_t<Rng>, CONCEPT_REQUIRES_(range::Range<Rng>()),
            typename = array_fallback<void, Rng>>
  base_array(const Rng& rng) : ref_array<T, 1>{this->allocate(range::size(rng), {0u, range::size(rng)})}
  {
    static_assert(std::is_constructible<T, const U&>::value, "incompatible value types");
    // TODO: XXX: check! short circuit should guarantee that order is not called if is not
    // an array.
    static_assert(!is_array<Rng>() || Rng::order == 1, "array order mismatch");
    this->create(detail::trivial_dispatch<T>(), this->data(), range::begin(rng), this->size());
  }

  template <typename Array, typename = array_request<void, Array>>
  base_array(const Array& source) : ref_array<T, 1>{this->allocate(source.size()), {0u, source.extents()}}
  {
    static_assert(Array::order == 1, "array order mismatch");
    static_assert(std::is_constructible<T, const typename Array::value_type&>::value, "incompatible value types");
    this->create(detail::trivial_dispatch<T>(), this->data(), source.begin(), this->size());
  }

  base_array(const base_array& source) : ref_array<T, 1>{this->allocate(source.size()), source.descriptor_}
  {
    this->create(detail::trivial_dispatch<T>(), this->data(), source.data(), this->size());
  }

  base_array(base_array&& source) noexcept : ref_array<T, 1>{move_ptr(source.data_), std::move(source.descriptor_)} {}

  auto operator=(const base_array& source) -> base_array&
  {
    this->clear();
    this->data_ = this->allocate(source.size());
    this->descriptor_ = source.descriptor_;
    this->create(detail::trivial_dispatch<T>(), this->data(), source.data(), source->size());
    return *this;
  }

  auto operator=(base_array&& source) noexcept -> base_array&
  {
    this->clear();
    this->data_ = move_ptr(source.data_);
    this->descriptor_ = std::move(source.descriptor_);
    return *this;
  }

  template <typename Array> auto operator=(const Array& source) -> array_request<base_array&, Array>
  {
    static_assert(Array::order == 1, "array order mismatch");
    static_assert(std::is_assignable<T&, typename Array::value_type>::value, "incompatible assignment");
    this->clear();
    this->data_ = this->allocate(source.size());
    this->descriptor_ = {0, source.extents()};
    this->create(detail::trivial_dispatch<T>(), this->data(), source.begin(), source.size());
    return *this;
  }

  auto begin() -> iterator { return this->data(); }
  auto end() -> iterator { return this->data() + this->size(); }

  auto begin() const -> const_iterator { return this->cbegin(); }
  auto end() const -> const_iterator { return this->cend(); }

  auto cbegin() const -> const_iterator { return this->data(); }
  auto cend() const -> const_iterator { return this->data() + this->size(); }

  auto data() { return this->data_; }
  auto data() const { return this->data_; }

private:
  void clear()
  {
    if (this->data()) {
      this->destroy(detail::trivial_dispatch<T>(), this->data(), this->size());
      this->deallocate(this->data(), this->size());
    }
  }
};

template <std::size_t N, typename T = jules::numeric> using array = base_array<T, N>;
template <typename T = jules::numeric> using matrix = base_array<T, 2>;
template <typename T = jules::numeric> using vector = base_array<T, 1>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
