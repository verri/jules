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

  /// \group member_types Class Types
  using value_type = T;

  /// \group member_types Class Types
  using allocator_type = Allocator;

  /// \group member_types Class Types
  using size_type = typename container_type::size_type;

  /// \group member_types Class Types
  using difference_type = typename container_type::difference_type;

  /// \group member_types Class Types
  using const_reference = typename container_type::const_reference;

  /// \group member_types Class Types
  using const_iterator = typename container_type::const_iterator;

  /// \group member_types Class Types
  using const_reverse_iterator = typename container_type::const_reverse_iterator;

  ~const_vector() = default;

  /// \group constructors Constructors
  /// Constructs a constant vector:
  ///
  /// (1) by forwarding the parameters to the underlying standard vector.
  ///
  /// (2) explicitly by forwarding one parameter to the underlying standard vector.
  ///
  /// (3) from a standard vector.
  ///
  /// (4) from a initializer list.
  ///
  /// (5-6) from another vector.
  template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>>
  const_vector(Args&&... args) : data_{std::make_shared<container_type>(std::forward<Args>(args)...)}
  {
  }

  /// \group constructors Constructors
  template <typename Arg> explicit const_vector(Arg&& arg) : data_{std::make_shared<container_type>(std::forward<Arg>(arg))} {}

  /// \group constructors Constructors
  const_vector(container_type source) : data_{std::make_shared<container_type>(std::move(source))} {}

  /// \group constructors Constructors
  const_vector(std::initializer_list<value_type> init, const allocator_type& alloc = {})
    : data_{std::make_shared<container_type>(init, alloc)}
  {
  }

  /// \group constructors Constructors
  const_vector(const const_vector& source) = default;

  /// \group constructors Constructors
  const_vector(const_vector&& source) noexcept = default;

  /// \group assignment Assignment
  ///
  /// (1) Copy assignment.
  ///
  /// (2) Move assignment.
  auto operator=(const const_vector& source) -> const_vector& = default;

  /// \group assignment Assignment
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

  /// \group access Element access
  auto operator[](size_type pos) const -> const_reference { return (*data_)[pos]; }

  /// \group access Element access
  auto front() const -> const_reference { return data_->front(); }

  /// \group access Element access
  auto back() const -> const_reference { return data_->back(); }

  /// \group access Element access
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

  /// \group iterator Iterators
  auto cbegin() const -> const_iterator { return data_->cbegin(); }

  /// \group iterator Iterators
  auto end() const -> const_iterator { return cend(); }

  /// \group iterator Iterators
  auto cend() const -> const_iterator { return data_->cend(); }

  /// \group iterator Iterators
  auto rbegin() const -> const_iterator { return crbegin(); }

  /// \group iterator Iterators
  auto crbegin() const -> const_iterator { return data_->crbegin(); }

  /// \group iterator Iterators
  auto rend() const -> const_iterator { return crend(); }

  /// \group iterator Iterators
  auto crend() const -> const_iterator { return data_->crend(); }

  /// \group capacity Capacity
  ///
  /// (1) checks whether the container is empty.
  ///
  /// (2) returns the number of elements.
  auto empty() const -> bool { return data_->empty(); }

  /// \group capacity Capacity
  auto size() const -> size_type { return data_->size(); }

private:
  std::shared_ptr<container_type> data_;
};
}

#endif // JULES_BASE_CONST_VECTOR_H
