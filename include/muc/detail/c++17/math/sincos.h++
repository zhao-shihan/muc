// -*- C++ -*-
//
// Copyright (C) 2021-2025  Shihan Zhao
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

#include <cmath>
#include <type_traits>
#include <utility>

#ifdef __GLIBC__
// For glibc, sincos is declared in math.h when _GNU_SOURCE is defined
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <math.h>
#undef _GNU_SOURCE
#else
#include <math.h>
#endif
#endif

namespace muc {

/// @brief Compute sine and cosine of a given angle
/// @tparam T The floating-point type
/// @param x The angle in radians
/// @return A pair containing sine and cosine of the angle
/// @note This function uses builtin or platform sincos if available. The
/// performance may be better than calling std::sin and std::cos separately
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
auto sincos(T x) -> std::pair<T, T> {
#if defined __APPLE__ and defined __MACH__
    [[maybe_unused]] constexpr auto has_system_sincos{true};
    [[maybe_unused]] constexpr auto on_mac_os{true};
#elif defined __GLIBC__ or defined __FreeBSD__ or defined __NetBSD__ or    \
    defined __OpenBSD__ or defined __DragonFly__ or defined __ANDROID__ or \
    defined __CYGWIN__
    [[maybe_unused]] constexpr auto has_system_sincos{true};
    [[maybe_unused]] constexpr auto on_mac_os{false};
#else
    [[maybe_unused]] constexpr auto has_system_sincos{false};
    [[maybe_unused]] constexpr auto on_mac_os{false};
#endif

    if constexpr (std::is_same_v<T, double>) {
#if defined __has_builtin and __has_builtin(__builtin_sincos)
        std::pair<T, T> r;
        __builtin_sincos(x, &r.first, &r.second);
        return r;
#else
        if constexpr (has_system_sincos) {
            std::pair<T, T> r;
            if constexpr (on_mac_os) {
                ::__sincos(x, &r.first, &r.second);
            } else {
                ::sincos(x, &r.first, &r.second);
            }
            return r;
        } else {
#if defined __has_builtin and __has_builtin(__builtin_sin) and \
    __has_builtin(__builtin_cos)
            return {__builtin_sin(x), __builtin_cos(x)};
#else
            return {std::sin(x), std::cos(x)};
#endif
        }
#endif
    } else if constexpr (std::is_same_v<T, float>) {
#if defined __has_builtin and __has_builtin(__builtin_sincosf)
        std::pair<T, T> r;
        __builtin_sincosf(x, &r.first, &r.second);
        return r;
#else
        if constexpr (has_system_sincos) {
            std::pair<T, T> r;
            if constexpr (on_mac_os) {
                ::__sincosf(x, &r.first, &r.second);
            } else {
                ::sincosf(x, &r.first, &r.second);
            }
            return r;
        } else {
#if defined __has_builtin and __has_builtin(__builtin_sinf) and \
    __has_builtin(__builtin_cosf)
            return {__builtin_sinf(x), __builtin_cosf(x)};
#else
            return {std::sin(x), std::cos(x)};
#endif
        }
#endif
    } else if constexpr (std::is_same_v<T, long double>) {
#if defined __has_builtin and __has_builtin(__builtin_sincosl)
        std::pair<T, T> r;
        __builtin_sincosl(x, &r.first, &r.second);
        return r;
#else
        if constexpr (has_system_sincos) {
            if constexpr (on_mac_os) { // no ::__sincosl on macOS
#if defined __has_builtin and __has_builtin(__builtin_sinl) and \
    __has_builtin(__builtin_cosl)
                return {__builtin_sinl(x), __builtin_cosl(x)};
#else
                return {std::sin(x), std::cos(x)};
#endif
            } else {
                std::pair<T, T> r;
                ::sincosl(x, &r.first, &r.second);
                return r;
            }
        } else {
#if defined __has_builtin and __has_builtin(__builtin_sinl) and \
    __has_builtin(__builtin_cosl)
            return {__builtin_sinl(x), __builtin_cosl(x)};
#else
            return {std::sin(x), std::cos(x)};
#endif
        }
#endif
    } else {
        return {std::sin(x), std::cos(x)};
    }
}

/// @brief Compute sine and cosine of a float angle
/// @param x The angle in radians
/// @return A pair containing sine and cosine of the angle
/// @note This function uses builtin or platform sincos if available. The
/// performance may be better than calling std::sin and std::cos separately
inline auto sincosf(float x) -> std::pair<float, float> {
    return muc::sincos(x);
}

/// @brief Compute sine and cosine of a long double angle
/// @param x The angle in radians
/// @return A pair containing sine and cosine of the angle
/// @note This function uses builtin or platform sincos if available. The
/// performance may be better than calling std::sin and std::cos separately
inline auto sincosl(long double x) -> std::pair<long double, long double> {
    return muc::sincos(x);
}

/// @brief Compute sine and cosine of an integral angle
/// @tparam T The floating-point type
/// @tparam U The integral type
/// @param x The angle in radians
/// @return A pair containing sine and cosine of the angle
/// @note This function uses builtin or platform sincos if available. The
/// performance may be better than calling std::sin and std::cos separately
template<typename T = double, typename U,
         std::enable_if_t<std::is_floating_point_v<T> and std::is_integral_v<U>,
                          bool> = true>
auto sincos(U x) -> std::pair<T, T> {
    return muc::sincos(static_cast<T>(x));
}

} // namespace muc
