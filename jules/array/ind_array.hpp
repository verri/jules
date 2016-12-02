#ifndef JULES_ARRAY_IND_ARRAY_H
#define JULES_ARRAY_IND_ARRAY_H

#include <jules/array/detail/iterator.hpp>
#include <jules/array/detail/slicing.hpp>
#include <jules/array/slice.hpp>
#include <jules/core/debug.hpp>
#include <jules/core/type.hpp>

#include <vector>

namespace jules
{

/// Array indirect reference.
///
/// This class is used internally by `jules` to represent a view of an concrete array with
/// arbitrary positions in each dimension.
///
/// \module N-Dimensional Array
template <typename T, std::size_t N> class ind_array
{
  static_assert(N > 0u, "invalid array dimension");

  template <typename, std::size_t> friend class ind_array;

public:
  using vector_type = std::vector<index_t>;
  using value_type = T;

  static constexpr auto order = N;

  using size_type = index_t;
  using difference_type = distance_t;

  using iterator = detail::iterator_from_indexes<T, typename vector_type::iterator>;
  using const_iterator = detail::iterator_from_indexes<const T, typename vector_type::iterator>;

  /// *TODO*: Explain why the user should probably not call this function.
  /// In C++17, we can provide a helper that generates a view with more security.
  ind_array(T* data, std::array<index_t, N> extents, vector_type indexes)
    : data_{data}, descriptor_{0ul, extents}, indexes_(std::move(indexes))
  {
  }

  ind_array(const ind_array& source) = delete;
  ind_array(ind_array&& source) noexcept = delete;

  ~ind_array() = default;

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> detail::not_array_request<ind_array&, U>
  {
    static_assert(std::is_assignable<T&, U>::value, "incompatible assignment");
    for (auto& elem : *this)
      elem = source;
    return *this;
  }

  /// \group Assignment
  template <typename Array> auto operator=(const Array& source) -> detail::array_request<ind_array&, Array>
  {
    static_assert(Array::order == N, "array order mismatch");
    static_assert(std::is_assignable<T&, typename Array::value_type>::value, "incompatible assignment");

    DEBUG_ASSERT(this->extents() == source.extents(), debug::module{}, debug::level::extents_check, "extents mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::module{}, debug::level::unreachable, "should never happen");

    return *this;
  }

  /// \group Assignment
  auto operator=(base_array<T, N>&& source) noexcept -> ind_array&
  {
    auto it = source.begin();
    for (auto& elem : *this)
      elem = std::move(*it++);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator ind_array<const T, N>() const { return {data_, descriptor_, indexes_}; }

  /// \group Indexing
  auto operator[](index_t i) -> ind_array<T, N - 1>
  {
    DEBUG_ASSERT(i <= row_count(), debug::module{}, debug::level::boundary_check, "out of range");
    auto row_info = row(i);
    return {data_, row_info.first, std::move(row_info.second)};
  }

  /// \group Indexing
  auto operator[](index_t i) const -> ind_array<const T, N - 1>
  {
    // It is very expensive to convert to ind_array<const T, N>.
    DEBUG_ASSERT(i <= row_count(), debug::module{}, debug::level::boundary_check, "out of range");
    auto row_info = row(i);
    return {data_, row_info.first, std::move(row_info.second)};
  }

  template <typename... Args> auto operator()(Args&&... args) -> detail::indirect_request<ind_array<T, N>, Args...>
  {
    static_assert(sizeof...(args) == N, "invalid number of arguments");
    auto slicing = detail::indirect_slicing(descriptor_, std::forward<Args>(args)...);
    for (auto& v : slicing.second)
      v = indexes_[v];
    return {data_, slicing.first, std::move(slicing.second)};
  }

  template <typename... Args> auto operator()(Args&&... args) -> detail::slice_request<ind_array<T, N>, Args...>
  {
    auto slice = detail::default_slicing(descriptor_, std::forward<Args>(args)...);
    auto indexes = vector_type();

    indexes.reserve(slice.size());
    for (auto j : slice)
      indexes.push_back(indexes_[j]);

    return {data_, slice.extents, std::move(indexes)};
  }

  template <typename... Args> auto operator()(Args&&... args) -> detail::element_request<T&, Args...>
  {
    static_assert(sizeof...(args) == N, "invalid number of arguments");
    // TODO: check boundaries
    return data_[indexes_[descriptor_(index_t{std::forward<Args>(args)}...)]];
  }

  template <typename... Args> auto operator()(Args&&... args) const -> detail::indirect_request<ind_array<const T, N>, Args...>
  {
    static_assert(sizeof...(args) == N, "invalid number of arguments");
    auto slicing = detail::indirect_slicing(descriptor_, std::forward<Args>(args)...);
    for (auto& v : slicing.second)
      v = indexes_[v];
    return {data_, slicing.first.extents, std::move(slicing.second)};
  }

  template <typename... Args> auto operator()(Args&&... args) const -> detail::slice_request<ind_array<const T, N>, Args...>
  {
    auto slice = detail::default_slicing(descriptor_, std::forward<Args>(args)...);
    auto indexes = vector_type();

    indexes.reserve(slice.size());
    for (auto j : slice)
      indexes.push_back(indexes_[j]);

    return {data_, slice.extents, std::move(indexes)};
  }

  template <typename... Args> auto operator()(Args&&... args) const -> detail::element_request<const T&, Args...>
  {
    static_assert(sizeof...(args) == N, "invalid number of arguments");
    // TODO: check boundaries
    return data_[indexes_[descriptor_(index_t{std::forward<Args>(args)}...)]];
  }

  auto begin() -> iterator { return {data_, indexes_.begin()}; }
  auto end() -> iterator { return {data_, indexes_.end()}; }

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return {data_, indexes_.begin()}; }
  auto cend() const -> const_iterator { return {data_, indexes_.end()}; }

  auto descriptor() const { return indexes_; }
  auto data() const { return data_; }
  auto extents() const { return descriptor_.extents; }
  auto size() const { return indexes_.size(); }

  auto row_count() const { return extents()[1]; }
  auto column_count() const { return extents()[2]; }

private:
  auto row(index_t i) const
  {
    auto extents = std::array<index_t, N - 1>();
    auto indexes = vector_type();
    indexes.reserve(size() / row_count());

    std::copy(std::begin(extents()) + 1, std::end(extents()), std::begin(extents));
    std::copy(indexes_.begin() + i * row_count(), indexes_.begin() + i * row_count() + size() / row_count(),
              std::back_inserter(indexes));

    return std::make_pair(std::move(extents), std::move(indexes));
  }

  T* data_;
  base_slice<N> descriptor_;
  vector_type indexes_;
};

/// 1-D Indirect array specialization.
///
/// This class is used internally by `jules` to represent a view of an concrete array with
/// arbitrary positions in each dimension.
///
/// \module N-Dimensional Array
template <typename T> class ind_array<T, 1>
{
  template <typename, std::size_t> friend class ind_array;

public:
  using vector_type = std::vector<index_t>;
  using value_type = T;
  static constexpr auto order = 1;

  using size_type = index_t;
  using difference_type = distance_t;

  using iterator = detail::iterator_from_indexes<T, typename vector_type::iterator>;
  using const_iterator = detail::iterator_from_indexes<const T, typename vector_type::iterator>;

  /// *TODO*: Explain why the user should probably not call this function.
  /// In C++17, we can provide a helper that generates a view with more security.
  ind_array(T* data, index_t extent, vector_type indexes) : data_{data}, descriptor_{0ul, extent}, indexes_(std::move(indexes)) {}

  ind_array(const ind_array& source) = delete;
  ind_array(ind_array&& source) noexcept = delete;

  ~ind_array() = default;

  /// \group Assignment
  template <typename U> auto operator=(const U& source) -> detail::not_array_request<ind_array&, U>
  {
    static_assert(std::is_assignable<T&, U>::value, "incompatible assignment");
    for (auto& elem : *this)
      elem = source;
    return *this;
  }

  /// \group Assignment
  template <typename Array> auto operator=(const Array& source) -> detail::array_request<ind_array&, Array>
  {
    static_assert(Array::order == 1, "array order mismatch");
    static_assert(std::is_assignable<T&, typename Array::value_type>::value, "incompatible assignment");

    DEBUG_ASSERT(this->extents() == source.extents(), debug::module{}, debug::level::extents_check, "extents mismatch");
    auto it = source.begin();
    for (auto& elem : *this)
      elem = *it++;
    DEBUG_ASSERT(it == source.end(), debug::module{}, debug::level::unreachable, "should never happen");

    return *this;
  }

  /// \group Assignment
  auto operator=(base_array<T, 1>&& source) noexcept -> ind_array&
  {
    auto it = source.begin();
    for (auto& elem : *this)
      elem = std::move(*it++);
    return *this;
  }

  /// Implicitly convertable to hold const values.
  operator ind_array<const T, 1>() const { return {data_, descriptor_}; }

  /// \group Indexing
  auto operator[](index_t i) -> T&
  {
    DEBUG_ASSERT(i <= length(), debug::module{}, debug::level::boundary_check, "out of range");
    return data_[indexes_[descriptor_(i)]];
  }

  /// \group Indexing
  auto operator[](index_t i) const -> const T&
  {
    DEBUG_ASSERT(i <= length(), debug::module{}, debug::level::boundary_check, "out of range");
    return data_[indexes_[descriptor_(i)]];
  }

  template <typename Rng, typename U = range::range_value_t<Rng>, CONCEPT_REQUIRES_(range::Range<Rng>())>
  auto operator()(const Rng& rng) -> ind_array<T, 1>
  {
    static_assert(std::is_convertible<U, index_t>::value, "arbitrary ranges must contain indexes");

    auto slicing = detail::indirect_slicing(descriptor_, rng);
    auto indexes = map_indexes(slicing.second);
    return {data_, slicing.first.extent, std::move(indexes)};
  }

  auto operator()(const base_slice<1>& slice) -> ind_array<T, 1>
  {
    auto new_slice = detail::default_slicing(descriptor_, slice);
    auto indexes = map_indexes(new_slice);
    return {data_, new_slice.extent, std::move(indexes)};
  }

  auto operator()(index_t i) -> T& { return (*this)[i]; }

  template <typename Rng, typename U = range::range_value_t<Rng>, CONCEPT_REQUIRES_(range::Range<Rng>())>
  auto operator()(const Rng& rng) const -> ind_array<const T, 1>
  {
    static_assert(std::is_convertible<U, index_t>::value, "arbitrary ranges must contain indexes");

    auto slicing = detail::indirect_slicing(descriptor_, rng);
    auto indexes = map_indexes(slicing.second);
    return {data_, slicing.first.extent, std::move(indexes)};
  }

  auto operator()(const base_slice<1>& slice) const -> ind_array<const T, 1>
  {
    auto new_slice = detail::default_slicing(descriptor_, slice);
    auto indexes = map_indexes(new_slice);
    return {data_, new_slice.extent, std::move(indexes)};
  }

  auto operator()(index_t i) const -> const T& { return (*this)[i]; }

  auto begin() -> iterator { return {data_, indexes_.begin()}; }
  auto end() -> iterator { return {data_, indexes_.end()}; }

  auto begin() const -> const_iterator { return cbegin(); }
  auto end() const -> const_iterator { return cend(); }

  auto cbegin() const -> const_iterator { return {data_, indexes_.begin()}; }
  auto cend() const -> const_iterator { return {data_, indexes_.end()}; }

  auto descriptor() const { return indexes_; }
  auto data() const { return data_; }
  auto extents() const { return descriptor_.extent; }
  auto size() const { return indexes_.size(); }
  auto length() const { return size(); }

private:
  template <typename Range> auto map_indexes(const Range& rng) const
  {
    auto indexes = vector_type();
    indexes.reserve(rng.size());
    for (index_t j : rng)
      indexes.push_back(indexes_[j]);
    return indexes;
  }

  T* data_;
  base_slice<1> descriptor_;
  vector_type indexes_;
};

} // namespace jules

#endif // JULES_ARRAY_IND_ARRAY_H
