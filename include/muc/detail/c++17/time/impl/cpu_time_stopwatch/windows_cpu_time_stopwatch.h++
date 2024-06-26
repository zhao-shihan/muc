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

#include <cstring>
#include <type_traits>

namespace muc::impl {

template<typename Time>
class cpu_time_stopwatch {
public:
    cpu_time_stopwatch() noexcept :
        m_current_process{GetCurrentProcess()},
        m_t0{clock_in_100ns()} {}

    auto s_used() const noexcept -> Time {
        return hecto_ns_used() / 10'000'000;
    }

    auto ms_used() const noexcept -> Time {
        return hecto_ns_used() / 10'000;
    }

    auto us_used() const noexcept -> Time {
        return hecto_ns_used() / 10;
    }

    auto ns_used() const noexcept -> Time {
        return hecto_ns_used() * 100;
    }

private:
    auto hecto_ns_used() const noexcept -> Time {
        return static_cast<Time>(clock_in_100ns().QuadPart - m_t0.QuadPart);
    }

    auto clock_in_100ns() const noexcept -> ULARGE_INTEGER {
        FILETIME t_creation;
        FILETIME t_exit;
        FILETIME t_kernel;
        FILETIME t_user;
        GetProcessTimes(m_current_process, &t_creation, &t_exit, &t_kernel,
                        &t_user);
        ULARGE_INTEGER t;
        static_assert(sizeof(FILETIME) == sizeof(ULARGE_INTEGER));
        static_assert(std::is_trivially_copyable_v<FILETIME>);
        static_assert(std::is_trivially_copyable_v<ULARGE_INTEGER>);
        std::memcpy(&t, &t_user, sizeof(FILETIME));
        return t;
    }

private:
    HANDLE m_current_process;
    ULARGE_INTEGER m_t0;
};

} // namespace muc::impl
