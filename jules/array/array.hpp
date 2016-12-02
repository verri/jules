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
    create(detail::trivial_dispatch<T>(), this->data(), this->size());
  }

  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>>
  base_array(const T& value, Dims... dims) : ref_array<T, N>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
    create(detail::trivial_dispatch<T>(), this->data(), this->size(), value);
  }

  template <typename Iter, typename... Dims, typename R = typename std::iterator_traits<Iter>::value_type,
            typename = detail::n_indexes_enabler<N, Dims...>, typename = std::enable_if_t<std::is_convertible<R, T>::value>>
  base_array(Iter iter, Dims... dims) : ref_array<T, N>{this->allocate(prod_args(dims...)), {0u, {{index_t{dims}...}}}}
  {
    create(detail::trivial_dispatch<T>(), this->data(), iter, this->size());
  }

  base_array(const base_array& source) : ref_array<T, N>{this->allocate(source.size()), source.descriptor_}
  {
    create(detail::trivial_dispatch<T>(), this->data(), source.data(), this->size());
  }

  base_array(base_array&& source) noexcept : ref_array<T, N>{move_ptr(source.data_), std::move(source.descriptor_)} {}

  template <typename Array, typename = detail::array_request<void, Array>>
  base_array(const Array& source) : ref_array<T, N>{this->allocate(source.size()), {0u, source.extents()}}
  {
    static_assert(Array::order == N, "array order mismatch");
    static_assert(std::is_constructible<T, const typename Array::value_type&>::value, "incompatible value types");
    create(detail::trivial_dispatch<T>(), this->data(), source.begin(), this->size());
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

  template <typename Array> auto operator=(const Array& source) -> detail::array_request<base_array&, Array>
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
