// Copyright (c) 2017-2020 Filipe Verri <filipeverri@gmail.com>

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

  static auto allocate(index_t size) -> value_type* { return reinterpret_cast<value_type*>(new storage_type[size]); }

  static auto deallocate(value_type* data, index_t) noexcept { delete[] reinterpret_cast<storage_type*>(data); }

  template <typename... Args>
  static auto construct(value_type* data, index_t size) noexcept(std::is_nothrow_constructible_v<value_type>)
  {
    std::uninitialized_value_construct_n(data, size);
  }

  template <typename U>
  requires constructible_from<value_type, const U&> static auto
  construct(value_type* to, index_t size, const U& value) noexcept(std::is_nothrow_constructible_v<value_type, const U&>)
  {
    std::uninitialized_fill_n(to, size, value);
  }

  template <typename It, typename U = typename ranges::iter_reference_t<It>>
  static auto construct(value_type* to, It from, index_t size) noexcept(std::is_nothrow_constructible_v<value_type, U>)
  {
    std::uninitialized_copy_n(from, size, to);
  }

  template <std::size_t N>
  static auto construct(value_type* to, recursive_initializer_list_t<value_type, N> values,
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

  static auto destroy(value_type* data, index_t size) noexcept { std::destroy_n(data, size); }

private:
  template <std::size_t N, typename List, typename... Args>
  static auto construct_recursive(value_type* to, List values, const descriptor<N>& desc, Args... indexes) noexcept
  {
    for (auto i = index_t{0u}; i < desc.extent(sizeof...(Args)); ++i)
      construct_recursive(to, *(values.begin() + i), desc, indexes..., i);
  }

  template <std::size_t N, typename... Args>
  static auto construct_recursive(value_type* to, const value_type& value, const descriptor<N>& desc, Args... indexes) noexcept
  {
    ::new (static_cast<void*>(to + desc({{indexes...}}))) value_type(value);
  }

  template <std::size_t N, typename List, typename... Args>
  static auto construct_recursive(index_t& constructed_count, value_type* to, List values, const descriptor<N>& desc,
                                  Args... indexes)
  {
    for (auto i = index_t{0u}; i < desc.extent(sizeof...(Args)); ++i)
      construct_recursive(constructed_count, to, *(values.begin() + i), desc, indexes..., i);
  }

  template <std::size_t N, typename... Args>
  static auto construct_recursive(index_t& constructed_count, value_type* to, const value_type& value, const descriptor<N>& desc,
                                  Args... indexes)
  {
    ::new (static_cast<void*>(to + desc({{indexes...}}))) value_type(value);
    ++constructed_count;
  }

  template <std::size_t N, typename List, typename... Args>
  static auto destroy_recursive(index_t& constructed_count, value_type* to, List values, const descriptor<N>& desc,
                                Args... indexes) noexcept
  {
    for (auto i = index_t{0u}; constructed_count > 0 && i < desc.extent(sizeof...(Args)); ++i)
      destroy_recursive(constructed_count, to, *(values.begin() + i), desc, indexes..., i);
  }

  template <std::size_t N, typename... Args>
  static auto destroy_recursive(index_t& constructed_count, value_type* to, const value_type&, const descriptor<N>& desc,
                                Args... indexes) noexcept
  {
    (to + desc({{indexes...}}))->~value_type();
    --constructed_count;
  }
};

} // namespace jules

#endif // JULES_ARRAY_ALLOCATOR_H
