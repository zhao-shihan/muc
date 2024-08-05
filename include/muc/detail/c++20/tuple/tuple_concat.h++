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

#include <tuple>

namespace muc {

namespace impl {

template<tuple_like...>
struct std_tuple_concat;

template<>
struct std_tuple_concat<> {
    using type = std::tuple<>;
};

template<typename... Ts>
struct std_tuple_concat<std::tuple<Ts...>> {
    using type = std::tuple<Ts...>;
};

template<typename... Ts, typename... Us, typename... Vs>
struct std_tuple_concat<std::tuple<Ts...>, std::tuple<Us...>, Vs...> :
    std_tuple_concat<std::tuple<Ts..., Us...>, Vs...> {};

template<tuple_like... Ts>
using std_tuple_concat_t = typename std_tuple_concat<Ts...>::type;

} // namespace impl

template<tuple_like... Ts>
struct tuple_concat : impl::std_tuple_concat<to_std_tuple_t<Ts>...> {};

template<tuple_like... Ts>
using tuple_concat_t = typename tuple_concat<Ts...>::type;

} // namespace muc

#ifdef MUC_STATIC_TEST

#include <concepts>

static_assert(std::same_as<muc::tuple_concat_t<std::tuple<double, int>,
                                               std::tuple<float, short>>,
                           std::tuple<double, int, float, short>>);
static_assert(std::same_as<muc::tuple_concat_t<std::tuple<double, int>,
                                               std::pair<float, short>>,
                           std::tuple<double, int, float, short>>);
static_assert(std::same_as<muc::tuple_concat_t<std::pair<double, int>,
                                               std::tuple<float, short>>,
                           std::tuple<double, int, float, short>>);
static_assert(std::same_as<muc::tuple_concat_t<std::pair<double, int>,
                                               std::pair<float, short>>,
                           std::tuple<double, int, float, short>>);
static_assert(
    std::same_as<muc::tuple_concat_t<std::tuple<double, int>, std::tuple<>>,
                 std::tuple<double, int>>);
static_assert(
    std::same_as<muc::tuple_concat_t<std::tuple<>, std::tuple<double, int>>,
                 std::tuple<double, int>>);
static_assert(std::same_as<muc::tuple_concat_t<std::tuple<int>, std::tuple<>>,
                           std::tuple<int>>);
static_assert(std::same_as<muc::tuple_concat_t<std::tuple<>, std::tuple<int>>,
                           std::tuple<int>>);
static_assert(std::same_as<muc::tuple_concat_t<std::tuple<>, std::tuple<>>,
                           std::tuple<>>);

#endif
