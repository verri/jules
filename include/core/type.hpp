#ifndef JULES_CORE_TYPE_H
#define JULES_CORE_TYPE_H

#include <type_traits>
#include <iterator>
#include <string>

namespace jules
{
struct default_coercion_rules {
    using numeric_t = double;
    using string_t = std::string;

    static string_t coerce_to_string(numeric_t value) { return std::to_string(value); }
    static numeric_t coerce_to_numeric(string_t value) { return std::stod(value); }
};

template <typename R, typename I = decltype(std::begin(R{}))>
struct range_traits : public std::iterator_traits<I> {
    using iterator_type = I;
    static_assert(std::is_same<iterator_type, decltype(std::end(R{}))>::value,
                  "begin and end iteratores do not match");
};

} // namespace jules

#endif // JULES_CORE_TYPE_H
