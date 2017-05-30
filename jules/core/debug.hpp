// Copyright (c) 2017 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_CORE_DEBUG_H
#define JULES_CORE_DEBUG_H

#include <debug_assert.hpp>
#include <stdexcept>

#ifndef NDEBUG
#ifndef JULES_DEBUG_LEVEL
#define JULES_DEBUG_LEVEL static_cast<unsigned>(-1) // All debug assertions
#endif
#else
#define JULES_DEBUG_LEVEL 0 // No debug assertions
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
struct fatal_module_t : debug_assert::set_level<JULES_DEBUG_LEVEL> {
  static auto handle(const debug_assert::source_location& loc, const char* expression, const char* message) noexcept
  {
    if (*expression == '\0')
      std::fprintf(stderr, "[jules] %s:%u: Unreachable code reached - %s.\n", loc.file_name, loc.line_number, message);
    else
      std::fprintf(stderr, "[jules] %s:%u: Assertion '%s' failed - %s.\n", loc.file_name, loc.line_number, expression, message);
  }
};
#endif

struct throwing_module_t : debug_assert::set_level<static_cast<unsigned>(-1)>, debug_assert::allow_exception {
  [[noreturn]] static auto handle(const debug_assert::source_location& loc, const char* expression, const char* message)
  {
    char buffer[512];

    if (*expression == '\0')
      std::snprintf(buffer, sizeof(buffer), "[jules] %s:%u: Unreachable code reached - %s.\n", loc.file_name, loc.line_number,
                    message);
    else
      std::snprintf(buffer, sizeof(buffer), "[jules] %s:%u: Assertion '%s' failed - %s.\n", loc.file_name, loc.line_number,
                    expression, message);

    throw std::logic_error{buffer};
  }
};

#ifndef JULES_DEBUG_THROWS
static const auto default_module = fatal_module_t{};
#else
static const auto default_module = throwing_module_t{};
#endif

static const auto throwing_module = throwing_module_t{};

} // namespace debug
} // namespace jules

#endif // JULES_CORE_DEBUG_H
