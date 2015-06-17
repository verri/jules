#ifndef JULES_CORE_TYPE_H
#define JULES_CORE_TYPE_H

#include <string>
#include <vector>

namespace jules
{
using numeric_t = double;
using string_t = std::string;
template <typename T> using vector_t = std::vector<T>;

string_t coerse_to_string(numeric_t value) { return std::to_string(value); }

} // namespace jules

#endif // JULES_CORE_TYPE_H
