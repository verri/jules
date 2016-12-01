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

namespace jules
{
namespace debug
{

/// Debug level for each kind of checking.
namespace level
{
constexpr auto invalid_argument = debug_assert::level<1u>{};
constexpr auto invalid_state = debug_assert::level<1u>{};
constexpr auto boundary_check = debug_assert::level<2u>{};
constexpr auto incompatible_comparison = debug_assert::level<3u>{};
} // namespace level

struct module :
  debug_assert::default_handler,
  debug_assert::set_level<JULES_DEBUG_LEVEL> {};

} // namespace debug
} // namespace jules

#endif // JULES_CORE_DEBUG_H
