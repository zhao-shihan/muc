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

#include "muc/detail/c++17/type_traits/remove_cvref.h++"

#include <type_traits>

namespace muc {

namespace impl {
namespace {

template<typename T, std::size_t = sizeof(T)>
auto is_complete(T*) -> std::true_type;

template<typename T, std::enable_if_t<std::is_function_v<T>, bool> = true>
auto is_complete(T) -> std::true_type;

template<typename T>
auto is_complete(...) -> std::false_type;

} // namespace
} // namespace impl

template<typename T>
struct is_complete :
    decltype(impl::is_complete<muc::remove_cvref_t<T>>(nullptr)) {};

template<typename T>
inline constexpr bool is_complete_v{is_complete<T>::value};

} // namespace muc

#ifdef MUC_STATIC_TEST

namespace muc {
namespace {

class is_complete_test_complete_type {};

class is_complete_test_incomplete_type;

auto is_complete_test_function() -> void {}

} // namespace
} // namespace muc

static_assert(muc::is_complete_v<void*>);
static_assert(muc::is_complete_v<int[4]>);
static_assert(muc::is_complete_v<double>);
static_assert(muc::is_complete_v<muc::is_complete_test_complete_type>);
static_assert(muc::is_complete_v<decltype(muc::is_complete_test_function)>);
static_assert(not muc::is_complete_v<void>);
static_assert(not muc::is_complete_v<int[]>);
static_assert(not muc::is_complete_v<muc::is_complete_test_incomplete_type>);

#endif
