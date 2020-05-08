// Copyright (c) 2020 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_STRING_H
/// \exclude
#define JULES_CORE_STRING_H

#include <algorithm>
#include <array>
#include <iosfwd>
#include <memory>
#include <string_view>

namespace jules
{

template <std::size_t N> class sso_string
{
  static_assert(N > 0);

public:
  constexpr sso_string() noexcept = default;

  constexpr ~sso_string() noexcept
  {
    if (!is_small())
      delete[] data();
  }

  constexpr sso_string(const sso_string& source) : size_(source.size())
  {
    if (!is_small())
      data_.big_string = new char[size()];
    std::copy_n(source.data(), source.size(), data());
  }

  constexpr sso_string(sso_string&& source) noexcept : size_(source.size())
  {
    if (is_small())
      std::copy_n(source.data(), source.size(), data());
    else
      data_.big_string = std::exchange(source.data_.big_string, nullptr);
  }

  constexpr sso_string(std::string_view source) : size_(source.size())
  {
    if (!is_small())
      data_.big_string = new char[size()];
    std::copy_n(source.data(), source.size(), data());
  }

  template <std::size_t M> constexpr sso_string(const char (&source)[M]) : sso_string(std::string_view{source}) {}

  constexpr sso_string(const std::string& source) : sso_string(std::string_view{source}) {}

  constexpr auto operator=(const sso_string& source) -> sso_string&
  {
    if (this == &source)
      return *this;

    *this = source.view();
    return *this;
  }

  constexpr auto operator=(sso_string&& source) noexcept -> sso_string&
  {
    if (this == &source)
      return *this;
    swap(source);
    return *this;
  }

  constexpr auto operator=(std::string_view source) -> sso_string&
  {
    if (source.size() <= N) {
      if (!is_small())
        delete[] source.data();
      size_ = source.size();
      std::copy_n(source.data(), source.size(), data());
      return *this;
    }

    if (is_small())
      data_.big_string = new char[source.size()];

    size_ = source.size();
    std::copy_n(source.data(), source.size(), data());
    return *this;
  }

  [[nodiscard]] constexpr auto view() const noexcept -> std::string_view { return {data(), size()}; }

  constexpr operator std::string_view() const noexcept { return view(); }

  constexpr auto swap(sso_string& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
  };

  // TODO constexpr auto operator<=>(const sso_string& other) const noexcept { return view() <=> other.view(); }

  constexpr auto operator==(const sso_string& other) const noexcept -> bool { return view() == other.view(); }
  constexpr auto operator!=(const sso_string& other) const noexcept -> bool { return view() != other.view(); }

  constexpr auto operator<(const sso_string& other) const noexcept { return view() < other.view(); }
  constexpr auto operator>(const sso_string& other) const noexcept { return view() > other.view(); }
  constexpr auto operator>=(const sso_string& other) const noexcept { return view() >= other.view(); }
  constexpr auto operator<=(const sso_string& other) const noexcept { return view() <= other.view(); }

private:
  [[nodiscard]] constexpr auto size() const noexcept { return size_; }

  [[nodiscard]] constexpr auto is_small() const noexcept { return size() <= N; }

  [[nodiscard]] constexpr auto data() const noexcept -> const char* { return is_small() ? data_.small_string : data_.big_string; }

  [[nodiscard]] constexpr auto data() noexcept -> char* { return is_small() ? data_.small_string : data_.big_string; }

  std::size_t size_ = 0;
  union {
    char small_string[N];
    char* big_string;
  } data_ = {};
};

template <std::size_t N> auto operator<<(std::ostream& os, const sso_string<N>& str) -> std::ostream& { return os << str.view(); }

template <std::size_t N> auto operator>>(std::istream& is, sso_string<N>& str) -> std::istream&
{
  std::string tmp;
  is >> tmp;
  str = tmp;
  return is;
}

} // namespace jules

template <std::size_t N> struct std::hash<jules::sso_string<N>>
{
  auto operator()(const jules::sso_string<N>& str) const noexcept { return std::hash<std::string_view>{}(str.view()); }
};

#endif // JULES_CORE_STRING_H
