#ifndef JULES_FORMULA_DETAIL_TERM_H
#define JULES_FORMULA_DETAIL_TERM_H

#include "formula/detail/term_decl.hpp"

namespace jules
{
namespace detail
{

template <typename Coercion>
auto term_eraser<Coercion>::extract_from(const dataframe_t& data) const -> column_t {
    return coerce_and_apply_modifiers(data);
}

} // namespace detail
} // namespace jules

#endif // JULES_FORMULA_DETAIL_TERM_H
