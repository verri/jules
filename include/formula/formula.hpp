#ifndef JULES_FORMULA_FORMULA_H
#define JULES_FORMULA_FORMULA_H

#include "dataframe/dataframe.hpp"
#include "util/algorithm.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace jules
{
namespace detail
{
template <typename Coercion> class term_eraser
{
  public:
    using dataframe_t = base_dataframe<Coercion>;
    using column_t = base_column<Coercion>;

    explicit term_eraser(const std::string& name) : name_{name} {}

    column_t extract_from(const dataframe_t& data)
    {
        auto col = data.col(name_);
        return coerce_and_apply_modifiers(col);
    }

    virtual column_t& coerce_and_apply_modifiers(column_t& col) = 0;
    virtual std::unique_ptr<term_eraser> clone() = 0;

    auto& name() { return name; }
    const auto& name() const { return name; }

  protected:
    std::string name_;
};

template <typename Coercion> struct term_erased_list {
    template <typename... Args> term_erased_list(Args&&... args)
    {
        for_each_arg([this](auto&& x) { terms.push_back(&x); }, args...);
    }

    auto begin() const { return terms.begin(); }
    auto end() const { return terms.end(); }
    std::vector<term_eraser<Coercion>*> terms;
};

} // namespace detail

template <typename Coercion, typename T = void> class base_term : public detail::term_eraser<Coercion>
{
  private:
    using column_t = typename detail::term_eraser<Coercion>::column_t;

  public:
    explicit base_term(const std::string& name) : detail::term_eraser<Coercion>{name} {}

    base_term(const base_term& source) = default;
    base_term(base_term&& source) = default;

    base_term& operator=(const base_term& source) = delete;
    base_term& operator=(base_term&& source) = delete;

    virtual column_t& coerce_and_apply_modifiers(column_t& col) override final
    {
        col.template coerce_to<T>();
        if (f_)
            col.template as_array<T>().apply(f_);

        return col;
    }

    virtual std::unique_ptr<detail::term_eraser<Coercion>> clone() override final
    {
        return std::unique_ptr<detail::term_eraser<Coercion>>{new base_term{*this}};
    }

    // TODO automatically generate other operators
    template <typename U> base_term friend operator*(const U& operand, const base_term& other)
    {
        return base_term{other, [operand](const T& value) { return value * operand; }};
    }

  private:
    explicit base_term(const base_term& source, const std::function<T(const T&)>& g)
        : detail::term_eraser<Coercion>{source.name_}
    {
        if (source.f_)
            f_ = [ g = g, h = source.f_ ](const T& value) { return g(h(value)); };
        else
            f_ = [g = g](const T& value) { return g(value); };
    }

    std::function<T(const T&)> f_;
};

template <typename Coercion> class base_term<Coercion, void> : public detail::term_eraser<Coercion>
{
  private:
    using column_t = typename detail::term_eraser<Coercion>::column_t;

  public:
    explicit base_term(const std::string& name) : detail::term_eraser<Coercion>{name} {}

    base_term(const base_term& source) = default;
    base_term(base_term&& source) = default;

    base_term& operator=(const base_term& source) = delete;
    base_term& operator=(base_term&& source) = delete;

    virtual column_t& coerce_and_apply_modifiers(column_t& col) override final { return col; }

    virtual std::unique_ptr<detail::term_eraser<Coercion>> clone() override final
    {
        return std::unique_ptr<detail::term_eraser<Coercion>>{new base_term{*this}};
    }
};

template <typename Coercion> class base_formula
{
  private:
    using dataframe_t = base_dataframe<Coercion>;
    using column_t = base_column<Coercion>;

  public:
    base_formula(std::unique_ptr<detail::term_eraser<Coercion>>&& response,
                 std::vector<std::unique_ptr<detail::term_eraser<Coercion>>>&& terms)
        : response_{std::move(response)}, terms_{std::move(terms)}
    {
    }

    column_t response(const dataframe_t& data) { return response_->extract_from(data); }

  private:
    std::unique_ptr<detail::term_eraser<Coercion>> response_;
    std::vector<std::unique_ptr<detail::term_eraser<Coercion>>> terms_;
};

template <typename Coercion, typename T> class base_response : public base_term<Coercion, T>
{
  private:
    using base_term = base_term<Coercion, T>;
    using term_eraser = detail::term_eraser<Coercion>;
    using term_ptr = std::unique_ptr<term_eraser>;

  public:
    using base_term::base_term;

    base_response(const base_response& source) = delete;
    base_response(base_response&& source) = delete;
    base_response& operator=(const base_response& source) = delete;
    base_response& operator=(base_response&& source) = delete;

    base_formula<Coercion> operator=(const detail::term_erased_list<Coercion>& terms)
    {
        std::vector<term_ptr> terms_vector;
        for (auto&& term : terms)
            terms_vector.push_back(term->clone());

        return base_formula<Coercion>{std::move(term_ptr{new base_term{*dynamic_cast<base_term*>(this)}}),
                                      std::move(terms_vector)};
    }
};

template <typename T = void> using term = base_term<default_coercion_rules, T>;
template <typename T = void> using response = base_response<default_coercion_rules, T>;
using formula = base_formula<default_coercion_rules>;

} // namespace jules

#endif // JULES_FORMULA_FORMULA_H
