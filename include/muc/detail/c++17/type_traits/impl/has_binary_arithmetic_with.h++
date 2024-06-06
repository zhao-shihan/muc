#pragma once

#include <type_traits>
#include <utility>

namespace muc::impl {

template<typename, typename, typename = void>
struct has_binary_arithmetic_with : std::false_type {};

template<typename T, typename U>
struct has_binary_arithmetic_with<
    T, U,
    std::void_t<
        // T +/- T
        decltype(std::declval<const T&>() + std::declval<const T&>()),
        decltype(std::declval<const T&>() - std::declval<const T&>()),
        // k * T
        decltype(+std::declval<const T&>()),
        decltype(-std::declval<const T&>()),
        /* decltype(1 * std::declval<const T&>()),
        decltype(std::declval<const T&>() * 1),
        decltype(std::declval<const T&>() / 1), */
        // U +/- U
        decltype(std::declval<const U&>() + std::declval<const U&>()),
        decltype(std::declval<const U&>() - std::declval<const U&>()),
        // k * U
        decltype(+std::declval<const U&>()),
        decltype(-std::declval<const U&>()),
        /* decltype(1 * std::declval<const U&>()),
        decltype(std::declval<const U&>() * 1),
        decltype(std::declval<const U&>() / 1), */
        // T +/- U
        decltype(std::declval<const T&>() + std::declval<const U&>()),
        decltype(std::declval<const T&>() - std::declval<const U&>()),
        // U +/- T
        decltype(std::declval<const U&>() + std::declval<const T&>()),
        decltype(std::declval<const U&>() - std::declval<const T&>()),
        // T x= X
        decltype(std::declval<T&>() += std::declval<const U&>()),
        decltype(std::declval<T&>() -= std::declval<const U&>())//,
        /* decltype(std::declval<T&>() *= 1), decltype(std::declval<T&>() /= 1) */>> :
    std::true_type {};

template<typename T, typename U>
inline constexpr bool has_binary_arithmetic_with_v{
    has_binary_arithmetic_with<T, U>::value};

} // namespace muc::impl
