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

#include "muc/detail/c++20/concepts/move_assignable.h++"

#include <concepts>

namespace muc {

/// @brief C++ named requirements: CopyAssignable. This is more strict than
/// std::is_copy_assignable because it does check the type of the result of the
/// assignment (which, for a CopyAssignable type, must be an lvalue of type T).
/// See also: https://en.cppreference.com/w/cpp/named_req/copy_assignable
template<typename T>
concept copy_assignable =
    move_assignable<T> and std::assignable_from<T&, T&> and
    std::assignable_from<T&, const T&> and std::assignable_from<T&, const T&&>;

template<typename T>
concept noexcept_copy_assignable =
    noexcept_move_assignable<T> and std::assignable_from<T&, T&> and
    std::assignable_from<T&, const T&> and
    std::assignable_from<T&, const T&&> and
    std::is_nothrow_move_assignable_v<T>;

} // namespace muc
