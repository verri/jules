#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/ref_array.hpp>
#include <jules/base/numeric.hpp>
#include <jules/core/type.hpp>
#include <jules/base/async.hpp>

#include <cstring>
#include <memory>
#include <type_traits>

namespace jules
{

template <typename T, std::size_t N> class base_array : public ref_array<T, N>
{
  template <typename, std::size_t> friend class base_array;

public:
  using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;
  using value_type = T;
  static constexpr auto order = N;

  using iterator = T*;
  using const_iterator = const T*;

  using size_type = index_t;
  using difference_type = distance_t;

  ~base_array()
  {
    clear();
  }

  base_array() : ref_array<T, N>{nullptr, {}} {}

  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>> explicit base_array(Dims... dims)
    : ref_array<T, N>{allocate(prod_args(dims...)), {0u, {{ index_t{dims}... }}}}
  {
    create(detail::trivial_dispatch<T>(), this->data(), this->size());
  }

  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>> base_array(const T& value, Dims... dims)
    : ref_array<T, N>{allocate(prod_args(dims...)), {0u, {{ index_t{dims}... }}}}
  {
    create(detail::trivial_dispatch<T>(), this->data(), this->size(), value);
  }

  template <typename Iter, typename... Dims, typename R = typename std::iterator_traits<Iter>::value_type,
            typename = detail::n_indexes_enabler<N, Dims...>, typename = std::enable_if_t<std::is_convertible<R, T>::value>>
  base_array(Iter iter, Dims... dims)
    : ref_array<T, N>{allocate(prod_args(dims...)), {0u, {{ index_t{dims}... }}}}
  {
    create(detail::trivial_dispatch<T>(), this->data(), iter, this->size());
  }

  base_array(const base_array& source)
    : ref_array<T, N>{allocate(source.size()), source.descriptor_}
  {
    create(detail::trivial_dispatch<T>(), this->data(), source.data(), this->size());
  }

  base_array(base_array&& source) noexcept
    :ref_array<T, N>{move_ptr(source.data_), std::move(source.descriptor_)}
  {
  }

  template <typename Array, typename = detail::array_request<void, Array>> base_array(const Array& source)
    : ref_array<T, N>{allocate(source.size()), {0u, source.extents()}}
  {
    static_assert(std::is_constructible<T, const typename Array::value_type&>::value, "incompatible value types");
    create(detail::trivial_dispatch<T>(), this->data(), source.begin(), this->size());
  }

  auto operator=(const base_array& source) -> base_array&
  {
    clear();
    this->data_ = allocate(source.size());
    this->descriptor_ = source.descriptor_;
    create(detail::trivial_dispatch<T>(), this->data(), source.data(), source->size());
    return *this;
  }

  auto operator=(base_array&& source) noexcept -> base_array&
  {
    clear();
    this->data_ = move_ptr(source.data_);
    this->descriptor_ = std::move(source.descriptor_);
    return *this;
  }

  template <typename Array> auto operator=(const Array& source) -> detail::array_request<base_array&, Array>
  {
    clear();
    this->data_ = allocate(source.size());
    this->descriptor_ = {0, source.extents()};
    create(detail::trivial_dispatch<T>(), this->data(), source.begin(), source.size());
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
  // TODO: before specializing, put these methods in a class
  static T* allocate(index_t size) { return reinterpret_cast<T*>(new storage_type[size]); }
  static void deallocate(T* data, index_t) { delete[] reinterpret_cast<storage_type*>(data); }

  template <typename... Args> static void create(detail::trivial_tag, T* data, index_t size, Args&&... args)
  {
    create(detail::non_trivial_tag{}, data, size, std::forward<Args>(args)...);
  }

  template <typename... Args> static void create(detail::non_trivial_tag, T* data, index_t size, Args&&... args)
  {
    auto current = index_t{0u};
    try {
      for (; current < size; ++current)
        ::new (static_cast<void*>(data + current)) T(std::forward<Args>(args)...);
    } catch (...) {
      for (auto i = index_t{0u}; i != current; ++i)
        static_cast<T*>(data + i)->~T();
      throw;
    }
  }

  template <typename It> static void create(detail::trivial_tag, T* to, It from, index_t size)
  {
    std::uninitialized_copy_n(from, size, to);
  }

  static void create(detail::trivial_tag, T* to, const T* from, index_t size) { std::memcpy(to, from, size * sizeof(T)); }

  template <typename It> static void create(detail::non_trivial_tag, T* to, It from, index_t size)
  {
    std::uninitialized_copy_n(from, size, to);
  }

  static void destroy(detail::trivial_tag, T*, index_t) {}

  static void destroy(detail::non_trivial_tag, T* data, index_t size)
  {
    for (auto i = index_t{0u}; i < size; ++i) // use jules::seq (verify other files)
      data[i].~T();
  }

  void clear()
  {
    if (this->data()) {
      destroy(detail::trivial_dispatch<T>(), this->data(), this->size());
      deallocate(this->data(), this->size());
    }
  }
};

template <std::size_t N, typename T = jules::numeric> using array = base_array<T, N>;
template <typename T = jules::numeric> using matrix = base_array<T, 2>;
template <typename T = jules::numeric> using vector = base_array<T, 1>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
