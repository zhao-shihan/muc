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

#include <concepts>
#include <initializer_list>
#include <limits>
#include <ranges>

namespace muc {

template<std::floating_point T, std::ranges::range C = std::initializer_list<T>>
constexpr auto polynomial(C&& coeff, T x) -> T {
    auto c{std::ranges::crbegin(coeff)};
    const auto end{std::ranges::crend(coeff)};
    if (c == end) {
        using nl = std::numeric_limits<T>;
        return nl::has_quiet_NaN ? nl::quiet_NaN() : 0;
    }
    T p{*c++};
    while (c != end) {
        p = p * x + *c++;
    }
    return p;
}

template<std::floating_point T = double,
         std::ranges::range C = std::initializer_list<T>>
constexpr auto polynomial(C&& coeff, std::integral auto x) -> T {
    return polynomial<T>(coeff, x);
}

} // namespace muc
