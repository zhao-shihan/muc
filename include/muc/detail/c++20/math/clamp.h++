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

#include "muc/detail/c++20/ceta_string/ceta_string.h++"

#include <cassert>
#include <cmath>
#include <concepts>
#include <type_traits>

namespace muc {

template<ceta_string<3> Mode, std::floating_point T1, std::floating_point T2,
         std::floating_point T3>
    requires(Mode == "[]" or Mode == "[)" or Mode == "(]" or Mode == "()")
constexpr auto clamp(T1 value, T2 low,
                     T3 high) -> std::common_type_t<T1, T2, T3> {
    const std::common_type_t<T1, T2, T3> v{value};
    const std::common_type_t<T1, T2, T3> lo{low};
    const std::common_type_t<T1, T2, T3> hi{high};
    assert(lo < hi);
    if constexpr (Mode == "[]") {
        return v < lo ? lo : hi < v ? hi : v;
    } else if constexpr (Mode == "[)") {
        return v < lo ? lo : hi <= v ? std::nexttoward(hi, lo) : v;
    } else if constexpr (Mode == "(]") {
        return v <= lo ? std::nexttoward(lo, hi) : hi < v ? hi : v;
    } else if constexpr (Mode == "()") {
        return v <= lo ? std::nexttoward(lo, hi) :
               hi <= v ? std::nexttoward(hi, lo) :
                         v;
    }
}

} // namespace muc
