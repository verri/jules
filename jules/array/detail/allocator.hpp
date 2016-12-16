// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_DETAIL_ALLOCATOR_H
#define JULES_ARRAY_DETAIL_ALLOCATOR_H

#include <jules/array/detail/slicing.hpp>
#include <jules/array/slice.hpp>
#include <jules/core/range.hpp>
#include <jules/core/type.hpp>

#include <cstring>
#include <memory>
#include <type_traits>

namespace jules
{
namespace detail
{

template <typename T> struct array_allocator {
  using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;

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

  template <typename It, CONCEPT_REQUIRES_(range::Iterator<It>())>
  static void create(detail::trivial_tag, T* to, It from, index_t size)
  {
    std::uninitialized_copy_n(from, size, to);
  }

  static void create(detail::trivial_tag, T* to, const T* from, index_t size) { std::memcpy(to, from, size * sizeof(T)); }

  template <std::size_t N>
  static void create(detail::trivial_tag, T* to, recursive_initializer_list_t<T, N> values, base_slice<N> descriptor)
  {
    create(detail::non_trivial_tag{}, to, values, descriptor);
  }

  template <std::size_t N>
  static void create(detail::non_trivial_tag, T* to, recursive_initializer_list_t<T, N> values, base_slice<N> descriptor)
  {
    create_impl(to, values, descriptor);
  }

  template <typename It, CONCEPT_REQUIRES_(range::Iterator<It>())>
  static void create(detail::non_trivial_tag, T* to, It from, index_t size)
  {
    std::uninitialized_copy_n(from, size, to);
  }

  static void destroy(detail::trivial_tag, T*, index_t) {}

  static void destroy(detail::non_trivial_tag, T* data, index_t size)
  {
    for (auto i = index_t{0u}; i < size; ++i) // use jules::seq (verify other files)
      data[i].~T();
  }

private:
  template <std::size_t N, typename List, typename... Args>
  static void create_impl(T* to, List values, base_slice<N> descriptor, Args... indexes)
  {
    for (auto i = index_t{0u}; i < extent(descriptor, sizeof...(Args)); ++i)
      create_impl(to, *(values.begin() + i), descriptor, indexes..., i);
  }

  template <std::size_t N, typename... Args>
  static void create_impl(T* to, const T& value, base_slice<N> descriptor, Args... indexes)
  {
    ::new (static_cast<void*>(to + descriptor(indexes...))) T(value);
  }
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ALLOCATOR_H
