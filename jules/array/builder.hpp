// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_BUILDER_H
#define JULES_ARRAY_BUILDER_H

#include <jules/array/allocator.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

#include <array>

namespace jules
{

template <typename T, std::size_t N> class array_builder : private array_allocator<T>
{
public:
  using value_type = T;

  explicit array_builder(const std::array<index_t, N>& extents) : data_{this->allocate(prod(extents))}, extents_{extents} {}

  ~array_builder()
  {
    if (data_) {
      this->destroy(data_, count_);
      this->deallocate(data_, prod(extents_));
    }
  }

  array_builder(const array_builder&) = delete;

  array_builder(array_builder&& source) noexcept
    : data_{std::exchange(source.data_, nullptr)}, extents_{source.extents_}, count_{std::exchange(source.count_, 0u)}
  {
  }

  auto operator=(const array_builder&) -> array_builder& = delete;

  auto operator=(array_builder&& source) noexcept -> array_builder&
  {
    std::swap(data_, source.data_);
    std::swap(extents_, source.extents_);
    std::swap(count_, source.count_);
  }

  auto is_complete() const noexcept { return count_ == prod(extents_); }

  auto push_back(const T& value)
  {
    DEBUG_ASSERT(!is_complete(), debug::default_module, debug::level::invalid_state, "builder is already complete");
    ::new (static_cast<void*>(data_ + count_)) T(value);
    ++count_;
  }

  auto push_back(T&& value) noexcept
  {
    DEBUG_ASSERT(!is_complete(), debug::default_module, debug::level::invalid_state, "builder is already complete");
    ::new (static_cast<void*>(data_ + count_)) T(std::move(value));
    ++count_;
  }

  template <typename... Args> auto emplace_back(Args&&... args) noexcept(noexcept(T(std::declval<Args>()...)))
  {
    DEBUG_ASSERT(!is_complete(), debug::default_module, debug::level::invalid_state, "builder is already complete");
    ::new (static_cast<void*>(data_ + count_)) T(std::forward<Args>(args)...);
    ++count_;
  }

  auto extents() const -> const std::array<index_t, N>& { return extents_; }

  auto release()
  {
    DEBUG_ASSERT(is_complete(), debug::default_module, debug::level::invalid_state, "builder is not complete");
    count_ = 0u;
    return std::exchange(data_, nullptr);
  }

private:
  T* data_;
  std::array<index_t, N> extents_;
  index_t count_ = 0u;
};

} // namespace jules

#endif // JULES_ARRAY_BUILDER_H
