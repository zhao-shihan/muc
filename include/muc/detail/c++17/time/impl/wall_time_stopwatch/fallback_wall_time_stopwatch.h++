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

#include <chrono>

namespace muc::impl {

template<typename Time>
class wall_time_stopwatch {
private:
    using sc = std::chrono::steady_clock;
    using s = std::chrono::duration<Time, std::ratio<1>>;
    using ms = std::chrono::duration<Time, std::milli>;
    using us = std::chrono::duration<Time, std::micro>;
    using ns = std::chrono::duration<Time, std::nano>;

public:
    wall_time_stopwatch() noexcept :
        m_t0{sc::now()} {}

    auto s_elapsed() const noexcept -> Time {
        s{sc::now() - m_t0}.count();
    }

    auto ms_elapsed() const noexcept -> Time {
        ms{sc::now() - m_t0}.count();
    }

    auto us_elapsed() const noexcept -> Time {
        us{sc::now() - m_t0}.count();
    }

    auto ns_elapsed() const noexcept -> Time {
        ns{sc::now() - m_t0}.count();
    }

private:
    sc::time_point m_t0;
};

} // namespace muc::impl
