// -*- C++ -*-
//
// Copyright (C) 2021-2025  Shihan Zhao
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

#include <chrono>
#include <mach/mach.h>
#include <mach/mach_time.h>

namespace muc::chrono::impl {

class thread_stopwatch {
public:
    thread_stopwatch() noexcept :
        m_current_thread{mach_thread_self()},
        m_t0{thread_clock_in_us()} {}

    ~thread_stopwatch() {
        mach_port_deallocate(mach_task_self(), m_current_thread);
    }

    auto reset() noexcept -> void {
        m_t0 = thread_clock_in_us();
    }

    auto read() const noexcept -> std::chrono::nanoseconds {
        return std::chrono::nanoseconds{(thread_clock_in_us() - m_t0) * 1000};
    }

private:
    auto thread_clock_in_us() const noexcept -> long long {
        mach_msg_type_number_t count{THREAD_BASIC_INFO_COUNT};
        thread_basic_info_data_t info{};
        kern_return_t kr{thread_info(m_current_thread, THREAD_BASIC_INFO,
                                     (thread_info_t)&info, &count)};
        if (kr != KERN_SUCCESS) {
            return 0;
        }
        return (info.user_time.seconds + info.system_time.seconds) *
                   1'000'000ll +
               (info.user_time.microseconds + info.system_time.microseconds);
    }

private:
    mach_port_t m_current_thread;
    long long m_t0;
};

} // namespace muc::chrono::impl
