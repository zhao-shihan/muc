// -*- C++ -*-
//
// Copyright (C) 2021-2026  Shihan Zhao
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

#include <limits>
#include <type_traits>

namespace muc {

template<typename R = double,
         std::enable_if_t<std::is_arithmetic_v<R>, bool> = true>
constexpr auto factorial(int n) -> R {
    if (n < 0) {
        if constexpr (std::is_floating_point_v<R>) {
            return std::numeric_limits<R>::quiet_NaN();
        } else {
            return R{};
        }
    }
    R result{1};
    for (int k{2}; k <= n; ++k) {
        result *= k;
    }
    return result;
}

template<typename R = unsigned long long,
         std::enable_if_t<std::is_integral_v<R>, bool> = true>
constexpr auto ifactorial(int n) -> R {
    return factorial<R>(n);
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::factorial(-1) != muc::factorial(-1)); // NaN check
static_assert(muc::factorial(0) == 1);
static_assert(muc::factorial(1) == 1);
static_assert(muc::factorial(2) == 2);
static_assert(muc::factorial(3) == 6);
static_assert(muc::factorial(4) == 24);
static_assert(muc::factorial(5) == 120);
static_assert(muc::factorial(6) == 720);
static_assert(muc::factorial(7) == 5040);
static_assert(muc::factorial(8) == 40320);
static_assert(muc::factorial(9) == 362880);
static_assert(muc::factorial(10) == 3628800);

static_assert(muc::ifactorial(-1) == 0);
static_assert(muc::ifactorial(0) == 1);
static_assert(muc::ifactorial(1) == 1);
static_assert(muc::ifactorial(2) == 2);
static_assert(muc::ifactorial(3) == 6);
static_assert(muc::ifactorial(4) == 24);
static_assert(muc::ifactorial(5) == 120);
static_assert(muc::ifactorial(6) == 720);
static_assert(muc::ifactorial(7) == 5040);
static_assert(muc::ifactorial(8) == 40320);
static_assert(muc::ifactorial(9) == 362880);
static_assert(muc::ifactorial(10) == 3628800);

#endif
