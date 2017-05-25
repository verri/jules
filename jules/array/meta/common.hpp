// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_ARRAY_META_COMMON_H
/// \exclude
#define JULES_ARRAY_META_COMMON_H

#include <jules/core/meta.hpp>
#include <jules/core/range.hpp>
#include <jules/core/type.hpp>

namespace jules
{
namespace meta
{
namespace result
{
template <typename T> using size = decltype(std::declval<const T&>().size());
template <typename T> using dimensions = decltype(std::declval<const T&>().dimensions());
template <typename T> using length = decltype(std::declval<const T&>().length());
template <typename T> using row_count = decltype(std::declval<const T&>().row_count());
template <typename T> using column_count = decltype(std::declval<const T&>().column_count());
template <typename T> using eval = decltype(eval(std::declval<const T&>()));
} // namespace result
} // namespace meta

template <typename T, typename = void> struct CommonArray : std::false_type {
};

template <typename T>
struct CommonArray<                                                                                    //
  T, meta::requires<                                                                                   //
       meta::always_true<typename T::value_type>,                                                      //
       std::is_same<decltype(T::order), const std::size_t>,                                            //
       std::bool_constant<(T::order > 0ul)>,                                                           //
       meta::compiles_same<T, typename T::size_type, meta::result::size>,                              //
       meta::compiles_same<T, std::array<typename T::size_type, T::order>, meta::result::dimensions>,  //
       meta::compiles<T, meta::result::eval>,                                                          //
       std::conditional_t<(T::order == 1ul),                                                           //
                          meta::compiles_same<T, typename T::size_type, meta::result::length>,         //
                          std::conjunction<                                                            //
                            meta::compiles_same<T, typename T::size_type, meta::result::row_count>,    //
                            meta::compiles_same<T, typename T::size_type, meta::result::column_count>> //
                          >,                                                                           //
       range::Range<T>                                                                                 //
       >> : std::true_type {
};

template <typename Derived> class common_array_base
{
public:
  auto size() const { return self().size(); }

  auto dimensions() const { return self().dimensions(); }

  auto length() const { return self().length(); }

  auto row_count() const { return self().row_count(); }

  auto column_count() const { return self().column_count(); }

  auto begin() { return self().begin(); }

  auto end() { return self().end(); }

  auto begin() const { return self().begin(); }

  auto end() const { return self().end(); }

  auto cbegin() const { return self().cbegin(); }

  auto cend() const { return self().cend(); }

protected:
  common_array_base() = default;

private:
  auto self() const -> const Derived& { return static_cast<const Derived&>(*this); }
  auto self() -> Derived& { return static_cast<Derived&>(*this); }
};

} // namespace jules

#endif // JULES_ARRAY_META_COMMON_H
