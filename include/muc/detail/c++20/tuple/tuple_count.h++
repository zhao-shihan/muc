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

#include "muc/detail/c++20/tuple/to_std_tuple.h++"
#include "muc/detail/c++20/tuple/tuple_like.h++"

#include <concepts>
#include <tuple>
#include <type_traits>

namespace muc {

namespace impl {

template<tuple_like, typename>
struct std_tuple_count;

template<typename... Ts, typename T>
struct std_tuple_count<std::tuple<Ts...>, T> :
    std::integral_constant<std::size_t,
                           (0 + ... +
                            static_cast<std::size_t>(std::same_as<T, Ts>))> {};

} // namespace impl

template<tuple_like T, typename U>
struct tuple_count : impl::std_tuple_count<to_std_tuple_t<T>, U> {};

template<tuple_like T, typename U>
inline constexpr auto tuple_count_v{tuple_count<T, U>::value};

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::tuple_count_v<std::tuple<int, double>, int> == 1);
static_assert(muc::tuple_count_v<std::tuple<int, int>, int> == 2);
static_assert(muc::tuple_count_v<std::tuple<int, double>, float> == 0);
static_assert(muc::tuple_count_v<std::tuple<>, float> == 0);

#endif
