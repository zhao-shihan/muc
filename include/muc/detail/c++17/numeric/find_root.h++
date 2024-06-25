// -*- C++ -*-
//
// Copyright 2024  Shihan Zhao
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
#include "muc/detail/c++17/math/pow.h++"
#include "muc/detail/c++17/numeric/midpoint.h++"

#include <cmath>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>

namespace muc::find_root {

template<typename T = double,
         std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
inline constexpr auto default_tolerance{
    muc::pow<std::numeric_limits<T>::digits / 2, T>(2) *
    std::numeric_limits<T>::epsilon()};

template<typename T, typename F, typename DF,
         std::enable_if_t<std::is_floating_point_v<T> and
                              std::is_invocable_v<F, T> and
                              std::is_invocable_v<DF, T>,
                          bool> = true>
constexpr auto
newton_raphson(const F& f, const DF& df, T x0, int max_iter = 1000,
               T tolerance = default_tolerance<T>) -> std::pair<T, bool> {
    auto x1{x0 - f(x0) / df(x0)};
    for (int i{}; i < max_iter; ++i) {
        if (muc::isnan(x1)) {
            break;
        }
        if (muc::abs(x1 - x0) <= muc::abs(muc::midpoint(x1, x0)) * tolerance) {
            return {x1, true};
        }
        x0 = x1;
        x1 = x0 - f(x0) / df(x0);
    }
    return {x1, false};
}

template<
    typename T, typename F,
    std::enable_if_t<std::is_floating_point_v<T> and std::is_invocable_v<F, T>,
                     bool> = true>
constexpr auto
secant(const F& f, T x0, std::optional<T> x1O = {}, int max_iter = 1000,
       T tolerance = default_tolerance<T>) -> std::pair<T, bool> {
    auto fx0{f(x0)};
    if (fx0 == 0) {
        return {x0, true};
    }
    auto x1{x1O.value_or(x0 + fx0 * 2 * tolerance /
                                  (f(x0 - tolerance) - f(x0 + tolerance)))};
    auto fx1{f(x1)};
    auto x2{(x0 * fx1 - x1 * fx0) / (fx1 - fx0)};
    for (int i{}; i < max_iter; ++i) {
        if (muc::isnan(x2)) {
            break;
        }
        if (muc::abs(x2 - x1) <= muc::abs(muc::midpoint(x2, x1)) * tolerance) {
            return {x2, true};
        }
        x0 = x1;
        fx0 = fx1;
        x1 = x2;
        fx1 = f(x2);
        x2 = (x0 * fx1 - x1 * fx0) / (fx1 - fx0);
    }
    return {x2, false};
}

} // namespace muc::find_root

#ifdef MUC_STATIC_TEST

static_assert([] {
    const auto [x, converged]{muc::find_root::newton_raphson(
        [](auto x) {
            return x * x - 1;
        },
        [](auto x) {
            return 2 * x;
        },
        0.5)};
    return converged and
           muc::abs(x - 1) < 2 * muc::find_root::default_tolerance<>;
}());

static_assert([] {
    const auto [x, converged]{muc::find_root::secant(
        [](auto x) {
            return x * x - 1;
        },
        0.5)};
    return converged and
           muc::abs(x - 1) < 2 * muc::find_root::default_tolerance<>;
}());

#endif
