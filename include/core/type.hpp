#ifndef JULES_CORE_TYPE_H
#define JULES_CORE_TYPE_H

#include <type_traits>
#include <iterator>
#include <string>

namespace jules
{
struct numeric_rules {
    using value_type = double;
    static value_type coerce_from(const std::string& value) { return std::stod(value); }
};

struct default_coercion_rules {
    using numeric_t = double;
    using string_t = std::string;

    static string_t coerce_to_string(numeric_t value) { return std::to_string(value); }
    static numeric_t coerce_to_numeric(string_t value) { return std::stod(value); }
};

template <typename... Rules> struct coercion_traits {
    // auto
};

using default_coercion_traits = coercion_traits<numeric_rules>;

template <typename R, typename I = decltype(std::begin(R{}))>
struct range_traits : public std::iterator_traits<I> {
    using iterator_type = I;
    static_assert(std::is_same<iterator_type, decltype(std::end(R{}))>::value,
                  "begin and end iteratores do not match");
};

} // namespace jules

#endif // JULES_CORE_TYPE_H
