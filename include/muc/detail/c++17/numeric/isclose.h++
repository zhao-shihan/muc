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

#include "muc/detail/c++17/math/constexpr_cmath.h++"
#include "muc/detail/c++17/numeric/default_tolerance.h++"
#include "muc/detail/c++17/utility/assume.h++"

#include <algorithm>
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
/// @param rel_epsilon Relative tolerance (default: `default_tolerance<T>`).
///        Represents acceptable error relative to input magnitude.
/// @param abs_epsilon Absolute tolerance (default:
///        `std::numeric_limits<T>::epsilon()`). Represents minimum absolute
///        error threshold.
/// @return `true` if `a` and `b` are approximately equal within tolerances,
/// `false` otherwise.
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto isclose(T a, T b, T rel_epsilon = default_tolerance<T>,
                       T abs_epsilon = std::numeric_limits<T>::epsilon())
    -> bool {
    assume(rel_epsilon > 0);
    assume(abs_epsilon > 0);
    if (a == b) {
        return true;
    }
    const auto epsilon{
        std::max(abs_epsilon, rel_epsilon * std::max(abs(a), abs(b)))};
    return abs(a - b) <= epsilon;
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::isclose(0., 0.));
static_assert(muc::isclose(1., 1. + std::numeric_limits<double>::epsilon()));
static_assert(muc::isclose(-1., -1. + std::numeric_limits<double>::epsilon()));
static_assert(not muc::isclose(-1., 0.));
static_assert(not muc::isclose(0., 1.));
static_assert(not muc::isclose(1., 2.));
static_assert(not muc::isclose(0., muc::default_tolerance<double>));

static_assert(muc::isclose(0.f, 0.f));
static_assert(muc::isclose(1.f, 1.f + std::numeric_limits<float>::epsilon()));
static_assert(muc::isclose(-1.f, -1.f + std::numeric_limits<float>::epsilon()));
static_assert(not muc::isclose(-1.f, 0.f));
static_assert(not muc::isclose(0.f, 1.f));
static_assert(not muc::isclose(1.f, 2.f));
static_assert(not muc::isclose(0.f, muc::default_tolerance<float>));

static_assert(muc::isclose(0.l, 0.l));
static_assert(muc::isclose(1.l,
                           1.l + std::numeric_limits<long double>::epsilon()));
static_assert(muc::isclose(-1.l,
                           -1.l + std::numeric_limits<long double>::epsilon()));
static_assert(not muc::isclose(-1.l, 0.l));
static_assert(not muc::isclose(0.l, 1.l));
static_assert(not muc::isclose(1.l, 2.l));
static_assert(not muc::isclose(0.l, muc::default_tolerance<long double>));

#endif
