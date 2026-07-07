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

#include "muc/detail/c++17/type_traits/impl/has_linear_combination.h++"

#include <type_traits>

namespace muc {

template<typename T>
struct is_linearly_combinable :
    std::conjunction<impl::has_scalar_mul<T>, impl::has_add_sub<T, T>> {};

template<typename T>
inline constexpr bool is_linearly_combinable_v{
    is_linearly_combinable<T>::value};

} // namespace muc

#ifdef MUC_STATIC_TEST

#include <array>
#include <complex>
#include <string>
#include <valarray>
#include <vector>

static_assert(muc::is_linearly_combinable_v<int>);
static_assert(muc::is_linearly_combinable_v<float>);
static_assert(muc::is_linearly_combinable_v<double>);
static_assert(muc::is_linearly_combinable_v<long double>);

static_assert(muc::is_linearly_combinable_v<std::complex<float>>);
static_assert(muc::is_linearly_combinable_v<std::complex<double>>);
static_assert(muc::is_linearly_combinable_v<std::complex<long double>>);

static_assert(muc::is_linearly_combinable_v<std::valarray<float>>);
static_assert(muc::is_linearly_combinable_v<std::valarray<double>>);
static_assert(muc::is_linearly_combinable_v<std::valarray<long double>>);

static_assert(not muc::is_linearly_combinable_v<std::array<float, 3>>);
static_assert(not muc::is_linearly_combinable_v<std::vector<double>>);
static_assert(not muc::is_linearly_combinable_v<std::string>);

#endif
