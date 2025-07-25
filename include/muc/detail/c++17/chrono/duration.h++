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

#include <chrono>
#include <climits>
#include <type_traits>

namespace muc::chrono {

template<typename T = std::chrono::nanoseconds::rep,
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 64, bool> = true>
using nanoseconds = std::chrono::duration<T, std::nano>;

template<typename T = std::chrono::microseconds::rep,
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 55, bool> = true>
using microseconds = std::chrono::duration<T, std::micro>;

template<typename T = std::chrono::milliseconds::rep,
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 45, bool> = true>
using milliseconds = std::chrono::duration<T, std::milli>;

template<typename T = std::chrono::seconds::rep,
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 35, bool> = true>
using seconds = std::chrono::duration<T, std::ratio<1>>;

template<typename T = std::chrono::minutes::rep,
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 29, bool> = true>
using minutes = std::chrono::duration<T, std::ratio<60>>;

template<typename T = std::chrono::hours::rep,
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 23, bool> = true>
using hours = std::chrono::duration<T, std::ratio<3600>>;

#if __cplusplus >= 202002L
template<typename T = std::chrono::days::rep,
#else // C++17 workaround
template<typename T = std::chrono::hours::rep,
#endif
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 25, bool> = true>
using days = std::chrono::duration<T, std::ratio<86400>>;

#if __cplusplus >= 202002L
template<typename T = std::chrono::weeks::rep,
#else // C++17 workaround
template<typename T = std::chrono::hours::rep,
#endif
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 22, bool> = true>
using weeks = std::chrono::duration<T, std::ratio<604800>>;

#if __cplusplus >= 202002L
template<typename T = std::chrono::months::rep,
#else // C++17 workaround
template<typename T = std::chrono::hours::rep,
#endif
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 20, bool> = true>
using months = std::chrono::duration<T, std::ratio<2629746>>;

#if __cplusplus >= 202002L
template<typename T = std::chrono::years::rep,
#else // C++17 workaround
template<typename T = std::chrono::hours::rep,
#endif
         std::enable_if_t<std::is_signed_v<T>, bool> = true,
         std::enable_if_t<sizeof(T) * CHAR_BIT >= 17, bool> = true>
using years = std::chrono::duration<T, std::ratio<31556952>>;

} // namespace muc::chrono
