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

/// @brief Computes the sigmoid of a given value.
///
/// The sigmoid function is defined as:
/// \f[ \sigma(x) = \frac{1}{1 + e^{-x}} \f]
/// It maps any real-valued number into the range (0, 1). This is commonly used
/// in neural networks and logistic regression.
///
/// @tparam T The type of the input value, which must be a floating point type
/// (e.g., float, double).
/// @param x The input value for which the sigmoid function is to be computed.
///          It should be a floating point number.
/// @return The computed sigmoid value of the input `x`, which will be in the
/// range (0, 1).
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
auto sigmoid(T x) -> T {
    return 1 / (1 + std::exp(-x));
}

/// @brief Computes the sigmoid of a given integral value and returns it as a
/// floating-point type.
///
/// This function takes an integral input value, converts it to a floating-point
/// type, and then computes the sigmoid using the `sigmoid` function template
/// for floating point types. The resulting value is within the range (0, 1), as
/// with the typical sigmoid function.
///
/// @tparam T The type of the output value (default is double).
///            This should be a floating point type (e.g., float, double).
/// @tparam U The type of the input value, which must be an integral type (e.g.,
/// int, long).
/// @param x The input value for which the sigmoid function is to be computed.
///          It should be an integral number that will be converted to type `T`.
/// @return The computed sigmoid value of the input `x`, converted to type `T`,
/// which will be in the range (0, 1).
template<typename T = double, typename U,
         std::enable_if_t<std::is_floating_point_v<T> and std::is_integral_v<U>,
                          bool> = true>
auto sigmoid(U x) -> T {
    return muc::sigmoid(static_cast<T>(x));
}

} // namespace muc
