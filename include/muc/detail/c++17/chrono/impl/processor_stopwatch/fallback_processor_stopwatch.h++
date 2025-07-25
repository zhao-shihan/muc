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
#include "muc/detail/c++17/math/llround.h++"

#include <chrono>
#include <ctime>

namespace muc::chrono::impl {

class processor_stopwatch {
public:
    processor_stopwatch() noexcept :
        m_t0{std::clock()} {}

    auto reset() noexcept -> void {
        m_t0 = std::clock();
    }

    auto read() const noexcept -> std::chrono::nanoseconds {
        const auto clocks{std::clock() - m_t0};
        constexpr auto tick{
            div(static_cast<clock_t>(1'000'000'000), CLOCKS_PER_SEC)};
        if constexpr (tick.rem == 0) {
            return std::chrono::nanoseconds{clocks * tick.quot};
        } else {
            return std::chrono::nanoseconds{
                llround(clocks * (1e9 / CLOCKS_PER_SEC))};
        }
    }

private:
    std::clock_t m_t0;
};

} // namespace muc::chrono::impl
