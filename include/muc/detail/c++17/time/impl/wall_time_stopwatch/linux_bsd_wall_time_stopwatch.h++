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

#include <ctime>
#include <sys/time.h>

namespace muc::impl {

template<typename Time>
class wall_time_stopwatch {
public:
    wall_time_stopwatch() noexcept :
        m_t0{} {
        clock_gettime(CLOCK_MONOTONIC, &m_t0);
    }

    auto s_elapsed() const noexcept -> Time {
        return ns_elapsed() / 1'000'000'000;
    }

    auto ms_elapsed() const noexcept -> Time {
        return ns_elapsed() / 1'000'000;
    }

    auto us_elapsed() const noexcept -> Time {
        return ns_elapsed() / 1'000;
    }

    auto ns_elapsed() const noexcept -> Time {
        std::timespec t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        return static_cast<Time>(t.tv_sec - m_t0.tv_sec) * 1'000'000'000 +
               static_cast<Time>(t.tv_nsec - m_t0.tv_nsec);
    }

private:
    std::timespec m_t0;
};

} // namespace muc::impl
