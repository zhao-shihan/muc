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

#include <algorithm>
#include <limits>
#include <type_traits>

namespace muc {

/// @brief Computes the ReLU (Rectified Linear Unit) of a given floating-point
/// value.
///
/// The ReLU function is defined as:
/// \f[ \text{ReLU}(x) = \max(0, x) \f]
/// It outputs the input value if it is positive; otherwise, it outputs 0.
/// If the input is NaN, the function also returns NaN.
///
/// @tparam T The type of the input value, which must be a floating point type
/// (e.g., float, double).
/// @param x The input value for which the ReLU function is to be computed.
///          It should be a floating-point number.
/// @return The computed ReLU value of the input `x`, which will be in the range
/// [0, inf).
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto relu(T x) -> T {
    if (muc::isnan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    return std::max({}, x);
}

/// @brief Computes the ReLU (Rectified Linear Unit) of an integral value and
/// returns it as a floating-point type.
///
/// This function takes an integral input value, converts it to a floating-point
/// type, and computes the ReLU function. If the floating-point type `T` is
/// compliant with IEEE 754 (as determined by
/// `std::numeric_limits<T>::is_iec559`), it uses `std::max` to compute the ReLU
/// value. Otherwise, it defaults to invoking the ReLU function defined for
/// floating-point types. The resulting value is in the range [0, inf).
///
/// @tparam T The type of the output value (default is double).
///            This should be a floating-point type (e.g., float, double).
/// @tparam U The type of the input value, which must be an integral type (e.g.,
/// int, long).
/// @param x The input integral value for which the ReLU function is to be
/// computed.
/// @return The computed ReLU value of the input `x`, converted to type `T`,
/// which
///         will be in the range [0, inf).
template<typename T = double, typename U,
         std::enable_if_t<std::is_floating_point_v<T> && std::is_integral_v<U>,
                          bool> = true>
constexpr auto relu(U x) -> T {
    if constexpr (std::numeric_limits<T>::is_iec559) {
        return std::max({}, x);
    } else {
        return muc::relu(static_cast<T>(x));
    }
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::relu(-3.3) == 0);
static_assert(muc::relu(-2.3) == 0);
static_assert(muc::relu(-1.3) == 0);
static_assert(muc::relu(0.3) == 0.3);
static_assert(muc::relu(1.3) == 1.3);
static_assert(muc::relu(2.3) == 2.3);
static_assert(muc::relu(3.3) == 3.3);

static_assert(muc::relu(-3) == 0);
static_assert(muc::relu(-2) == 0);
static_assert(muc::relu(-1) == 0);
static_assert(muc::relu(0) == 0);
static_assert(muc::relu(1) == +1);
static_assert(muc::relu(2) == +2);
static_assert(muc::relu(3) == +3);

static_assert(muc::llround(-std::numeric_limits<double>::infinity()) == 0);
static_assert(muc::llround(std::numeric_limits<double>::infinity()) ==
              std::numeric_limits<double>::infinity());
static_assert(muc::llround(std::numeric_limits<double>::quiet_NaN()) ==
              std::numeric_limits<double>::quiet_NaN());

#endif
