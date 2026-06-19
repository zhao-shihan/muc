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

#include <new>
#include <utility>

namespace muc {

/// @brief A backport of std::construct_at. Creates a T object initialized with
/// args... at the memory location pointed to by ptr.
///
/// This function uses placement new to construct an object of type T at the
/// given address, forwarding the provided arguments to T's constructor. This
/// is equivalent to the C++20 std::construct_at.
///
/// @tparam T The type of object to construct
/// @tparam Args The types of arguments to forward to T's constructor
/// @param ptr Pointer to the memory location where the object will be
/// constructed. Must point to properly aligned storage of sufficient size
/// @param args Arguments to forward to T's constructor
///
/// @return A pointer to the newly constructed T object (same as ptr)
template<typename T, typename... Args>
auto construct_at(T* ptr, Args&&... args) -> T* {
    return ::new (static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
}

} // namespace muc
