#pragma once

#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <array>
#include <cmath>
#include <type_traits>

#if __cplusplus >= 202002L // >= C++20
#include <span>
#endif

namespace muc {

#define MUC_BILERP_IMPL             \
    const T a00{c[0]};              \
    const T a10{c[2] - a00};        \
    const T a01{c[1] - a00};        \
    const T a11{c[3] - c[1] - a10}; \
    return a00 + a10 * u + (a01 + a11 * u) * v;

/// @brief Performs bilinear interpolation.
/// @tparam T value type, can be a scalar or vector or something.
/// @tparam U scalar type
/// @param c values on square grid. See note for details.
/// @param u interpolation parameter. 0<u<1 implies interpolation, otherwise
/// extrapolation.
/// @param v interpolation parameter. 0<v<1 implies interpolation, otherwise
/// extrapolation.
/// @return interpolated value.
/// @note For parameter c:
///
///      c[1]      c[3]
///          +----+
///          |    |
/// v        +----+
/// ^    c[0]      c[2]
/// |
/// +----> u
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<U>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto bilerp(const std::array<T, 4>& c, U u, U v) -> T {
    MUC_BILERP_IMPL
}

#if __cplusplus >= 202002L // >= C++20

/// @brief Performs bilinear interpolation.
/// @tparam T value type, can be a scalar or vector or something.
/// @tparam U scalar type
/// @param c values on square grid. See note for details.
/// @param u interpolation parameter. 0<u<1 implies interpolation, otherwise
/// extrapolation.
/// @param v interpolation parameter. 0<v<1 implies interpolation, otherwise
/// extrapolation.
/// @return interpolated value.
/// @note For parameter c:
///
///      c[1]      c[3]
///          +----+
///          |    |
/// v        +----+
/// ^    c[0]      c[2]
/// |
/// +----> u
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<U>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto bilerp(std::span<T, 4> c, U u, U v) -> T {
    MUC_BILERP_IMPL
}

#endif

} // namespace muc
