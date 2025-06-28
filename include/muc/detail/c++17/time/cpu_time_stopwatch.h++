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

// Linux/BSD implementation:
#if (defined linux or defined __linux__ or defined __linux) or             \
    (defined __DragonFly__ or defined __FreeBSD__ or defined __NetBSD__ or \
     defined __OpenBSD__)
#include "muc/detail/c++17/time/impl/cpu_time_stopwatch/linux_bsd_cpu_time_stopwatch.h++"
// Windows implementation:
#elif defined _WIN32
#include "muc/detail/c++17/time/impl/cpu_time_stopwatch/windows_cpu_time_stopwatch.h++"
// Fallback implementation:
#else
#include "muc/detail/c++17/time/impl/cpu_time_stopwatch/fallback_cpu_time_stopwatch.h++"
#endif

#include <limits>
#include <type_traits>

namespace muc {

/// @brief CPU stopwatch better than std::clock() when available.
template<typename Time = double>
class cpu_time_stopwatch {
    static_assert(std::is_floating_point_v<Time>,
                  "the value type for stopwatch should be a floating point");
    static_assert(std::numeric_limits<Time>::digits >=
                      std::numeric_limits<double>::digits,
                  "stopwatch value type should be at least as long as double");

public:
    using value_type = Time;

public:
    auto s_used() const noexcept -> value_type {
        return m_impl.s_used();
    }

    auto ms_used() const noexcept -> value_type {
        return m_impl.ms_used();
    }

    auto us_used() const noexcept -> value_type {
        return m_impl.us_used();
    }

    auto ns_used() const noexcept -> value_type {
        return m_impl.ns_used();
    }

    auto reset() noexcept -> void {
        m_impl = {};
    }

private:
    impl::cpu_time_stopwatch<Time> m_impl;
};

} // namespace muc
