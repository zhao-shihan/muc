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

#include <cassert>
#include <limits>
#include <type_traits>

namespace muc {

/// @brief A fast implementation of std::llround but never raise floating point
/// exceptions. Computes the nearest integer value to x (in floating-point
/// format), rounding halfway cases away from zero.
/// @tparam T floating-point type
/// @param x floating-point value
/// @return the nearest integer value to x, rounding halfway cases away from
/// zero
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto llround(T x) -> long long {
    assert(static_cast<T>(std::numeric_limits<long long>::min()) < x and
           x < static_cast<T>(std::numeric_limits<long long>::max()));
    return x + (x >= 0 ? 0.5 : -0.5);
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::llround(-3.) == -3);
static_assert(muc::llround(-2.) == -2);
static_assert(muc::llround(-1.) == -1);
static_assert(muc::llround(+0.) == +0);
static_assert(muc::llround(+1.) == +1);
static_assert(muc::llround(+2.) == +2);
static_assert(muc::llround(+3.) == +3);

static_assert(muc::llround(-3.3) == -3);
static_assert(muc::llround(-2.3) == -2);
static_assert(muc::llround(-1.3) == -1);
static_assert(muc::llround(+0.3) == +0);
static_assert(muc::llround(+1.3) == +1);
static_assert(muc::llround(+2.3) == +2);
static_assert(muc::llround(+3.3) == +3);

static_assert(muc::llround(-3.5) == -4);
static_assert(muc::llround(-2.5) == -3);
static_assert(muc::llround(-1.5) == -2);
static_assert(muc::llround(+0.5) == +1);
static_assert(muc::llround(+1.5) == +2);
static_assert(muc::llround(+2.5) == +3);
static_assert(muc::llround(+3.5) == +4);

static_assert(muc::llround(-3.7) == -4);
static_assert(muc::llround(-2.7) == -3);
static_assert(muc::llround(-1.7) == -2);
static_assert(muc::llround(+0.7) == +1);
static_assert(muc::llround(+1.7) == +2);
static_assert(muc::llround(+2.7) == +3);
static_assert(muc::llround(+3.7) == +4);

#endif
