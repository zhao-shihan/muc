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

// Copyright (c) 2022, Matthew Bentley (mattreecebentley@gmail.com)
// www.plflib.org

// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

// Linux/BSD implementation:
#if (defined linux or defined __linux__ or defined __linux) or             \
    (defined __DragonFly__ or defined __FreeBSD__ or defined __NetBSD__ or \
     defined __OpenBSD__)
#include "muc/detail/c++17/time/impl/wall_time_stopwatch/linux_bsd_wall_time_stopwatch.h++"
// Windows implementation:
#elif defined _WIN32
#include "muc/detail/c++17/time/impl/wall_time_stopwatch/windows_wall_time_stopwatch.h++"
// Fallback implementation:
#else
#include "muc/detail/c++17/time/impl/wall_time_stopwatch/fallback_wall_time_stopwatch.h++"
#endif

#include <limits>
#include <type_traits>

namespace muc {

/// @brief high-precision cross-platform (linux/bsd/windows/etc.) simple
/// stopwatch class
template<typename Time = double>
class wall_time_stopwatch {
    static_assert(std::is_floating_point_v<Time>,
                  "the value type for stopwatch should be a floating point");
    static_assert(std::numeric_limits<Time>::digits >=
                      std::numeric_limits<double>::digits,
                  "stopwatch value type should be at least as long as double");

public:
    using value_type = Time;

public:
    auto s_elapsed() const noexcept -> value_type {
        return m_impl.s_elapsed();
    }

    auto ms_elapsed() const noexcept -> value_type {
        return m_impl.ms_elapsed();
    }

    auto us_elapsed() const noexcept -> value_type {
        return m_impl.us_elapsed();
    }

    auto ns_elapsed() const noexcept -> value_type {
        return m_impl.ns_elapsed();
    }

    auto reset() noexcept -> void {
        m_impl = {};
    }

private:
    impl::wall_time_stopwatch<Time> m_impl;
};

} // namespace muc
