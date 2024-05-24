#pragma once

#include "muc/detail/c++17/type_traits/impl/has_binary_arithmetic_with.h++"

#include <type_traits>
#include <utility>

namespace muc {

template<typename T, typename U>
struct is_arithmetic_operable_with :
    std::bool_constant<
        is_general_arithmetic_v<T> and is_general_arithmetic_v<U> and
        // T <-> {U}
        impl::has_binary_arithmetic_with_v<T, U> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const U&>() + std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const U&>() - std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(+std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(-std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T, decltype(1 * std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T, decltype(std::declval<const U&>() * 1)> and
        impl::has_binary_arithmetic_with_v<
            T, decltype(std::declval<const U&>() / 1)> and
        // T <-> {T <-> U}
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const T&>() + std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const T&>() - std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const U&>() + std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const U&>() - std::declval<const T&>())> and
        // U <-> {T}
        impl::has_binary_arithmetic_with_v<U, T> and
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const T&>() + std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const T&>() - std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(+std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(-std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(1 * std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(std::declval<const T&>() * 1)> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(std::declval<const T&>() / 1)> and
        // U <-> {T <-> U}
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const T&>() + std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const T&>() - std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const U&>() + std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(std::declval<const U&>() - std::declval<const T&>())>> {
};

template<typename T, typename U>
inline constexpr bool is_arithmetic_operable_with_v{
    is_arithmetic_operable_with<T, U>::value};

} // namespace muc
