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

#include <type_traits>

namespace muc {

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
constexpr auto odd(T n) -> auto {
    return n & static_cast<decltype(n)>(1);
}

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
constexpr auto even(T n) -> auto {
    return not odd(n);
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(not muc::odd(0));
static_assert(not muc::odd(2));
static_assert(muc::odd(1));
static_assert(muc::odd(3));

static_assert(muc::even(0));
static_assert(muc::even(2));
static_assert(not muc::even(1));
static_assert(not muc::even(3));

#endif
