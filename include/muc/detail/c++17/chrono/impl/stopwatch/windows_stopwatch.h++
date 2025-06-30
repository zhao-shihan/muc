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
class stopwatch {
public:
    stopwatch() :
        m_frequency{},
        m_t0{} {
        QueryPerformanceFrequency(&m_frequency);
        reset();
    }

    auto reset() noexcept -> void {
        QueryPerformanceCounter(&m_t0);
    }

    auto read() const noexcept -> nanoseconds<Time> {
        LARGE_INTEGER t{};
        QueryPerformanceCounter(&t);
        return nanoseconds<Time>{static_cast<Time>(t.QuadPart - m_t0.QuadPart) *
                                 1e9 / m_frequency.QuadPart};
    }

private:
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_t0;
};

} // namespace muc::chrono::impl
