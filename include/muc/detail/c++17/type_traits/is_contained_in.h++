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

#include <type_traits>

namespace muc {

template<typename T, typename... Us>
struct is_contained_in : std::bool_constant<(... or std::is_same_v<T, Us>)> {};

template<typename T, typename... Us>
inline constexpr bool is_contained_in_v{is_contained_in<T, Us...>::value};

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::is_contained_in_v<int, int>);
static_assert(muc::is_contained_in_v<void, void>);
static_assert(muc::is_contained_in_v<int[], int[]>);
static_assert(muc::is_contained_in_v<int[4], int[4], int[4], int[3]>);
static_assert(muc::is_contained_in_v<int, int, void, int[4], int>);
static_assert(muc::is_contained_in_v<void, void, double, void>);
static_assert(not muc::is_contained_in_v<int, float>);
static_assert(not muc::is_contained_in_v<void, int[]>);
static_assert(not muc::is_contained_in_v<int[3], int[4]>);
static_assert(not muc::is_contained_in_v<int, float, float, double>);
static_assert(not muc::is_contained_in_v<void, int[3], double, double>);
static_assert(not muc::is_contained_in_v<int[], void*, void>);

#endif
