// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_DEBUG_H
#define JULES_CORE_DEBUG_H

#include <debug_assert.hpp>

#ifndef NDEBUG
#ifndef JULES_DEBUG_LEVEL
#define JULES_DEBUG_LEVEL static_cast<unsigned>(-1) // All debug assertions
#endif
#else
#define JULES_DEBUG_LEVEL 0 // No debug assertions
#endif

#ifdef JULES_DEBUG_THROWS
#include <stdexcept>
#endif

namespace jules
{
namespace debug
{

/// Debug level for each kind of checking.
namespace level
{
constexpr auto unreachable = debug_assert::level<1u>{};
constexpr auto invalid_argument = debug_assert::level<2u>{};
constexpr auto invalid_state = debug_assert::level<2u>{};
constexpr auto boundary_check = debug_assert::level<3u>{};
constexpr auto extents_check = debug_assert::level<3u>{};
} // namespace level

#ifndef JULES_DEBUG_THROWS
struct module : debug_assert::default_handler, debug_assert::set_level<JULES_DEBUG_LEVEL> {
};
#else
struct module : debug_assert::set_level<JULES_DEBUG_LEVEL> {
  [[noreturn]] static auto handle(const debug_assert::source_location& loc, const char* expression, const char* message = nullptr)
  {
    char buffer[512];

    if (*expression == '\0') {
      if (message)
        std::snprintf(buffer, sizeof(buffer), "[debug assert] %s:%u: Unreachable code reached - %s.\n", loc.file_name,
                      loc.line_number, message);
      else
        std::snprintf(buffer, sizeof(buffer), "[debug assert] %s:%u: Unreachable code reached.\n", loc.file_name,
                      loc.line_number);
    } else if (message)
      std::snprintf(buffer, sizeof(buffer), "[debug assert] %s:%u: Assertion '%s' failed - %s.\n", loc.file_name, loc.line_number,
                    expression, message);
    else
      std::snprintf(buffer, sizeof(buffer), "[debug assert] %s:%u: Assertion '%s' failed.\n", loc.file_name, loc.line_number,
                    expression);

    throw std::logic_error{buffer};
  }
};
#endif

} // namespace debug
} // namespace jules

#endif // JULES_CORE_DEBUG_H
