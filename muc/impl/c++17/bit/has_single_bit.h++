#pragma once

#include "muc/impl/c++17/bit/impl/is_really_unsigned_integral.h++"

#include <type_traits>

namespace muc {

/// @brief Checks if x is an integral power of two. This participates in
/// overload resolution only if T is an unsigned integer type (that is, unsigned
/// char, unsigned short, unsigned int, unsigned long, unsigned long long, or an
/// extended unsigned integer type).
/// @tparam T unsigned integer type
/// @param x value of unsigned integer type
/// @return true if x is an integral power of two; otherwise false.
template<typename T,
         std::enable_if_t<impl::is_really_unsigned_integral_v<T>, bool> = true>
constexpr auto has_single_bit(T x) noexcept -> bool {
    return x && !(x & (x - 1u));
}

} // namespace muc
