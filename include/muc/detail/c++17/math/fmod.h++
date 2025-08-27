// -*- C++ -*-
//
// Copyright (C) 2021-2025  Shihan Zhao
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

/// @brief Fast but less precise floating-point modulus. This implementation
/// trades off some precision for improved performance.
///
/// Implements a faster alternative to std::fmod. This may not equal
/// std::fmod(x, y), when the rounding of x / y to initialize the argument of
/// std::trunc loses too much precision.
///
/// @tparam T Floating-point type (auto-deduced)
/// @param x Dividend value
/// @param y Divisor value
/// @return x - trunc(x/y) * y
///
/// @warning Not a drop-in replacement for std::fmod in precision sensitive
/// cases.
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
auto fmod(T x, T y) -> T {
    return x - std::trunc(x / y) * y;
}

} // namespace muc
