#ifndef JULES_FORMULA_DETAIL_TERM_DECL_H
#define JULES_FORMULA_DETAIL_TERM_DECL_H

#include "dataframe/dataframe.hpp"

#include <memory>
#include <string>
#include <vector>

namespace jules
{
namespace detail
{

template <typename Coercion> class term_eraser
{
  private:
    using dataframe_t = base_dataframe<Coercion>;
    using column_t = base_column<Coercion>;

  public:
    virtual ~term_eraser() = default;

    column_t extract_from(const dataframe_t& data) const;

  protected:
    virtual std::unique_ptr<term_eraser> clone() const = 0;
    virtual column_t& coerce_and_apply_modifiers(const dataframe_t& data) const = 0;
};

template <typename Coercion> class term_list
{
  private:
    using term_t = term_eraser<Coercion>;
    using term_ptr_t = std::unique_ptr<term_t>;

  public:
    explicit term_list(const term_t& term) : terms{term_ptr_t{term.clone()}} {}

    void append(const term_t& term) { terms.push_back(term_ptr_t{term.clone()}); }

  private:
    std::vector<term_ptr_t> terms;
};

} // namespace detail
} // namespace jules

#endif // JULES_FORMULA_DETAIL_TERM_DECL_H
