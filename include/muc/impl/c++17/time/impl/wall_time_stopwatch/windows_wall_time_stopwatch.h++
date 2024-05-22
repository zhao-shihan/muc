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

#if defined _MSC_VER and not defined __clang__ and not defined __GNUC__ and \
    not defined NOMINMAX
#    define NOMINMAX // Otherwise MS compilers act like idiots when using
                     // std::numeric_limits<>::max() and including windows.h
#endif
#ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    undef WIN32_LEAN_AND_MEAN
#else
#    include <windows.h>
#endif

namespace muc::impl {

template<typename Time>
class wall_time_stopwatch {
public:
    wall_time_stopwatch() noexcept :
        m_frequency{},
        m_t0{} {
        QueryPerformanceFrequency(&m_frequency);
        QueryPerformanceCounter(&m_t0);
    }

    auto s_elapsed() const noexcept -> Time {
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);
        return static_cast<Time>(t.QuadPart - m_t0.QuadPart) /
               m_frequency.QuadPart;
    }

    auto ms_elapsed() const noexcept -> Time {
        return s_elapsed() * 1'000;
    }

    auto us_elapsed() const noexcept -> Time {
        return s_elapsed() * 1'000'000;
    }

    auto ns_elapsed() const noexcept -> Time {
        return s_elapsed() * 1'000'000'000;
    }

private:
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_t0;
};

} // namespace muc::impl
