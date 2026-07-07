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

template<typename T, typename U>
struct is_linearly_combinable_with :
    std::conjunction<impl::has_scalar_mul<T>, impl::has_scalar_mul<U>,
                     impl::has_add_sub<T, U>, impl::has_add_sub<U, T>> {};

template<typename T, typename U>
inline constexpr bool is_linearly_combinable_with_v{
    is_linearly_combinable_with<T, U>::value};

} // namespace muc

#ifdef MUC_STATIC_TEST

#include <array>
#include <complex>
#include <string>
#include <valarray>
#include <vector>

static_assert(muc::is_linearly_combinable_with_v<int, int>);
static_assert(muc::is_linearly_combinable_with_v<float, float>);
static_assert(muc::is_linearly_combinable_with_v<double, double>);
static_assert(muc::is_linearly_combinable_with_v<long double, long double>);

static_assert(muc::is_linearly_combinable_with_v<int, float>);
static_assert(muc::is_linearly_combinable_with_v<float, double>);
static_assert(muc::is_linearly_combinable_with_v<double, long double>);

static_assert(muc::is_linearly_combinable_with_v<std::complex<float>,
                                                 std::complex<float>>);
static_assert(muc::is_linearly_combinable_with_v<std::complex<double>,
                                                 std::complex<double>>);
static_assert(muc::is_linearly_combinable_with_v<std::complex<long double>,
                                                 std::complex<long double>>);

static_assert(
    not muc::is_linearly_combinable_with_v<double, std::complex<double>>);
static_assert(
    not muc::is_linearly_combinable_with_v<std::complex<double>, double>);
static_assert(not muc::is_linearly_combinable_with_v<std::complex<float>,
                                                     std::complex<double>>);
static_assert(not muc::is_linearly_combinable_with_v<
              std::complex<double>, std::complex<long double>>);

static_assert(muc::is_linearly_combinable_with_v<std::valarray<float>,
                                                 std::valarray<float>>);
static_assert(muc::is_linearly_combinable_with_v<std::valarray<double>,
                                                 std::valarray<double>>);
static_assert(muc::is_linearly_combinable_with_v<std::valarray<long double>,
                                                 std::valarray<long double>>);

static_assert(
    not muc::is_linearly_combinable_with_v<double, std::valarray<double>>);
static_assert(
    not muc::is_linearly_combinable_with_v<std::valarray<double>, double>);
static_assert(not muc::is_linearly_combinable_with_v<std::valarray<float>,
                                                     std::valarray<double>>);
static_assert(not muc::is_linearly_combinable_with_v<
              std::valarray<double>, std::valarray<long double>>);

static_assert(not muc::is_linearly_combinable_with_v<std::array<float, 3>,
                                                     std::vector<double>>);
static_assert(
    not muc::is_linearly_combinable_with_v<std::vector<float>, std::string>);
static_assert(
    not muc::is_linearly_combinable_with_v<std::string, std::array<double, 3>>);

#endif
