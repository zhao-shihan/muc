#pragma once

#include "muc/detail/c++17/numeric/bilerp.h++"
#include "muc/detail/c++17/numeric/lerp.h++"
#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <type_traits>

namespace muc {

/// @brief Performs trilinear interpolation.
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
///         c011    c111
///            +----+
///      c010 /|   /|
///          +----+ + c101
///          |c001|/
/// v w      +----+
/// ^ ^  c000     c100
/// |/
/// +----> u
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<T>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto trilerp(const T& c000, const T& c100, const T& c010,
                       const T& c110, const T& c001, const T& c101,
                       const T& c011, const T& c111, U u, U v, U w) -> T {
    return muc::bilerp(muc::lerp(c000, c100, u), muc::lerp(c010, c110, u),
                       muc::lerp(c001, c101, u), muc::lerp(c011, c111, u), v,
                       w);
}

} // namespace muc
