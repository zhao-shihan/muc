#pragma once

#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <array>
#include <cmath>
#include <type_traits>

#if __cplusplus >= 202002L // >= C++20
#include <span>
#endif

namespace muc {

#define MUC_TRILERP_IMPL                                                     \
    const T a000{c[0]};                                                      \
    const T a100{c[4] - a000};                                               \
    const T a010{c[1] - a000};                                               \
    const T a001{c[2] - a000};                                               \
    const T b101{c[6] - c[2]};                                               \
    const T a110{c[5] - c[4] - a010};                                        \
    const T a101{b101 - a100};                                               \
    const T a011{c[6] - c[4] - a001};                                        \
    const T a111{c[7] - c[3] - b101 - a110};                                 \
    return a000 + (a100 + a101 * w + (a110 + a111 * w) * v) * u + a001 * w + \
           (a010 + a011 * w) * v;

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
///         c[3]    c[7]
///            +----+
///      c[1] /|   /|
///          +----+ + c[6]
///          |c[2]|/
/// v w      +----+
/// ^ ^  c[0]     c[4]
/// |/
/// +----> u
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<U>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto trilerp(const std::array<T, 8>& c, U u, U v, U w) -> T {
    MUC_TRILERP_IMPL
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
///         c[3]    c[7]
///            +----+
///      c[1] /|   /|
///          +----+ + c[6]
///          |c[2]|/
/// v w      +----+
/// ^ ^  c[0]     c[4]
/// |/
/// +----> u
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<U>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto trilerp(std::span<T, 8> c, U u, U v, U w) -> T {
    MUC_TRILERP_IMPL
}

#endif

} // namespace muc
