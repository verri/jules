#ifndef JULES_ARRAY_DETAIL_DEFINE_MACROS_H
#define JULES_ARRAY_DETAIL_DEFINE_MACROS_H

#include <jules/array/detail/common.hpp>

#define BASE_NDARRAY(X__, N__) const base_ndarray<X__, N__>&
#define REF_NDARRAY(X__, N__) const ref_ndarray<X__, N__>&
#define INDIRECT_NDARRAY(X__, N__) const indirect_ndarray<X__, N__>&
#define BINARY_EXPR_NDARRAY(X__, Y__, Z__, N__) const binary_expr_ndarray<X__, Y__, Z__, N__>&
#define UNARY_EXPR_NDARRAY(X__, Y__, N__) const unary_expr_ndarray<X__, Y__, N__>&

#define UNPACK(...) __VA_ARGS__

#define UNARY_APPLY_OPERATION(TX__, X__, N__)                                                                                    \
    template <typename Operator> auto apply(const Operator& op) const                                                            \
    {                                                                                                                            \
        return make_expr_ndarray(this->data_begin(), this->data_end(), op, this->extents());                                     \
    }

#define UNARY_OPERATION(TX__, X__, N__, OP__)                                                                                    \
    auto operator OP__() const                                                                                                   \
    {                                                                                                                            \
        return this->apply([](const T& a) { return OP__ a; });                                                                   \
    }

#define UNARY_OPERATIONS_LIST(TX__, X__, N__)                                                                                    \
    UNARY_APPLY_OPERATION(TX__, X__, N__)                                                                                        \
    UNARY_OPERATION(TX__, X__, N__, +)                                                                                           \
    UNARY_OPERATION(TX__, X__, N__, -)                                                                                           \
    UNARY_OPERATION(TX__, X__, N__, ~)                                                                                           \
    UNARY_OPERATION(TX__, X__, N__, !)

#define BINARY_APPLY_OPERATION(TX__, X__, TY__, Y__)                                                                             \
    template <UNPACK TY__, typename Operator> auto apply(const Operator& op, UNPACK Y__ rhs) const                               \
    {                                                                                                                            \
        const auto& lhs = *this;                                                                                                 \
        return make_expr_ndarray(lhs.data_begin(), lhs.data_end(), rhs.data_begin(), rhs.data_end(), op, lhs.extents());         \
    }

#define BINARY_OPERATION(TX__, X__, TY__, Y__, OP__)                                                                             \
    template <UNPACK TY__> auto operator OP__(UNPACK Y__ rhs) const                                                              \
    {                                                                                                                            \
        using Value = typename std::decay_t<UNPACK Y__>::value_type;                                                             \
        return this->apply([](const T& a, const Value& b) { return a OP__ b; }, rhs);                                            \
    }

#define BINARY_LEFT_TYPE_OPERATION(TY__, Y__, OP__)                                                                              \
    template <UNPACK TY__, typename U, typename, typename> friend auto operator OP__(const U& lhs, UNPACK Y__ rhs);              \
                                                                                                                                 \
    template <UNPACK TY__, typename U, typename, typename>                                                                       \
    friend auto operator OP__(const base_ndarray<U, 0>& lhs, UNPACK Y__ rhs);

#define BINARY_RIGHT_TYPE_OPERATION(TX__, X__, OP__)                                                                             \
    template <typename U, typename = std::enable_if_t<!is_array<U>()>,                                                           \
              typename = decltype(std::declval<T>() OP__ std::declval<U>())>                                                     \
    auto operator OP__(const U& rhs) const                                                                                       \
    {                                                                                                                            \
        return this->apply([&rhs](const T& a) { return a OP__ rhs; });                                                           \
    }                                                                                                                            \
                                                                                                                                 \
    template <typename U, typename = decltype(std::declval<T>() OP__ std::declval<U>())>                                         \
    auto operator OP__(const base_ndarray<U, 0>& rhs) const                                                                      \
    {                                                                                                                            \
        return this->apply([&rhs](const T& a) { return a OP__ static_cast<const U&>(rhs); });                                    \
    }

#define BINARY_TYPE_OPERATION(TX__, X__, OP__)                                                                                   \
    BINARY_RIGHT_TYPE_OPERATION(TX__, X__, OP__)                                                                                 \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, OP__)

#define BINARY_COMBINATIONS(TX__, X__, N__, OP__)                                                                                \
    BINARY_OPERATION(TX__, X__, (typename U), (BASE_NDARRAY(U, N__)), OP__)                                                      \
    BINARY_OPERATION(TX__, X__, (typename U), (REF_NDARRAY(U, N__)), OP__)                                                       \
    BINARY_OPERATION(TX__, X__, (typename U), (INDIRECT_NDARRAY(U, N__)), OP__)                                                  \
    BINARY_OPERATION(TX__, X__, (typename A, typename B), (UNARY_EXPR_NDARRAY(A, B, N__)), OP__)                                 \
    BINARY_OPERATION(TX__, X__, (typename A, typename B, typename C), (BINARY_EXPR_NDARRAY(A, B, C, N__)), OP__)                 \
    BINARY_TYPE_OPERATION(TX__, X__, OP__)

#define BINARY_APPLY_COMBINATIONS(TX__, X__, N__)                                                                                \
    BINARY_APPLY_OPERATION(TX__, X__, (typename U), (BASE_NDARRAY(U, N__)))                                                      \
    BINARY_APPLY_OPERATION(TX__, X__, (typename U), (REF_NDARRAY(U, N__)))                                                       \
    BINARY_APPLY_OPERATION(TX__, X__, (typename U), (INDIRECT_NDARRAY(U, N__)))                                                  \
    BINARY_APPLY_OPERATION(TX__, X__, (typename A, typename B), (UNARY_EXPR_NDARRAY(A, B, N__)))                                 \
    BINARY_APPLY_OPERATION(TX__, X__, (typename A, typename B, typename C), (BINARY_EXPR_NDARRAY(A, B, C, N__)))

#define BINARY_OPERATIONS_LIST(TX__, X__, N__)                                                                                   \
    BINARY_APPLY_COMBINATIONS(TX__, X__, N__)                                                                                    \
    BINARY_COMBINATIONS(TX__, X__, N__, +)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, -)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, *)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, /)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, %)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, &)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, |)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, ^)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, <<)                                                                                      \
    BINARY_COMBINATIONS(TX__, X__, N__, >>)                                                                                      \
    BINARY_COMBINATIONS(TX__, X__, N__, &&)                                                                                      \
    BINARY_COMBINATIONS(TX__, X__, N__, ||)                                                                                      \
    BINARY_COMBINATIONS(TX__, X__, N__, ==)                                                                                      \
    BINARY_COMBINATIONS(TX__, X__, N__, !=)                                                                                      \
    BINARY_COMBINATIONS(TX__, X__, N__, <)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, <=)                                                                                      \
    BINARY_COMBINATIONS(TX__, X__, N__, >)                                                                                       \
    BINARY_COMBINATIONS(TX__, X__, N__, >=)

#define OPERATIONS_LIST(TX__, X__, N__)                                                                                          \
    UNARY_OPERATIONS_LIST(TX__, X__, N__)                                                                                        \
    BINARY_OPERATIONS_LIST(TX__, X__, N__)

#define BINARY_LEFT_TYPE_OPERATION_DECLARATION(TY__, Y__, OP__)                                                                  \
    template <UNPACK TY__, typename U, typename = std::enable_if_t<!is_array<U>() && (M > 0)>,                                   \
              typename = decltype(std::declval<U>() OP__ std::declval<typename std::decay_t<UNPACK Y__>::value_type>())>         \
    auto operator OP__(const U& lhs, UNPACK Y__ rhs);                                                                            \
                                                                                                                                 \
    template <UNPACK TY__, typename U, typename = std::enable_if_t<(M > 0)>,                                                     \
              typename = decltype(std::declval<U>() OP__ std::declval<typename std::decay_t<UNPACK Y__>::value_type>())>         \
    auto operator OP__(const base_ndarray<U, 0>& lhs, UNPACK Y__ rhs);

#define BINARY_LEFT_TYPE_OPERATION_DEFINITION(TY__, Y__, OP__)                                                                   \
    template <UNPACK TY__, typename U, typename, typename> auto operator OP__(const U& lhs, UNPACK Y__ rhs)                      \
    {                                                                                                                            \
        using Value = typename std::decay_t<UNPACK Y__>::value_type;                                                             \
        return rhs.apply([&lhs](const Value& b) { return static_cast<const U&>(lhs) OP__ b; });                                  \
    }                                                                                                                            \
                                                                                                                                 \
    template <UNPACK TY__, typename U, typename, typename> auto operator OP__(const base_ndarray<U, 0>& lhs, UNPACK Y__ rhs)     \
    {                                                                                                                            \
        using Value = typename std::decay_t<UNPACK Y__>::value_type;                                                             \
        return rhs.apply([&lhs](const Value& b) { return lhs OP__ b; });                                                         \
    }

#define FRIEND_OPERATIONS_DECLARATION(TX__, X__)                                                                                 \
    BINARY_LEFT_TYPE_OPERATION_DECLARATION(TX__, X__, &&)                                                                        \
    BINARY_LEFT_TYPE_OPERATION_DECLARATION(TX__, X__, ||)                                                                        \
    BINARY_LEFT_TYPE_OPERATION_DECLARATION(TX__, X__, ==)                                                                        \
    BINARY_LEFT_TYPE_OPERATION_DECLARATION(TX__, X__, !=)                                                                        \
    BINARY_LEFT_TYPE_OPERATION_DECLARATION(TX__, X__, <)                                                                         \
    BINARY_LEFT_TYPE_OPERATION_DECLARATION(TX__, X__, <=)                                                                        \
    BINARY_LEFT_TYPE_OPERATION_DECLARATION(TX__, X__, >)                                                                         \
    BINARY_LEFT_TYPE_OPERATION_DECLARATION(TX__, X__, >=)

#define FRIEND_OPERATIONS_DEFINITION(TX__, X__)                                                                                  \
    BINARY_LEFT_TYPE_OPERATION_DEFINITION(TX__, X__, &&)                                                                         \
    BINARY_LEFT_TYPE_OPERATION_DEFINITION(TX__, X__, ||)                                                                         \
    BINARY_LEFT_TYPE_OPERATION_DEFINITION(TX__, X__, ==)                                                                         \
    BINARY_LEFT_TYPE_OPERATION_DEFINITION(TX__, X__, !=)                                                                         \
    BINARY_LEFT_TYPE_OPERATION_DEFINITION(TX__, X__, <)                                                                          \
    BINARY_LEFT_TYPE_OPERATION_DEFINITION(TX__, X__, <=)                                                                         \
    BINARY_LEFT_TYPE_OPERATION_DEFINITION(TX__, X__, >)                                                                          \
    BINARY_LEFT_TYPE_OPERATION_DEFINITION(TX__, X__, >=)

#define FRIEND_OPERATIONS(TX__, X__)                                                                                             \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, &&)                                                                                    \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, ||)                                                                                    \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, ==)                                                                                    \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, !=)                                                                                    \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, <)                                                                                     \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, <=)                                                                                    \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, >)                                                                                     \
    BINARY_LEFT_TYPE_OPERATION(TX__, X__, >=)

#endif // JULES_ARRAY_DETAIL_DEFINE_MACROS_H
