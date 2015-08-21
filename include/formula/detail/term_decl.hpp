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

template <typename Coercion> class any_term;

template <typename Coercion> class term_model
{
  public:
    friend class any_term<Coercion>;

  private:
    using dataframe_t = base_dataframe<Coercion>;
    using column_t = base_column<Coercion>;

  public:
    virtual ~term_model() = default;
    virtual column_t extract_from(const dataframe_t& data) const = 0;

  protected:
    virtual std::unique_ptr<term_model> clone() const = 0;
};

template <typename Coercion> class base_any_term
{

};

} // namespace detail
} // namespace jules

#endif // JULES_FORMULA_DETAIL_TERM_DECL_H
