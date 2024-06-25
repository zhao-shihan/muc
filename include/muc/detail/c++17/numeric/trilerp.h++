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

#include "muc/detail/c++17/numeric/bilerp.h++"
#include "muc/detail/c++17/numeric/lerp.h++"
#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <type_traits>

namespace muc {

/// @brief Performs trilinear interpolation.
/// @tparam T value type, can be a scalar or vector or something.
/// @tparam U scalar type
/// @param c values on square grid. See note for details.
/// @param u interpolation parameter. 0<u<1 implies interpolation, otherwise
/// extrapolation.
/// @param v interpolation parameter. 0<v<1 implies interpolation, otherwise
/// extrapolation.
/// @return interpolated value.
/// @note For parameter c:
///
///         c011    c111
///            +----+
///      c010 /|   /|
///          +----+ + c101
///          |c001|/
/// v w      +----+
/// ^ ^  c000     c100
/// |/
/// +----> u
template<typename T, typename U,
         std::enable_if_t<is_general_arithmetic_v<T>, bool> = true,
         std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr auto trilerp(const T& c000, const T& c001, const T& c010,
                       const T& c011, const T& c100, const T& c101,
                       const T& c110, const T& c111, U u, U v, U w) -> T {
    return muc::bilerp(muc::lerp(c000, c001, w), muc::lerp(c010, c011, w),
                       muc::lerp(c100, c101, w), muc::lerp(c110, c111, w), u,
                       v);
}

} // namespace muc
