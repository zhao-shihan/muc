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

#include <type_traits>
#include <utility>

namespace muc::impl {

template<typename, typename, typename = void>
struct has_binary_arithmetic_with : std::false_type {};

template<typename T, typename U>
struct has_binary_arithmetic_with<
    T, U,
    std::void_t<
        // T +/- T
        decltype(std::declval<const T&>() + std::declval<const T&>()),
        decltype(std::declval<const T&>() - std::declval<const T&>()),
        // k * T
        /* decltype(+std::declval<const T&>()), */
        decltype(-std::declval<const T&>()),
        /* decltype(1 * std::declval<const T&>()),
        decltype(std::declval<const T&>() * 1),
        decltype(std::declval<const T&>() / 1), */
        // U +/- U
        decltype(std::declval<const U&>() + std::declval<const U&>()),
        decltype(std::declval<const U&>() - std::declval<const U&>()),
        // k * U
        /* decltype(+std::declval<const U&>()), */
        decltype(-std::declval<const U&>()),
        /* decltype(1 * std::declval<const U&>()),
        decltype(std::declval<const U&>() * 1),
        decltype(std::declval<const U&>() / 1), */
        // T +/- U
        decltype(std::declval<const T&>() + std::declval<const U&>()),
        decltype(std::declval<const T&>() - std::declval<const U&>()),
        // U +/- T
        decltype(std::declval<const U&>() + std::declval<const T&>()),
        decltype(std::declval<const U&>() - std::declval<const T&>()),
        // T x= X
        decltype(std::declval<T&>() += std::declval<const U&>()),
        decltype(std::declval<T&>() -= std::declval<const U&>())//,
        /* decltype(std::declval<T&>() *= 1), decltype(std::declval<T&>() /= 1) */>> :
    std::true_type {};

template<typename T, typename U>
inline constexpr bool has_binary_arithmetic_with_v{
    has_binary_arithmetic_with<T, U>::value};

} // namespace muc::impl
