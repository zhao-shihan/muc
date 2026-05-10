// -*- C++ -*-
//
// Copyright (C) 2021-2026  Shihan Zhao
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

#include <future>

namespace muc {

/// @brief Defer the execution of a function until the result is needed. The
/// function will be executed in the same thread that waits on the returned
/// future.
/// @tparam F The type of the function to be deferred.
/// @tparam ...Args The types of the arguments to be passed to the function when
/// it is executed.
/// @param f The function to be deferred.
/// @param ...args The arguments to be passed to the function when it is
/// executed.
/// @return A future that will hold the result of the function after it is
/// executed.
template<typename F, typename... Args>
auto defer(F&& f, Args&&... args) -> auto {
    return std::async(std::launch::deferred, std::forward<F>(f),
                      std::forward<Args>(args)...);
}

} // namespace muc
