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

#include <cmath>
#include <type_traits>

namespace muc {

/// @brief Computes the sum of squares for floating-point values
///
/// Calculates the square of the Euclidean norm (Σx²) for 2 or more
/// floating-point values. Compile-time enabled only for floating-point types
/// with 2+ arguments.
///
/// @tparam Ts Floating-point types (at least 2 required)
/// @param x Floating-point values to square and sum
///
/// @return Sum of squares: x₁² + x₂² + ... + xₙ²
///
/// @note Compile-time constraints:
///       - All types must be floating-point (float/double/etc)
///       - Minimum 2 arguments required
/// @see muc::hypot() for the Euclidean norm
template<typename... Ts, std::enable_if_t<((sizeof...(Ts) >= 2) and ... and
                                           std::is_floating_point_v<Ts>),
                                          bool> = true>
constexpr auto hypot_sq(Ts... x) -> auto {
    return (... + (x * x));
}

/// @brief Computes the Euclidean norm (hypotenuse) for floating-point values
///
/// Calculates √(Σx²) for 2 or more floating-point values. The multi-dimensional
/// generalization of the Pythagorean theorem.
///
/// @tparam Ts Floating-point types (at least 2 required)
/// @param x Floating-point values to include in norm calculation
///
/// @return Euclidean norm: √(x₁² + x₂² + ... + xₙ²)
///
/// @note Compile-time constraints:
///       - All types must be floating-point (float/double/etc)
///       - Minimum 2 arguments required
/// @note Delegates to muc::hypot_sq() for sum of squares
template<typename... Ts, std::enable_if_t<((sizeof...(Ts) >= 2) and ... and
                                           std::is_floating_point_v<Ts>),
                                          bool> = true>
auto hypot(Ts... x) -> auto {
    return std::sqrt(muc::hypot_sq(x...));
}

} // namespace muc
