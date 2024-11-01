// -*- C++ -*-
//
// Copyright 2024  Shihan Zhao
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

#include "muc/detail/c++17/math/constexpr_cmath.h++"

#include <cmath>
#include <type_traits>

namespace muc {

/// @brief Rounds a floating-point number to a specified number of decimal
/// places.
///
/// This function rounds the input floating-point value `x` to `n` decimal
/// places. If `x` is zero, the function returns zero directly. The rounding is
/// performed using a scaling method which accounts for the magnitude of `x`,
/// ensuring correctness across a wide range of values.
///
/// @tparam T The type of the input value, which must be a floating point type
///           (e.g., float, double).
/// @param x The floating-point number to be rounded.
/// @param n The number of decimal places to which `x` should be rounded.
/// @return The rounded value of `x`, with `n` decimal places.
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
auto round_to(T x, int n) -> T {
    if (x == 0) {
        return 0;
    }
    const auto scale{std::pow(10, n - std::ceil(std::log10(muc::abs(x))))};
    return std::round(x * scale) / scale;
}

/// @brief Rounds an integral value to a specified number of decimal places as a
/// floating-point type.
///
/// This function converts the input integral value `x` to a floating-point type
/// `T` and rounds it to `n` decimal places. It uses the `muc::round_to`
/// function defined for floating-point types to perform the rounding operation.
///
/// @tparam T The type of the output value (default is double).
///            This should be a floating-point type (e.g., float, double).
/// @tparam U The type of the input value, which must be an integral type (e.g.,
/// int, long).
/// @param x The input integral value to be rounded.
/// @param n The number of decimal places to which `x` should be rounded.
/// @return The rounded value of `x` as a floating-point number with `n` decimal
/// places.
template<typename T = double, typename U,
         std::enable_if_t<std::is_floating_point_v<T> && std::is_integral_v<U>,
                          bool> = true>
auto round_to(U x, int n) -> T {
    return muc::round_to(static_cast<T>(x), n);
}

} // namespace muc
