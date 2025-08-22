// -*- C++ -*-
//
// Copyright 2021-2025  Shihan Zhao
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "muc/detail/c++17/math/parity.h++"
#include "muc/detail/common/inline_macro.h++"

#include <type_traits>

namespace muc {

/// @brief Fast floating-point exponentiation with integer exponent
///
/// Computes x^n using exponentiation by squaring for efficient evaluation.
/// Handles both positive and negative exponents.
///
/// @tparam T Floating-point type (auto-deduced)
/// @param x Base value
/// @param n Integer exponent (positive, negative, or zero)
/// @return x raised to the power of n
///
/// @note Behavior:
///   - Uses exponentiation by squaring (O(log n) complexity)
///   - Negative exponents: returns 1/pow(x, -n)
///   - Zero exponent: returns 1 (for any x, including 0 and NaN)
///
/// @warning Precision may differ from std::pow near representability limits
/// @see std::pow() for standards-compliant implementation
/// @see ipow() for integer base version
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
MUC_STRONG_INLINE constexpr auto pow(T x, int n) -> T {
    if (n < 0) {
        return 1 / muc::pow(x, -n);
    }
    T z{1};
    while (n > 0) {
        if (muc::odd(n)) {
            z *= x;
        }
        x *= x;
        n /= 2;
    }
    return z;
}

/// @brief Floating-point exponentiation with integral base conversion
///
/// Computes x^n by first converting integral base to floating-point.
/// More efficient than std::pow for integer bases with integer exponents.
///
/// @tparam T Floating-point result type (default: double)
/// @tparam U Integral base type (auto-deduced)
/// @param x Integral base value
/// @param n Integer exponent (positive, negative, or zero)
/// @return x converted to T and raised to the power of n
///
/// @see ipow() for pure integer version (may overflow)
/// @see pow() for floating-point base version
template<typename T = double, typename U,
         std::enable_if_t<std::is_floating_point_v<T> and std::is_integral_v<U>,
                          bool> = true>
MUC_STRONG_INLINE constexpr auto pow(U x, int n) -> T {
    return muc::pow(static_cast<T>(x), n);
}

/// @brief Integer exponentiation
///
/// Computes m^n using exponentiation by squaring with integral arithmetic.
///
/// @tparam T Integral type of base and result (auto-deduced)
/// @param m Base value
/// @param n Integer exponent
/// @return m raised to the power of n
///
/// @note Special cases for negative exponents:
///   - If m = 1: returns 1 (1^n = 1 for any n)
///   - If m = -1: returns 1 for even exponents, -1 for odd exponents
///   - Otherwise: returns 0
///
/// @warning May overflow for large values of m and n
/// @see pow() for floating-point version
template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
MUC_STRONG_INLINE constexpr auto ipow(T m, int n) -> T {
    if (n < 0) {
        if constexpr (std::is_signed_v<T>) {
            if (m == 1) {
                return 1;
            } else if (m == -1) {
                return muc::even(n) ? 1 : -1;
            }
        } else {
            if (m == 1) {
                return 1;
            }
        }
        return 0;
    }
    T k{1};
    while (n > 0) {
        if (muc::odd(n)) {
            k *= m;
        }
        m *= m;
        n /= 2;
    }
    return k;
}

} // namespace muc
