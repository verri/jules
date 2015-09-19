#ifndef JULES_CORE_TYPE_H
#define JULES_CORE_TYPE_H

#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

namespace jules
{
template <typename T> struct tag {
    using untag = T;
};

struct numeric_rules {
    using type = double;
    static type coerce_from(const std::string& value) { return std::stod(value); }
};

struct string_rules {
    using type = std::string;
    static type coerce_from(double value) { return std::to_string(value); }
};

template <typename... Rules> class coercion_rules
{
  private:
    using types = std::tuple<typename Rules::type...>;
    using rules = std::tuple<Rules...>;

  public:
    template <std::size_t I> using type = typename std::tuple_element<I, types>::type;
    static constexpr std::size_t ntypes() { return sizeof...(Rules); }

    template <std::size_t I> using rule = typename std::tuple_element<I, rules>::type;
    static constexpr std::size_t nrules() { return sizeof...(Rules); }
};

using default_coercion_rules = coercion_rules<numeric_rules, string_rules>;

template <typename F, typename Enabler = void> struct is_callable {
    static constexpr bool value = false;
};

template <typename F>
struct is_callable<F, typename std::enable_if_t<std::is_same<void, decltype(std::declval<F>()())>::value>> {
    static constexpr bool value = true;
};

} // namespace jules

#endif // JULES_CORE_TYPE_H
