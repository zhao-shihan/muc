#pragma once

#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <array>
#include <cmath>
#include <type_traits>

#if __cplusplus >= 202002L // >= C++20
#include <span>
#endif

namespace muc {

/// @brief Performs linear interpolation.
/// @tparam T value type, can be a scalar or vector or something.
/// @tparam U scalar type
/// @param c values on endpoints
/// @param t interpolation parameter. 0<t<1 implies interpolation, otherwise
/// extrapolation.
/// @return interpolated value.
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<U>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto lerp(const T& a, const T& b, U t) -> auto {
    return (1 - t) * a + t * b;
}

/// @brief Performs linear interpolation.
/// @tparam T value type, can be a scalar or vector or something.
/// @tparam U scalar type
/// @param c values on endpoints
/// @param t interpolation parameter. 0<t<1 implies interpolation, otherwise
/// extrapolation.
/// @return interpolated value.
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<U>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto lerp(const std::array<T, 2>& c, U t) -> auto {
    return muc::lerp(c[0], c[1], t);
}

#if __cplusplus >= 202002L // >= C++20

/// @brief Performs linear interpolation.
/// @tparam T value type, can be a scalar or vector or something.
/// @tparam U scalar type
/// @param c values on endpoints
/// @param t interpolation parameter. 0<t<1 implies interpolation, otherwise
/// extrapolation.
/// @return interpolated value.
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<U>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto lerp(std::span<T, 2> c, U t) -> auto {
    return muc::lerp(c[0], c[1], t);
}

#endif

} // namespace muc
