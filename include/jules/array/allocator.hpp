// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_ALLOCATOR_H
#define JULES_ARRAY_ALLOCATOR_H

#include <jules/array/descriptor.hpp>
#include <jules/core/type.hpp>

#include <array>
#include <iterator>
#include <memory>
#include <type_traits>

namespace jules
{

template <typename T> struct array_allocator
{
  using value_type = T;

  using storage_type = std::aligned_storage_t<sizeof(value_type), alignof(value_type)>;

  auto allocate(index_t size) -> value_type* { return reinterpret_cast<value_type*>(new storage_type[size]); }

  auto deallocate(value_type* data, index_t) noexcept { delete[] reinterpret_cast<storage_type*>(data); }

  template <typename... Args> auto construct(value_type* data, index_t size) noexcept(std::is_nothrow_constructible_v<value_type>)
  {
    std::uninitialized_value_construct_n(data, size);
  }

  template <typename U, typename = meta::requires<std::is_constructible<value_type, const U&>>>
  auto construct(value_type* to, index_t size, const U& value) noexcept(std::is_nothrow_constructible_v<value_type, const U&>)
  {
    std::uninitialized_fill_n(to, size, value);
  }

  template <typename It, typename U = typename std::iterator_traits<It>::reference>
  auto construct(value_type* to, It from, index_t size) noexcept(std::is_nothrow_constructible_v<value_type, U>)
  {
    std::uninitialized_copy_n(from, size, to);
  }

  template <std::size_t N>
  auto construct(value_type* to, recursive_initializer_list_t<value_type, N> values,
                 const descriptor<N>& desc) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  {
    if constexpr (std::is_nothrow_copy_constructible_v<value_type>) {
      construct_recursive(to, values, desc);
    } else {
      auto constructed_count = index_t{0};
      try {
        construct_recursive(constructed_count, to, values, desc);
      } catch (...) {
        destroy_recursive(constructed_count, to, values, desc);
        throw;
      }
    }
  }

  void destroy(value_type* data, index_t size) noexcept { std::destroy_n(data, size); }

private:
  template <std::size_t N, typename List, typename... Args>
  auto construct_recursive(value_type* to, List values, const descriptor<N>& desc, Args... indexes) noexcept
  {
    for (auto i = index_t{0u}; i < desc.extents[sizeof...(Args)]; ++i)
      construct_recursive(to, *(values.begin() + i), desc, indexes..., i);
  }

  template <std::size_t N, typename... Args>
  auto construct_recursive(value_type* to, const value_type& value, const descriptor<N>& desc, Args... indexes) noexcept
  {
    ::new (static_cast<void*>(to + desc({{indexes...}}))) value_type(value);
  }

  template <std::size_t N, typename List, typename... Args>
  auto construct_recursive(index_t& constructed_count, value_type* to, List values, const descriptor<N>& desc, Args... indexes)
  {
    for (auto i = index_t{0u}; i < desc.extents[sizeof...(Args)]; ++i)
      construct_recursive(constructed_count, to, *(values.begin() + i), desc, indexes..., i);
  }

  template <std::size_t N, typename... Args>
  auto construct_recursive(index_t& constructed_count, value_type* to, const value_type& value, const descriptor<N>& desc,
                           Args... indexes)
  {
    ::new (static_cast<void*>(to + desc({{indexes...}}))) value_type(value);
    ++constructed_count;
  }

  template <std::size_t N, typename List, typename... Args>
  auto destroy_recursive(index_t& constructed_count, value_type* to, List values, const descriptor<N>& desc,
                         Args... indexes) noexcept
  {
    for (auto i = index_t{0u}; i < desc.extents[sizeof...(Args)]; ++i) {
      destroy_recursive(constructed_count, to, *(values.begin() + i), desc, indexes..., i);
      if (constructed_count == 0)
        return;
    }
  }

  template <std::size_t N, typename... Args>
  auto destroy_recursive(index_t& constructed_count, value_type* to, const value_type&, const descriptor<N>& desc,
                         Args... indexes) noexcept
  {
    if (constructed_count == 0)
      return;
    (to + desc({{indexes...}}))->~value_type();
    --constructed_count;
  }
};

} // namespace jules

#endif // JULES_ARRAY_ALLOCATOR_H
