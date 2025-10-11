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

#include <cstddef>
#include <type_traits>

namespace muc {

template<typename T, typename... Ts>
struct type_count :
    std::integral_constant<std::size_t, (... + static_cast<std::size_t>(
                                                   std::is_same_v<T, Ts>))> {};

template<typename T, typename... Ts>
inline constexpr auto type_count_v{type_count<T, Ts...>::value};

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::type_count_v<int, int> == 1);
static_assert(muc::type_count_v<void, void> == 1);
static_assert(muc::type_count_v<int[], int[]> == 1);
static_assert(muc::type_count_v<int[4], int[4], int[4], int[3]> == 2);
static_assert(muc::type_count_v<int, int, void, int[4], int> == 2);
static_assert(muc::type_count_v<void, void, double, void> == 2);
static_assert(muc::type_count_v<int, float> == 0);
static_assert(muc::type_count_v<void, int[]> == 0);
static_assert(muc::type_count_v<int[3], int[4]> == 0);
static_assert(muc::type_count_v<int, float, float, double> == 0);
static_assert(muc::type_count_v<void, int[3], double, double> == 0);
static_assert(muc::type_count_v<int[], void*, void> == 0);

#endif
