#ifndef JULES_FORMULA_FORMULA_DECL_H
#define JULES_FORMULA_FORMULA_DECL_H

#include "dataframe/dataframe.hpp"
#include "formula/detail/term.hpp"

#include <memory>
#include <string>

namespace jules
{

template <typename Coercion, typename T = void> class base_term : public detail::term_eraser<Coercion>
{
  private:
    using term_eraser_t = detail::term_eraser<Coercion>;
    using dataframe_t = base_dataframe<Coercion>;
    using column_t = base_column<Coercion>;

  public:
    explicit base_term(const std::string& colname);

    template <typename F> base_term apply(F&& f) const;

    template <typename U> base_term operator+(const U& operand) const;
    template <typename U> base_term operator-(const U& operand) const;
    template <typename U> base_term operator/(const U& operand) const;
    template <typename U> base_term operator*(const U& operand) const;
    template <typename U> base_term operator-() const;

    template <typename U> base_term operator+(const base_term<Coercion, U>& operand) const;
    template <typename U> base_term operator-(const base_term<Coercion, U>& operand) const;
    template <typename U> base_term operator/(const base_term<Coercion, U>& operand) const;
    template <typename U> base_term operator*(const base_term<Coercion, U>& operand) const;

  protected:
    virtual std::unique_ptr<term_eraser_t> clone() const;
    virtual column_t& coerce_and_apply_modifiers(const dataframe_t& data) const;

  private:
    explicit base_term(std::function<column_t&(const dataframe_t& data)> f) : f_{f} {}
    std::function<column_t&(const dataframe_t& data)> f_;
};

template <typename Coercion> class base_term<Coercion, void> : public detail::term_eraser<Coercion>
{
  private:
    using term_eraser_t = detail::term_eraser<Coercion>;
    using column_t = base_column<Coercion>;

};

template  <typename Coercion> class base_formula
{
  private:
    using dataframe_t = base_dataframe<Coercion>;
    using term_list_t = detail::term_list<Coercion>;

  public:
    base_formula(const term_list_t& lhs, const term_list_t& rhs) : lhs_{lhs}, rhs_{rhs} {}

    dataframe_t lhs_from(const dataframe_t& data) const;
    dataframe_t rhs_from(const dataframe_t& data) const;

  private:
    term_list_t lhs_, rhs_;
};

template <typename T = void> using term = base_term<default_coercion_rules, T>;
using formula = base_formula<default_coercion_rules>;

} // namespace jules

#endif // JULES_FORMULA_FORMULA_DECL_H
