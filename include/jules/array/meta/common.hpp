// Copyright (c) 2017-2019 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_COMMON_H
/// \exclude
#define JULES_ARRAY_META_COMMON_H

#include <jules/core/meta.hpp>
#include <jules/core/ranges.hpp>
#include <jules/core/type.hpp>

namespace jules
{
namespace meta::result
{
template <typename T> using size = decltype(std::declval<const T&>().size());
template <typename T> using dimensions = decltype(std::declval<const T&>().dimensions());
template <typename T> using length = decltype(std::declval<const T&>().length());
template <typename T> using row_count = decltype(std::declval<const T&>().row_count());
template <typename T> using column_count = decltype(std::declval<const T&>().column_count());
template <typename T> using eval = decltype(eval(std::declval<const T&>()));
} // namespace meta::result

template <typename T, typename = void> struct CommonArray : std::false_type
{};

template <typename T>
struct CommonArray<                                                                                    //
  T, meta::requires_<                                                                                  //
       meta::always_true<typename T::value_type>,                                                      //
       std::is_same<decltype(T::order), const std::size_t>,                                            //
       std::bool_constant<(T::order > 0u)>,                                                            //
       meta::compiles_same<T, typename T::size_type, meta::result::size>,                              //
       meta::compiles_same<T, std::array<typename T::size_type, T::order>, meta::result::dimensions>,  //
       meta::compiles<T, meta::result::eval>,                                                          //
       std::conditional_t<(T::order == 1u),                                                            //
                          meta::compiles_same<T, typename T::size_type, meta::result::length>,         //
                          std::conjunction<                                                            //
                            meta::compiles_same<T, typename T::size_type, meta::result::row_count>,    //
                            meta::compiles_same<T, typename T::size_type, meta::result::column_count>> //
                          >,                                                                           //
       std::bool_constant<ranges::range<T>>                                                            //
       >> : std::true_type
{};

template <typename Derived> class common_array_base
{
public:
  [[nodiscard]] auto size() const noexcept { return self().size(); }

  [[nodiscard]] auto dimensions() const noexcept { return self().dimensions(); }

  [[nodiscard]] auto length() const noexcept { return self().length(); }

  [[nodiscard]] auto row_count() const noexcept { return self().row_count(); }

  [[nodiscard]] auto column_count() const noexcept { return self().column_count(); }

  [[nodiscard]] auto begin() noexcept { return self().begin(); }

  [[nodiscard]] auto end() noexcept { return self().end(); }

  [[nodiscard]] auto begin() const noexcept { return self().begin(); }

  [[nodiscard]] auto end() const noexcept { return self().end(); }

  [[nodiscard]] auto cbegin() const noexcept { return self().cbegin(); }

  [[nodiscard]] auto cend() const noexcept { return self().cend(); }

  operator const Derived&() const { return self(); }

  operator Derived&() { return self(); }

protected:
  constexpr common_array_base() noexcept = default;

private:
  constexpr auto self() const noexcept -> const Derived& { return static_cast<const Derived&>(*this); }
  constexpr auto self() noexcept -> Derived& { return static_cast<Derived&>(*this); }
};

} // namespace jules

#endif // JULES_ARRAY_META_COMMON_H
