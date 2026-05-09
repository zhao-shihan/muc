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

#include <algorithm>
#include <limits>
#include <type_traits>
#include <utility>

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

/// @brief Calculate tolerance value for floating-point type at a given point
/// @tparam T The floating-point type
/// @param x The point where the tolerance is evaluated
/// @param abs_tol The absolute tolerance (default is default_abs_tol<T>)
/// @param rel_tol The relative tolerance (default is default_rel_tol<T>)
/// @return The tolerance value at point x
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto tolerance(T x, T abs_tol = default_abs_tol<T>,
                         T rel_tol = default_rel_tol<T>) -> T {
    return abs_tol + muc::abs(x) * rel_tol;
}

/// @brief Calculate tolerance value for floating-point type with two points
/// @tparam T The floating-point type
/// @param x A pair of points where the tolerance is evaluated
/// @param abs_tol The absolute tolerance (default is default_abs_tol<T>)
/// @param rel_tol The relative tolerance (default is default_rel_tol<T>)
/// @return The tolerance value with the two points
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto tolerance(std::pair<T, T> x, T abs_tol = default_abs_tol<T>,
                         T rel_tol = default_rel_tol<T>) -> T {
    return abs_tol + std::max(muc::abs(x.first), muc::abs(x.second)) * rel_tol;
}

} // namespace muc
