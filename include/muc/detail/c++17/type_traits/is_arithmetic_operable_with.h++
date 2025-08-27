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

#include "muc/detail/c++17/type_traits/impl/has_binary_arithmetic_with.h++"
#include "muc/detail/c++17/type_traits/is_general_arithmetic.h++"

#include <type_traits>
#include <utility>

namespace muc {

template<typename T, typename U>
struct is_arithmetic_operable_with :
    std::bool_constant<
        is_general_arithmetic_v<T> and is_general_arithmetic_v<U> and
        // T <-> {U}
        impl::has_binary_arithmetic_with_v<T, U> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const U&>() + std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const U&>() - std::declval<const U&>())> and
        /* impl::has_binary_arithmetic_with_v<
            U, decltype(+std::declval<const U&>())> and */
        impl::has_binary_arithmetic_with_v<
            U, decltype(-std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T, decltype(1 * std::declval<const U&>())> and
        /* impl::has_binary_arithmetic_with_v<
            T, decltype(std::declval<const U&>() * 1)> and
        impl::has_binary_arithmetic_with_v<
            T, decltype(std::declval<const U&>() / 1)> and */
        // T <-> {T <-> U}
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const T&>() + std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const T&>() - std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const U&>() + std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            T,
            decltype(std::declval<const U&>() - std::declval<const T&>())> and
        // U <-> {T}
        impl::has_binary_arithmetic_with_v<U, T> and
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const T&>() + std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const T&>() - std::declval<const T&>())> and
        /* impl::has_binary_arithmetic_with_v<
            U, decltype(+std::declval<const T&>())> and */
        impl::has_binary_arithmetic_with_v<
            U, decltype(-std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(1 * std::declval<const T&>())> and
        /* impl::has_binary_arithmetic_with_v<
            U, decltype(std::declval<const T&>() * 1)> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(std::declval<const T&>() / 1)> and */
        // U <-> {T <-> U}
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const T&>() + std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const T&>() - std::declval<const U&>())> and
        impl::has_binary_arithmetic_with_v<
            U,
            decltype(std::declval<const U&>() + std::declval<const T&>())> and
        impl::has_binary_arithmetic_with_v<
            U, decltype(std::declval<const U&>() - std::declval<const T&>())>> {
};

template<typename T, typename U>
inline constexpr bool is_arithmetic_operable_with_v{
    is_arithmetic_operable_with<T, U>::value};

} // namespace muc
