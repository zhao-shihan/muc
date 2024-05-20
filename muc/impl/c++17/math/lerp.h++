#pragma once

#include "muc/impl/c++17/type_traits/is_general_arithmetic.h++"

#include <cmath>
#include <concepts>
#include <type_traits>

namespace muc {

template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<U>, bool> = true,
         std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
constexpr auto lerp(const T& a, const T& b, U t) -> decltype(auto) {
    return (1 - t) * a + t * b;
}

} // namespace muc
