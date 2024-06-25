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

#include <ctime>

namespace muc::impl {

template<typename Time>
class cpu_time_stopwatch {
public:
    cpu_time_stopwatch() noexcept :
        m_t0{std::clock()} {}

    auto s_used() const noexcept -> Time {
        static_cast<Time>(std::clock() - m_t0) / CLOCKS_PER_SEC;
    }

    auto ms_used() const noexcept -> Time {
        return s_used() * 1'000;
    }

    auto us_used() const noexcept -> Time {
        return s_used() * 1'000'000;
    }

    auto ns_used() const noexcept -> Time {
        return s_used() * 1'000'000'000;
    }

private:
    std::clock_t m_t0;
};

} // namespace muc::impl
