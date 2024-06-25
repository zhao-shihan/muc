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

#include <type_traits>

namespace muc::impl {

template<typename T>
struct is_really_unsigned_integral :
    std::bool_constant<std::is_unsigned_v<T> and not std::is_same_v<T, bool> and
                       not std::is_same_v<T, char> and
#if __cplusplus >= 202002L
                       not std::is_same_v<T, char8_t> and
#endif
                       not std::is_same_v<T, char16_t> and
                       not std::is_same_v<T, char32_t> and
                       not std::is_same_v<T, wchar_t>> {
};

template<typename T>
inline constexpr auto is_really_unsigned_integral_v{
    is_really_unsigned_integral<T>::value};

} // namespace muc::impl
