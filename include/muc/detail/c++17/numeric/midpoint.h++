#pragma once

#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <type_traits>

namespace muc {

template<typename T, std::enable_if_t<is_general_arithmetic_v<T>, bool> = true>
constexpr auto midpoint(const T& a, const T& b) -> T {
    return a + (b - a) / 2;
}

} // namespace muc
