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

namespace muc {

/// @brief Get n-based digits of an integer.
/// @param m The integer.
/// @param base The base.
/// @param with_sign Set whether the digits in return includes "-" sign (true
/// means include, false means exclude).
/// @return The n-based digits.
template<typename T,
         std::enable_if_t<std::is_signed_v<T> and std::is_integral_v<T>, bool> =
             true>
constexpr auto digits_of(T m, int base, bool with_sign = false) -> int {
    if (base == 0) {
        return 0;
    }
    if (base < 0) {
        base = -base;
    }
    int d{};
    if (m < 0) {
        if (with_sign) {
            ++d;
        }
        m = -m;
    }
    do {
        m /= base;
        ++d;
    } while (m > 0);
    return d;
}

/// @brief Get n-based digits of an integer.
/// @tparam Base The base.
/// @param m The integer.
/// @param with_sign Set whether the digits in return includes "-" sign (true
/// means include, false means exclude).
/// @return The n-based digits.
template<unsigned Base, typename T,
         std::enable_if_t<std::is_signed_v<T> and std::is_integral_v<T>, bool> =
             true>
constexpr auto digits_of(T m, bool with_sign = false) -> int {
    return muc::digits_of(m, Base, with_sign);
}

/// @brief Get n-based digits of an integer.
/// @param m The integer.
/// @param base The base.
/// @return The n-based digits.
template<typename T,
         std::enable_if_t<std::is_unsigned_v<T> and std::is_integral_v<T>,
                          bool> = true>
constexpr auto digits_of(T m, int base) -> int {
    if (base == 0) {
        return 0;
    }
    if (base < 0) {
        base = -base;
    }
    int d{};
    do {
        m /= base;
        ++d;
    } while (m > 0);
    return d;
}

/// @brief Get n-based digits of an integer.
/// @tparam Base The base.
/// @param m The integer.
/// @return The n-based digits.
template<unsigned Base, typename T,
         std::enable_if_t<std::is_unsigned_v<T> and std::is_integral_v<T>,
                          bool> = true>
constexpr auto digits_of(T m) -> auto {
    return muc::digits_of(m, Base);
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::digits_of(0, 2) == 1);
static_assert(muc::digits_of(0u, 2) == 1);
static_assert(muc::digits_of(0b10100101000, 2) == 11);
static_assert(muc::digits_of(0b10100101000u, 2) == 11);
static_assert(muc::digits_of(-0b10100101000, 2) == 11);
static_assert(muc::digits_of(-0b10100101000, 2, true) == 12);
static_assert(muc::digits_of<2>(0) == 1);
static_assert(muc::digits_of<2>(0u) == 1);
static_assert(muc::digits_of<2>(0b10100101000) == 11);
static_assert(muc::digits_of<2>(0b10100101000u) == 11);
static_assert(muc::digits_of<2>(-0b10100101000) == 11);
static_assert(muc::digits_of<2>(-0b10100101000, true) == 12);

static_assert(muc::digits_of(0, 8) == 1);
static_assert(muc::digits_of(0u, 8) == 1);
static_assert(muc::digits_of(0123456, 8) == 6);
static_assert(muc::digits_of(0123456u, 8) == 6);
static_assert(muc::digits_of(-0123456, 8) == 6);
static_assert(muc::digits_of(-0123456, 8, true) == 7);
static_assert(muc::digits_of<8>(0) == 1);
static_assert(muc::digits_of<8>(0u) == 1);
static_assert(muc::digits_of<8>(0123456) == 6);
static_assert(muc::digits_of<8>(0123456u) == 6);
static_assert(muc::digits_of<8>(-0123456) == 6);
static_assert(muc::digits_of<8>(-0123456, true) == 7);

static_assert(muc::digits_of(0, 10) == 1);
static_assert(muc::digits_of(0u, 10) == 1);
static_assert(muc::digits_of(1, 10) == 1);
static_assert(muc::digits_of(1u, 10) == 1);
static_assert(muc::digits_of(99, 10) == 2);
static_assert(muc::digits_of(99u, 10) == 2);
static_assert(muc::digits_of(100, 10) == 3);
static_assert(muc::digits_of(100u, 10) == 3);
static_assert(muc::digits_of(123456, 10) == 6);
static_assert(muc::digits_of(123456u, 10) == 6);
static_assert(muc::digits_of(1919810, 10) == 7);
static_assert(muc::digits_of(1919810u, 10) == 7);
static_assert(muc::digits_of(-123456, 10) == 6);
static_assert(muc::digits_of(-123456, 10, true) == 7);
static_assert(muc::digits_of<10>(123456) == 6);
static_assert(muc::digits_of<10>(123456u) == 6);
static_assert(muc::digits_of<10>(-123456) == 6);
static_assert(muc::digits_of<10>(-123456, true) == 7);

static_assert(muc::digits_of(0, 16) == 1);
static_assert(muc::digits_of(0u, 16) == 1);
static_assert(muc::digits_of(0x123456, 16) == 6);
static_assert(muc::digits_of(0x123456u, 16) == 6);
static_assert(muc::digits_of(-0x123456, 16) == 6);
static_assert(muc::digits_of(-0x123456, 16, true) == 7);
static_assert(muc::digits_of<16>(0) == 1);
static_assert(muc::digits_of<16>(0u) == 1);
static_assert(muc::digits_of<16>(0x123456) == 6);
static_assert(muc::digits_of<16>(0x123456u) == 6);
static_assert(muc::digits_of<16>(-0x123456) == 6);
static_assert(muc::digits_of<16>(-0x123456, true) == 7);

#endif
