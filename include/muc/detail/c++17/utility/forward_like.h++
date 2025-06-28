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

#if __cplusplus >= 202002L // >= C++20
#include <version>
#ifdef __cpp_lib_forward_like // >= C++23
#define MUC_CPP_LIB_FORWARD_LIKE
#endif
#endif

#include <utility>

#ifndef MUC_CPP_LIB_FORWARD_LIKE // backport
#include <type_traits>
#endif

namespace muc {

template<typename T, typename U>
[[nodiscard]] constexpr auto forward_like(U&& x) noexcept -> auto&& {
#ifdef MUC_CPP_LIB_FORWARD_LIKE // C++23
    return std::forward_like<T, U>(x);
#else // backport
    constexpr bool is_adding_const =
        std::is_const_v<std::remove_reference_t<T>>;
    if constexpr (std::is_lvalue_reference_v<T&&>) {
        if constexpr (is_adding_const) {
            return std::as_const(x);
        } else {
            return static_cast<U&>(x);
        }
    } else {
        if constexpr (is_adding_const) {
            return std::move(std::as_const(x));
        } else {
            return std::move(x);
        }
    }
#endif
}

} // namespace muc

#undef MUC_CPP_LIB_FORWARD_LIKE
