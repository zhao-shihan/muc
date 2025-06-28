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
#ifdef __cpp_lib_to_underlying // >= C++23
#define MUC_CPP_LIB_TO_UNDERLYING
#endif
#endif

#ifdef MUC_CPP_LIB_TO_UNDERLYING
#include <utility>
#else // backport
#include <type_traits>
#endif

namespace muc {

template<typename E>
constexpr auto to_underlying(E value) noexcept -> std::underlying_type_t<E> {
#ifdef MUC_CPP_LIB_TO_UNDERLYING
    return std::to_underlying<E>(value);
#else // backport
    return static_cast<typename std::underlying_type_t<E>>(value);
#endif
}

} // namespace muc

#undef MUC_CPP_LIB_TO_UNDERLYING
