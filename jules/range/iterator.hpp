#ifndef JULES_RANGE_ITERATOR_H
#define JULES_RANGE_ITERATOR_H

#include <iterator>
#include <type_traits>
#include <utility>

namespace jules
{
namespace detail
{
template <typename F> class generator : public std::iterator<std::forward_iterator_tag, std::result_of_t<F()>>
{
  public:
    generator(F&& f) : f_{std::forward<F>(f)} {}

    auto operator*() const { return f_(); }

    auto operator++(int) -> generator<F> & { return *this; }
    auto operator++() -> generator<F> & { return *this; }

  private:
    F f_;
};

} // namespace detail

template <typename F> auto as_iterator(F&& f) -> detail::generator<F> { return {std::forward<F>(f)}; }

} // namespace jules

#endif // JULES_RANGE_ITERATOR_H
