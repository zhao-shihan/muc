#pragma once

#include "muc/detail/c++17/numeric/lerp.h++"
#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <type_traits>

namespace muc {

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
///      c01      c11
///         +----+
///         |    |
/// v       +----+
/// ^    c00      c10
/// |
/// +----> u
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<T>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto bilerp(const T& c00, const T& c10, const T& c01, const T& c11,
                      U u, U v) -> T {
    return muc::lerp(muc::lerp(c00, c10, u), muc::lerp(c01, c11, u), v);
}

} // namespace muc
