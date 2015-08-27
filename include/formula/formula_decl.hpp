#ifndef JULES_FORMULA_FORMULA_DECL_H
#define JULES_FORMULA_FORMULA_DECL_H

#include "dataframe/dataframe.hpp"

#include <memory>
#include <string>

namespace jules
{
template <typename Coercion, typename T = void> class base_expr
{
  private:
    using expr_t = base_expr<Coercion, void>;
    using dataframe_t = base_dataframe<Coercion>;
    using column_t = base_column<Coercion>;

  public:
    explicit base_expr(const std::string& colname);

    base_expr(const base_expr& source) = default;
    base_expr(base_expr&& source) = default;

    base_expr& operator=(const base_expr& source) = default;
    base_expr& operator=(base_expr&& source) = default;

    template <typename F> base_expr apply(F&& f) const;

    template <typename U> auto operator+(const U& operand) const;
    template <typename U> auto operator-(const U& operand) const;
    template <typename U> auto operator/(const U& operand) const;
    template <typename U> auto operator*(const U& operand) const;
    template <typename U> auto operator-() const;

    template <typename C, typename V, typename U>
    friend base_expr<C, decltype(U{} + V{})> operator+(const U& operand, const base_expr<C, V>& expr);
    template <typename C, typename V, typename U>
    friend base_expr<C, decltype(U{} - V{})> operator-(const U& operand, const base_expr<C, V>& expr);
    template <typename C, typename V, typename U>
    friend base_expr<C, decltype(U{} / V{})> operator/(const U& operand, const base_expr<C, V>& expr);
    template <typename C, typename V, typename U>
    friend base_expr<C, decltype(U{} * V{})> operator*(const U& operand, const base_expr<C, V>& expr);

    template <typename U> auto operator+(const base_expr<Coercion, U>& operand) const;
    template <typename U> auto operator-(const base_expr<Coercion, U>& operand) const;
    template <typename U> auto operator/(const base_expr<Coercion, U>& operand) const;
    template <typename U> auto operator*(const base_expr<Coercion, U>& operand) const;

    column_t extract_from(const dataframe_t& data) const;

  private:
    explicit base_expr(std::function<column_t(const dataframe_t& data)> f) : f_{f} {}
    explicit base_expr() = default;

    std::function<column_t(const dataframe_t& data)> f_ = nullptr;
};

template <typename Coercion> class base_expr<Coercion, void>
{
  private:
    using expr_t = base_expr<Coercion, void>;
    using dataframe_t = base_dataframe<Coercion>;
    using column_t = base_column<Coercion>;

  public:
    template <typename T> base_expr(const base_expr<Coercion, T>& source);
    explicit base_expr(const std::string& colname) : colname_{colname} {}

    base_expr(const base_expr& source);
    base_expr(base_expr&& source) = default;

    base_expr& operator=(const base_expr& source);
    base_expr& operator=(base_expr&& source) = default;

    column_t extract_from(const dataframe_t& data) const;

  private:
    struct expr_concept {
        virtual ~expr_concept() = default;
        virtual column_t extract_from(const dataframe_t& data) const = 0;
        virtual expr_concept* clone() const = 0;
    };

    template <typename T> struct expr_model : expr_concept {
        expr_model(const base_expr<Coercion, T>& expr) : expr_{expr} {}

        virtual column_t extract_from(const dataframe_t& data) const { return expr_.extract_from(data); }
        virtual expr_concept* clone() const { return new expr_model{expr_}; }

      private:
        base_expr<Coercion, T> expr_;
    };

    std::unique_ptr<expr_concept> child_;
    std::string colname_;
};

template <typename Coercion, typename T, typename U> class base_formula
{
  private:
    using dataframe_t = base_dataframe<Coercion>;
    template <typename X> using expr_t = base_expr<Coercion, X>;

  public:
    base_formula(const expr_t<T>& lhs, const expr_t<U>& rhs) : lhs_{lhs}, rhs_{rhs} {}

    const auto& lhs() const { return lhs_; }
    const auto& rhs() const { return rhs_; }

  private:
    expr_t<T> lhs_;
    expr_t<U> rhs_;
};

template <typename Coercion, typename T, typename U>
base_formula<Coercion, T, U> operator==(const base_expr<Coercion, T>& lhs, const base_expr<Coercion, U>& rhs)
{
    return {lhs, rhs};
}

template <typename T = void> using expr = base_expr<default_coercion_rules, T>;

} // namespace jules

#endif // JULES_FORMULA_FORMULA_DECL_H
