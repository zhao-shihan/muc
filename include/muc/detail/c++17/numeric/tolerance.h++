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

#include "muc/detail/c++17/math/constexpr_cmath.h++"
#include "muc/detail/c++17/math/pow.h++"
#include "muc/detail/c++17/utility/assume.h++"

#include <algorithm>
#include <limits>
#include <type_traits>

namespace muc {

/// @brief Default relative tolerance value for floating-point type.
/// This constexpr variable provides a default tolerance value for
/// floating-points. It is set to (approximately) the square root of
/// the machine epsilon
/// @tparam T The floating-point type
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
inline constexpr auto default_rel_tol{
    muc::ipow(2ull, std::numeric_limits<T>::digits / 2) *
    std::numeric_limits<T>::epsilon()};

/// @brief Default absolute tolerance value for floating-point type.
/// This constexpr variable provides a default tolerance value for
/// floating-points. It is set to the machine epsilon of T
/// @tparam T The floating-point type
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
inline constexpr auto default_abs_tol{std::numeric_limits<T>::epsilon()};

/// @brief Tolerance control for floating-point algorithms.
///
/// This struct bundles absolute and relative tolerances into a single parameter
/// that can be passed to numeric algorithms. It provides `at()` member
/// functions to evaluate tolerance values at given points.
///
/// @tparam T The floating-point type
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
struct tolerance {
    T abs{default_abs_tol<T>}; ///< Absolute tolerance
    T rel{default_rel_tol<T>}; ///< Relative tolerance

    /// @brief Evaluate the tolerance at a point.
    /// @param x The point where the tolerance is evaluated
    /// @return abs + |x| * rel
    constexpr auto at(T x) const -> T {
        assume(abs > 0);
        assume(rel > 0);
        return abs + muc::abs(x) * rel;
    }

    /// @brief Evaluate the tolerance for a pair of points.
    /// @param x1 The first point
    /// @param x2 The second point
    /// @return abs + max(|x1|, |x2|) * rel
    constexpr auto at(T x1, T x2) const -> T {
        assume(abs > 0);
        assume(rel > 0);
        return abs + std::max(muc::abs(x1), muc::abs(x2)) * rel;
    }
};

} // namespace muc
