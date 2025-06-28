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

#include "muc/detail/c++17/bit/impl/is_really_unsigned_integral.h++"

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
