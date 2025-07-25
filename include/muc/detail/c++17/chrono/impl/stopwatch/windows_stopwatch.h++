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

#if defined _MSC_VER and not defined __clang__ and not defined __GNUC__ and \
    not defined NOMINMAX
#define NOMINMAX // Otherwise MS compilers act like idiots when using
                 // std::numeric_limits<>::max() and including windows.h
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#else
#include <windows.h>
#endif

#include "muc/detail/c++17/math/constexpr_cmath.h++"

#include <chrono>
#include <variant>

namespace muc::chrono::impl {

class stopwatch {
public:
    stopwatch() noexcept :
        m_tick{},
        m_t0{} {
        LARGE_INTEGER frequency{};
        QueryPerformanceFrequency(&frequency);
        const auto tick{div(1'000'000'000ll, frequency.QuadPart)};
        if (tick.rem == 0) {
            m_tick = tick.quot;
        } else {
            m_tick = 1e9 / frequency.QuadPart;
        }
        reset();
    }

    auto reset() noexcept -> void {
        QueryPerformanceCounter(&m_t0);
    }

    auto read() const noexcept -> std::chrono::nanoseconds {
        LARGE_INTEGER t{};
        QueryPerformanceCounter(&t);
        const auto clocks{t.QuadPart - m_t0.QuadPart};
        if (m_tick.index() == 0) {
            return std::chrono::nanoseconds{clocks * get<LONGLONG>(m_tick)};
        } else {
            return std::chrono::nanoseconds{
                llround(clocks * get<double>(m_tick))};
        }
    }

private:
    std::variant<LONGLONG, double> m_tick;
    LARGE_INTEGER m_t0;
};

} // namespace muc::chrono::impl
