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

namespace muc::chrono::impl {

template<typename Time>
class processor_stopwatch {
public:
    processor_stopwatch() noexcept :
        m_current_process{GetCurrentProcess()},
        m_t0{clock_in_100ns()} {}

    auto reset() noexcept -> void {
        m_t0 = clock_in_100ns();
    }

    auto read() const noexcept -> nanoseconds<Time> {
        return nanoseconds<Time>{static_cast<Time>(clock_in_100ns() - m_t0) *
                                 100};
    }

private:
    auto clock_in_100ns() const noexcept -> ULONGLONG {
        FILETIME _1;
        FILETIME _2;
        FILETIME t_k{};
        FILETIME t_u{};
        GetProcessTimes(m_current_process, &_1, &_2, &t_k, &t_u);
        ULARGE_INTEGER t_kernel;
        t_kernel.LowPart = t_k.dwLowDateTime;
        t_kernel.HighPart = t_k.dwHighDateTime;
        ULARGE_INTEGER t_user;
        t_user.LowPart = t_u.dwLowDateTime;
        t_user.HighPart = t_u.dwHighDateTime;
        return t_kernel.QuadPart + t_user.QuadPart;
    }

private:
    HANDLE m_current_process;
    ULONGLONG m_t0;
};

} // namespace muc::chrono::impl
