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
#include "muc/detail/c++17/math/llround.h++"
#include "muc/detail/c++17/math/pow.h++"
#include "muc/detail/c++17/numeric/midpoint.h++"

#include <algorithm>
#include <cmath>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>

namespace muc::find_root {

/// @brief Default tolerance value for convergence in root-finding algorithms.
/// This constexpr variable provides a default tolerance value for convergence
/// in root-finding algorithms. It is calculated as half of the number of
/// significant digits of the floating-point type T.
/// @tparam T The type of the input value (default is double).
template<typename T = double,
         std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
inline constexpr auto default_tolerance{
    muc::pow<std::numeric_limits<T>::digits / 2, T>(2) *
    std::numeric_limits<T>::epsilon()};

/// @brief Newton-Raphson method for finding roots of a function.
/// This function implements the Newton-Raphson method for finding roots of a
/// function. It iteratively calculates the next value using the function and
/// its derivative until convergence.
/// @tparam T The type of the input value.
/// @tparam F The type of the function to evaluate.
/// @tparam DF The type of the derivative function.
/// @param f The function to evaluate.
/// @param df The derivative function.
/// @param x0 The initial guess for the root.
/// @param max_iter The maximum number of iterations allowed (default is 1000).
/// @param tolerance The tolerance value for convergence (default is
/// default_tolerance<T>).
/// @return A pair containing the root value and a boolean indicating if
/// convergence was achieved.
template<typename T, typename F, typename DF,
         std::enable_if_t<std::is_floating_point_v<T> and
                              std::is_invocable_r_v<T, F, T> and
                              std::is_invocable_r_v<T, DF, T>,
                          bool> = true>
constexpr auto
newton_raphson(F&& f, DF&& df, T x0, int max_iter = 1000,
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

/// @brief Secant method for finding roots of a function.
/// This function implements the secant method for finding roots of a function.
/// It iteratively calculates the next value using two initial guesses until
/// convergence.
/// @tparam T The type of the input value.
/// @tparam F The type of the function to evaluate.
/// @param f The function to evaluate.
/// @param x0 The first initial guess for the root.
/// @param x1O The optional second initial guess for the root (default is
/// empty).
/// @param max_iter The maximum number of iterations allowed (default is 1000).
/// @param tolerance The tolerance value for convergence (default is
/// default_tolerance<T>).
/// @return A pair containing the root value and a boolean indicating if
/// convergence was achieved.
template<typename T, typename F,
         std::enable_if_t<std::is_floating_point_v<T> and
                              std::is_invocable_r_v<T, F, T>,
                          bool> = true>
constexpr auto
secant(F&& f, T x0, std::optional<T> x1O = {}, int max_iter = 1000,
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

/// @brief Brent's method for finding roots of a function.
/// This function implements Brent's method for finding roots of a function.
/// It iteratively narrows down the root using a combination of bisection,
/// secant, and inverse quadratic interpolation.
/// @tparam T The type of the input value.
/// @tparam F The type of the function to evaluate.
/// @param f The function to evaluate.
/// @param x1 The first initial guess for the root.
/// @param x2 The second initial guess for the root.
/// @param max_iter The maximum number of iterations allowed (default is
/// 100000).
/// @param tolerance The tolerance value for convergence (default is
/// default_tolerance<T>).
/// @return A pair containing the root value and a boolean indicating if
/// convergence was achieved.
template<typename T, typename F,
         std::enable_if_t<std::is_floating_point_v<T> and
                              std::is_invocable_r_v<T, F, T>,
                          bool> = true>
constexpr auto
zbrent(F&& f, T x1, T x2, int max_iter = 100000,
       T tolerance = default_tolerance<T>) -> std::pair<T, bool> {
    auto a{x1};
    auto b{x2};
    auto c{x2};
    auto d{x2 - x1};
    auto e{x2 - x1};
    auto fa{f(a)};
    auto fb{f(b)};
    auto fc{fb};
    // Check if there is a single zero in range
    if (fa * fb > 0) {
        return {b, false};
    }
    // Start search
    for (auto iter{0ll}; iter < max_iter; ++iter) {
        if ((fb > 0 and fc > 0) or (fb < 0 and fc < 0)) {
            c = a;
            fc = fa;
            d = b - a;
            e = d;
        }
        if (muc::abs(fc) < muc::abs(fb)) {
            a = b;
            b = c;
            c = a;
            fa = fb;
            fb = fc;
            fc = fa;
        }
        if (muc::isnan(fb)) {
            break;
        }
        const auto tol{2 * std::numeric_limits<T>::epsilon() * muc::abs(b) +
                       tolerance / 2};
        const auto xm{(c - b) / 2};
        if (muc::abs(xm) <= tol or fb == 0) {
            return {b, true};
        }
        if (muc::abs(e) >= tol and muc::abs(fa) > muc::abs(fb)) {
            T p;
            T q;
            const auto s{fb / fa};
            if (a == c) {
                p = 2 * xm * s;
                q = 1 - s;
            } else {
                q = fa / fc;
                const auto r1{fb / fc};
                p = s * (2 * xm * q * (q - r1) - (b - a) * (r1 - 1));
                q = (q - 1) * (r1 - 1) * (s - 1);
            }
            if (p > 0)
                q = -q;
            p = muc::abs(p);
            if (2 * p <
                std::min(3 * xm * q - muc::abs(tol * q), muc::abs(e * q))) {
                e = d;
                d = p / q;
            } else {
                d = xm;
                e = d;
            }
        } else {
            d = xm;
            e = d;
        }
        a = b;
        fa = fb;
        if (muc::abs(d) > tol) {
            b += d;
        } else {
            b += (xm > 1) ? tol : -tol;
        }
        fb = f(b);
    }
    // nan or max_iter reached
    return {b, false};
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

static_assert([] {
    const auto [x, converged]{muc::find_root::zbrent(
        [](auto x) {
            return x * x - 1;
        },
        0.5, 2.5)};
    return converged and
           muc::abs(x - 1) < 2 * muc::find_root::default_tolerance<>;
}());

#endif
