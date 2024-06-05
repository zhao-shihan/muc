#pragma once

#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <type_traits>

namespace muc {

/// @brief Performs linear interpolation.
/// @tparam T value type, can be a scalar or vector or something.
/// @tparam U scalar type
/// @param c values on endpoints
/// @param t interpolation parameter. 0<t<1 implies interpolation, otherwise
/// extrapolation.
/// @return interpolated value.
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<T>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto lerp(const T& a, const T& b, U t) -> T {
    if constexpr (std::is_integral_v<T>) {
        return (1 - t) * a + t * b;
    } else {
        return a + t * (b - a);
    }
}

} // namespace muc
