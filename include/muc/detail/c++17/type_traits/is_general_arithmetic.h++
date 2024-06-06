#pragma once

#include "muc/detail/c++17/type_traits/impl/has_binary_arithmetic_with.h++"

#include <type_traits>
#include <utility>

namespace muc {

template<typename T>
struct is_general_arithmetic :
    std::bool_constant<std::is_default_constructible_v<T> and
                       impl::has_binary_arithmetic_with_v<T, T> and
                       impl::has_binary_arithmetic_with_v<
                           T, decltype(std::declval<const T&>() +
                                       std::declval<const T&>())> and
                       impl::has_binary_arithmetic_with_v<
                           T, decltype(std::declval<const T&>() -
                                       std::declval<const T&>())> /* and
                       impl::has_binary_arithmetic_with_v<
                           T, decltype(1 * std::declval<const T&>())> and
                       impl::has_binary_arithmetic_with_v<
                           T, decltype(std::declval<const T&>() * 1)> and
                       impl::has_binary_arithmetic_with_v<
                           T, decltype(std::declval<const T&>() / 1)> */> {};

template<typename T>
inline constexpr bool is_general_arithmetic_v{is_general_arithmetic<T>::value};

} // namespace muc
