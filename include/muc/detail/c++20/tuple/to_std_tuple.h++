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

#include "muc/detail/c++20/tuple/tuple_like.h++"

#include <tuple>

namespace muc {

namespace impl {

template<tuple_like, std::size_t, std::size_t, tuple_like>
struct to_std_tuple_helper;

template<tuple_like T, std::size_t I, std::size_t N, typename... Ts>
struct to_std_tuple_helper<T, I, N, std::tuple<Ts...>> :
    to_std_tuple_helper<T, I + 1, N,
                        std::tuple<Ts..., std::tuple_element_t<I, T>>> {};

template<tuple_like T, std::size_t N, typename... Ts>
struct to_std_tuple_helper<T, N, N, std::tuple<Ts...>> {
    using type = std::tuple<Ts...>;
};

} // namespace impl

template<tuple_like T>
struct to_std_tuple :
    impl::to_std_tuple_helper<T, 0, std::tuple_size_v<T>, std::tuple<>> {};

template<tuple_like T>
using to_std_tuple_t = typename to_std_tuple<T>::type;

} // namespace muc

#ifdef MUC_STATIC_TEST

#include <concepts>

static_assert(
    std::same_as<muc::to_std_tuple_t<std::tuple<int>>, std::tuple<int>>);
static_assert(
    std::same_as<muc::to_std_tuple_t<std::tuple<int>>, std::tuple<int>>);
static_assert(
    std::same_as<muc::to_std_tuple_t<std::pair<int, int>>, std::tuple<int, int>>);
static_assert(std::same_as<muc::to_std_tuple_t<std::pair<int, double>>,
                           std::tuple<int, double>>);
static_assert(std::same_as<muc::to_std_tuple_t<std::tuple<int, double>>,
                           std::tuple<int, double>>);
static_assert(std::same_as<muc::to_std_tuple_t<std::tuple<>>, std::tuple<>>);

#endif
