// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

// My soul melts away for sorrow;
//   strengthen me according to your word!
// Psalm 119:28 (ESV)

#ifndef JULES_BASE_CONST_VECTOR_H
/// \exclude
#define JULES_BASE_CONST_VECTOR_H

#include <memory>
#include <type_traits>
#include <vector>

namespace jules
{

/// Sequence container with constant size and elements.
///
/// The elements are stored contiguously and cannot be changed after
/// the vector construction.
///
/// \notes Unused memory is always freed.
template <typename T, typename Allocator = typename std::vector<T>::allocator_type> class const_vector
{
public:
  /// \group member_types Class Types
  using container_type = std::vector<T, Allocator>;

  /// \group member_types
  using value_type = T;

  /// \group member_types
  using allocator_type = Allocator;

  /// \group member_types
  using size_type = typename container_type::size_type;

  /// \group member_types
  using difference_type = typename container_type::difference_type;

  /// \group member_types
  using const_reference = typename container_type::const_reference;

  /// \group member_types
  using const_iterator = typename container_type::const_iterator;

  /// \group member_types
  using const_reverse_iterator = typename container_type::const_reverse_iterator;

  ~const_vector() = default;

  /// \group constructors Constructors
  /// Constructs a constant vector:
  ///
  /// (1) with no elements.
  ///
  /// (2) by forwarding the parameters to the underlying standard vector.
  ///
  /// (3) explicitly by forwarding one parameter to the underlying standard vector.
  ///
  /// (4) from a standard vector.
  ///
  /// (5) from a initializer list.
  ///
  /// (6-7) from another vector.
  const_vector() : data_{std::make_shared<container_type>()} {}

  /// \group constructors
  template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>>
  const_vector(Args&&... args) : data_{std::make_shared<container_type>(std::forward<Args>(args)...)}
  {
  }

  /// \group constructors
  template <typename Arg> explicit const_vector(Arg&& arg) : data_{std::make_shared<container_type>(std::forward<Arg>(arg))} {}

  /// \group constructors
  const_vector(container_type source) : data_{std::make_shared<container_type>(std::move(source))} {}

  /// \group constructors
  const_vector(std::initializer_list<value_type> init, const allocator_type& alloc = {})
    : data_{std::make_shared<container_type>(init, alloc)}
  {
  }

  /// \group constructors
  const_vector(const const_vector& source) = default;

  /// \group constructors
  const_vector(const_vector&& source) noexcept = default;

  /// \group assignment Assignment
  ///
  /// (1) Copy assignment.
  ///
  /// (2) Move assignment.
  auto operator=(const const_vector& source) -> const_vector& = default;

  /// \group assignment
  auto operator=(const_vector&& source) noexcept -> const_vector& = default;

  /// TODO: assign and get_allocator

  /// \group access Element access
  ///
  /// Accessing elements.
  ///
  /// (1) Access specified element with bounds checking.
  ///
  /// (2) Access specified element.
  ///
  /// (3) Access the first element.
  ///
  /// (4) Access the last element.
  ///
  /// (5) Direct access to the underlying array.
  auto at(size_type pos) const -> const_reference { return data_->at(pos); }

  /// \group access
  auto operator[](size_type pos) const -> const_reference { return (*data_)[pos]; }

  /// \group access
  auto front() const -> const_reference { return data_->front(); }

  /// \group access
  auto back() const -> const_reference { return data_->back(); }

  /// \group access
  auto data() const -> const value_type* { return data_->data(); }

  /// \group iterator Iterators
  ///
  /// (1-2) returns an iterator to the beginning.
  ///
  /// (3-4) returns an iterator to the end.
  ///
  /// (5-6) returns a reverse iterator to the beginning.
  ///
  /// (7-8) returns a reverse iterator to the end.
  auto begin() const -> const_iterator { return cbegin(); }

  /// \group iterator
  auto cbegin() const -> const_iterator { return data_->cbegin(); }

  /// \group iterator
  auto end() const -> const_iterator { return cend(); }

  /// \group iterator
  auto cend() const -> const_iterator { return data_->cend(); }

  /// \group iterator
  auto rbegin() const -> const_iterator { return crbegin(); }

  /// \group iterator
  auto crbegin() const -> const_iterator { return data_->crbegin(); }

  /// \group iterator
  auto rend() const -> const_iterator { return crend(); }

  /// \group iterator
  auto crend() const -> const_iterator { return data_->crend(); }

  /// \group capacity Capacity
  ///
  /// (1) checks whether the container is empty.
  ///
  /// (2) returns the number of elements.
  auto empty() const -> bool { return data_->empty(); }

  /// \group capacity
  auto size() const -> size_type { return data_->size(); }

  /// \group comparison Comparison
  /// Lexicographically compares the values in the vector.
  auto operator==(const const_vector& other) const -> bool { return *data_ == *other.data_; }

  /// \group comparison
  auto operator!=(const const_vector& other) const -> bool { return *data_ != *other.data_; }

  /// \group comparison
  auto operator<(const const_vector& other) const -> bool { return *data_ < *other.data_; }

  /// \group comparison
  auto operator<=(const const_vector& other) const -> bool { return *data_ <= *other.data_; }

  /// \group comparison
  auto operator>(const const_vector& other) const -> bool { return *data_ > *other.data_; }

  /// \group comparison
  auto operator>=(const const_vector& other) const -> bool { return *data_ >= *other.data_; }

private:
  std::shared_ptr<container_type> data_;
};
}

#endif // JULES_BASE_CONST_VECTOR_H
