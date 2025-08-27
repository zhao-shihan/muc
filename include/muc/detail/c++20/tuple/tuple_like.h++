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

#include <concepts>
#include <type_traits>
#include <utility>

namespace muc {

namespace impl {

template<typename T, std::size_t I>
concept has_tuple_element_and_get = requires(T t) {
    typename std::tuple_element_t<I, T>;
    { get<I>(t) } -> std::convertible_to<const std::tuple_element_t<I, T>&>;
};

} // namespace impl

template<typename T>
concept tuple_like = requires {
    requires not std::is_reference_v<T>;
    typename std::tuple_size<T>::type;
    requires std::derived_from<
        std::tuple_size<T>,
        std::integral_constant<std::size_t, std::tuple_size_v<T>>>;
    requires([]<std::size_t... Is>(std::index_sequence<Is...>) {
        return (... and impl::has_tuple_element_and_get<T, Is>);
    }(std::make_index_sequence<std::tuple_size_v<T>>()));
};

} // namespace muc
