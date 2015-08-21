#ifndef JULES_FORMULA_FORMULA_H
#define JULES_FORMULA_FORMULA_H

#include "formula/formula_decl.hpp"

#include <stdexcept>

namespace jules
{

template <typename Coercion, typename T>
base_term<Coercion, T>::base_term(const std::string& colname) {
    f_ = [colname](const dataframe_t& data) {
        auto col = data.col(colname);
        if (col.elements_type() != typeid(T))
            col.template coerce_to<T>;
        return col;
    };
}

template <typename Coercion, typename T>
auto base_term<Coercion, T>::clone() const -> std::unique_ptr<term_eraser_t> {
    return std::unique_ptr<term_eraser_t>{new base_term{*this}};
}

template <typename Coercion, typename T>
auto base_term<Coercion, T>::coerce_and_apply_modifiers(const dataframe_t& data) const -> column_t& {
    return f_(data);
}

template <typename Coercion, typename T>
template <typename U> auto base_term<Coercion, T>::operator+(const U& operand) const -> base_term {
    return base_term{[parent = *this, operand](const dataframe_t& data) {
        auto col = parent.extract_from(data);

        auto view = make_view<T>(col);
        for (auto& v : view)
            v = v + operand;

        return col;
    }};
}

template <typename Coercion, typename T>
template <typename U> auto base_term<Coercion, T>::operator+(const base_term<Coercion, U>& operand) const -> base_term {
    return base_term{[parent1 = *this, parent2 = operand](const dataframe_t& data) {
        auto lhs = parent1.extract_from(data);
        auto rhs = parent2.extract_from(data);

        std::size_t size;
        if ((size = lhs.size()) != rhs.size())
            throw std::runtime_error{"columns' size differ"};

        auto lhs_view = make_view<T>(lhs);
        auto rhs_view = make_view<U>(rhs);

        auto value = T{} + U{};
        auto result = column_t(lhs.name() + "+" + rhs.name(), {value});
        result.resize(size);

        auto result_view = make_view<decltype(value)>(result);

        for (std::size_t i = 0; i < size; ++i)
            result_view[i] = lhs_view[i] + rhs_view[i];

        return result;
    }};
}

} // namespace jules

#endif // JULES_FORMULA_FORMULA_H
