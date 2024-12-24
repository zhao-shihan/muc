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

/// @brief Truncates a floating-point number to a specified number of decimal
/// places.
///
/// This function truncates the input floating-point value `x` to `n` decimal
/// places. If `x` is zero, the function returns zero directly. The truncation
/// is performed using a scaling method that accounts for the magnitude of `x`,
/// ensuring that `n` decimal places are preserved.
///
/// @tparam T The type of the input value, which must be a floating point type
///           (e.g., float, double).
/// @param x The floating-point number to be truncated.
/// @param n The number of decimal places to which `x` should be truncated.
/// @return The truncated value of `x`, with `n` decimal places.
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
auto trunc_to(T x, int n) -> T {
    if (x == 0) {
        return 0;
    }
    const auto scale{std::pow(10, n - std::ceil(std::log10(muc::abs(x))))};
    return std::trunc(x * scale) / scale;
}

/// @brief Truncates an integral value to a specified number of decimal places
/// as a floating point.
///
/// This function converts the input integral value `x` to a floating-point type
/// `T` and then truncates it to `n` decimal places. It calls the
/// `muc::trunc_to` function defined for floating-point types to perform the
/// truncation.
///
/// @tparam T The type of the output value (default is double).
///            This should be a floating-point type (e.g., float, double).
/// @tparam U The type of the input value, which must be an integral type (e.g.,
/// int, long).
/// @param x The input integral value to be truncated.
/// @param n The number of decimal places to which `x` should be truncated.
/// @return The truncated value of `x` as a floating-point number with `n`
/// decimal places.
template<typename T = double, typename U,
         std::enable_if_t<std::is_floating_point_v<T> && std::is_integral_v<U>,
                          bool> = true>
auto trunc_to(U x, int n) -> T {
    return muc::trunc_to(static_cast<T>(x), n);
}

} // namespace muc