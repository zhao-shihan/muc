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

#include "muc/detail/c++17/math/constexpr_cmath.h++"
#include "muc/detail/c++17/numeric/default_tolerance.h++"
#include "muc/detail/c++17/utility/assume.h++"

#include <limits>
#include <type_traits>

namespace muc {

/// @brief Checks if two floating-point numbers are approximately equal.
///
/// This function compares two floating-point values (`a` and `b`) for
/// approximate equality using a combination of absolute and relative
/// tolerances.
///
/// @tparam T Floating-point type.
/// @param a First floating-point value to compare.
/// @param b Second floating-point value to compare.
/// @param rel_tol Relative tolerance (default is `default_rel_tol<T>`).
/// @param abs_tol Absolute tolerance (default is `default_abs_tol<T>`).
/// @return `true` if `a` and `b` are approximately equal within tolerances,
/// `false` otherwise.
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto isclose(T a, T b, T rel_tol = default_rel_tol<T>,
                       T abs_tol = default_abs_tol<T>) -> bool {
    assume(rel_tol > 0);
    assume(abs_tol > 0);
    if (a == b) {
        return true;
    }
    const auto tol{muc::tolerance({a, b}, abs_tol, rel_tol)};
    return muc::abs(a - b) <= tol;
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::isclose(0., 0.));
static_assert(muc::isclose(1., 1. + std::numeric_limits<double>::epsilon()));
static_assert(muc::isclose(-1., -1. + std::numeric_limits<double>::epsilon()));
static_assert(not muc::isclose(-1., 0.));
static_assert(not muc::isclose(0., 1.));
static_assert(not muc::isclose(1., 2.));
static_assert(not muc::isclose(0., muc::default_rel_tol<double>));

static_assert(muc::isclose(0.f, 0.f));
static_assert(muc::isclose(1.f, 1.f + std::numeric_limits<float>::epsilon()));
static_assert(muc::isclose(-1.f, -1.f + std::numeric_limits<float>::epsilon()));
static_assert(not muc::isclose(-1.f, 0.f));
static_assert(not muc::isclose(0.f, 1.f));
static_assert(not muc::isclose(1.f, 2.f));
static_assert(not muc::isclose(0.f, muc::default_rel_tol<float>));

static_assert(muc::isclose(0.l, 0.l));
static_assert(muc::isclose(1.l,
                           1.l + std::numeric_limits<long double>::epsilon()));
static_assert(muc::isclose(-1.l,
                           -1.l + std::numeric_limits<long double>::epsilon()));
static_assert(not muc::isclose(-1.l, 0.l));
static_assert(not muc::isclose(0.l, 1.l));
static_assert(not muc::isclose(1.l, 2.l));
static_assert(not muc::isclose(0.l, muc::default_rel_tol<long double>));

#endif
