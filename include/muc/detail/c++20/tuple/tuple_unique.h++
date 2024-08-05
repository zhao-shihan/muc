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
#include "muc/detail/c++20/tuple/tuple_contains.h++"
#include "muc/detail/c++20/tuple/tuple_like.h++"

#include <tuple>
#include <type_traits>

namespace muc {

namespace impl {

template<typename, typename>
struct tuple_unique;

template<typename... Us>
struct tuple_unique<std::tuple<>, std::tuple<Us...>> {
    using type = std::tuple<Us...>;
};

template<typename T, typename... Ts, typename... Us>
struct tuple_unique<std::tuple<T, Ts...>, std::tuple<Us...>> {
    using type = std::conditional_t<
        tuple_contains_v<std::tuple<Us...>, T>,
        typename tuple_unique<std::tuple<Ts...>, std::tuple<Us...>>::type,
        typename tuple_unique<std::tuple<Ts...>, std::tuple<Us..., T>>::type>;
};

} // namespace impl

template<tuple_like T>
struct tuple_unique {
    using type =
        typename impl::tuple_unique<to_std_tuple_t<T>, std::tuple<>>::type;
};

template<tuple_like T>
using tuple_unique_t = typename tuple_unique<T>::type;

} // namespace muc

#ifdef MUC_STATIC_TEST

#include <concepts>

static_assert(std::same_as<muc::tuple_unique_t<std::tuple<>>, std::tuple<>>);
static_assert(
    std::same_as<muc::tuple_unique_t<std::tuple<int>>, std::tuple<int>>);
static_assert(
    std::same_as<muc::tuple_unique_t<std::tuple<int, int>>, std::tuple<int>>);
static_assert(std::same_as<muc::tuple_unique_t<std::tuple<int, int, int>>,
                           std::tuple<int>>);
static_assert(
    std::same_as<muc::tuple_unique_t<std::tuple<int, double, short, short>>,
                 std::tuple<int, double, short>>);
static_assert(std::same_as<
              muc::tuple_unique_t<std::tuple<short, int, short, long, short>>,
              std::tuple<short, int, long>>);

#endif
