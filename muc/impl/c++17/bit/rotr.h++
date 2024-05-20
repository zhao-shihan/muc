#pragma once

#include "muc/impl/c++17/bit/impl/is_really_unsigned_integral.h++"

#include <limits>
#include <type_traits>

namespace muc {

/// @brief Computes the result of bitwise right-rotating the value of x by s
/// positions. This overload participates in overload resolution only if T is an
/// unsigned integer type (that is, unsigned char, unsigned short, unsigned int,
/// unsigned long, unsigned long long, or an extended unsigned integer type).
/// @tparam T unsigned integer type
/// @param x value of unsigned integer type
/// @param s number of positions to shift
/// @return The result of bitwise right-rotating x by s positions.
template<typename T,
         std::enable_if_t<impl::is_really_unsigned_integral_v<T>, bool> = true>
[[nodiscard]] constexpr auto rotr(T x, int s) noexcept -> T {
    constexpr auto n{std::numeric_limits<T>::digits};
    const auto r{s % n};
    if (r > 0) { return (x >> r) | (x << (n - r)); }
    if (r < 0) { return (x << -r) | (x >> (n + r)); }
    return x; // r == 0
}

} // namespace muc
