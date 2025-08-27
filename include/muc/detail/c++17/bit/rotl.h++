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

#include "muc/detail/c++17/bit/impl/is_really_unsigned_integral.h++"

#include <limits>
#include <type_traits>

namespace muc {

/// @brief Computes the result of bitwise left-rotating the value of x by s
/// positions. This overload participates in overload resolution only if T is an
/// unsigned integer type (that is, unsigned char, unsigned short, unsigned int,
/// unsigned long, unsigned long long, or an extended unsigned integer type).
/// @tparam T unsigned integer type
/// @param x value of unsigned integer type
/// @param s number of positions to shift
/// @return The result of bitwise left-rotating x by s positions.
template<typename T,
         std::enable_if_t<impl::is_really_unsigned_integral_v<T>, bool> = true>
[[nodiscard]] constexpr auto rotl(T x, int s) noexcept -> T {
    constexpr auto n{std::numeric_limits<T>::digits};
    const auto r{s % n};
    if (r > 0) {
        return (x << r) | (x >> (n - r));
    }
    if (r < 0) {
        return (x >> -r) | (x << (n + r));
    }
    return x; // r == 0
}

} // namespace muc
