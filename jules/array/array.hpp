#ifndef JULES_ARRAY_ARRAY_H
#define JULES_ARRAY_ARRAY_H

#include <jules/array/detail/common.hpp>
#include <jules/array/ref_array.hpp>
#include <jules/core/type.hpp>

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

  base_array();
  ~base_array();

  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>> explicit base_array(Dims... dims);

  template <typename... Dims, typename = detail::n_indexes_enabler<N, Dims...>> base_array(const T& value, Dims... dims);

  template <typename Iter, typename... Dims, typename R = typename std::iterator_traits<Iter>::value_type,
            typename = detail::n_indexes_enabler<N, Dims...>, typename = std::enable_if_t<std::is_convertible<R, T>::value>>
  base_array(Iter iter, Dims... dims);

  base_array(const base_array& source);
  base_array(base_array&& source) noexcept;

  template <typename Array, typename = detail::array_request<void, Array>> base_array(const Array& source);

  auto operator=(const base_array& source) -> base_array&;
  auto operator=(base_array&& source) noexcept -> base_array&;

  template <typename Array> auto operator=(const Array& source) -> detail::array_request<base_array&, Array>;

  auto begin() -> iterator { return data_; }
  auto end() -> iterator { return data_ + size(); }

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return data_; }
  auto cend() const -> const_iterator { return data_ + size(); }

private:
  static T* allocate(index_t size) { return reinterpret_cast<T*>(new storage_type[size]); }
  static void deallocate(T* data, index_t) { delete[] reinterpret_cast<storage_type*>(data); }

  template <typename... Args> static void create(detail::trivial_tag, T* data, index_t size, Args&&... args)
  {
    create(detail::non_trivial_tag{}, data, size, std::forward<Args>(args)...);
  }

  template <typename... Args> static void create(detail::non_trivial_tag, T* data, index_t size, Args&&... args)
  {
    auto current = index{0u};
    try {
      for (; current < size; ++current)
        ::new (static_cast<void*>(data + current)) T(std::forward<Args>(args)...);
    } catch (...) {
      for (auto i = index{0u}; i != current; ++j)
        static_cast<T*>(data + i)->~T();
      throw;
    }
    return current;
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

  static void destroy(detail::trivial_tag, T* data, index_t size) {}

  static void destroy(detail::non_trivial_tag, T* data, index_t size)
  {
    for (auto i = index_t{0u}; i < size; ++i) // use jules::seq (verify other files)
      data[i].~T();
  }
};

template <std::size_t N, typename T = jules::numeric> using array = base_array<T, N>;
template <typename T = jules::numeric> using matrix = base_array<T, 2>;
template <typename T = jules::numeric> using vector = base_array<T, 1>;

} // namespace jules

#endif // JULES_ARRAY_ARRAY_H
