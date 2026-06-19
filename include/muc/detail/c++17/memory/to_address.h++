// -*- C++ -*-
//
// Copyright (C) 2021-2026  Shihan Zhao
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

#include <memory>
#include <type_traits>
#include <utility>

namespace muc {

/// @brief A backport of std::to_address. Obtains the raw pointer represented
/// by a pointer or pointer-like object.
///
/// This overload handles raw pointers directly by returning the pointer as-is.
///
/// @tparam T The pointed-to type (must not be a function type)
/// @param ptr A raw pointer
/// @return The raw pointer unchanged
template<typename T>
constexpr auto to_address(T* ptr) noexcept -> T* {
    static_assert(not std::is_function_v<T>);
    return ptr;
}

namespace impl {

/// @cond INTERNAL
template<typename, typename = void>
struct has_pointer_traits_to_address : std::false_type {};

template<typename T>
struct has_pointer_traits_to_address<
    T, std::void_t<decltype(std::pointer_traits<T>::to_address(
           std::declval<const T&>()))>> : std::true_type {};
/// @endcond

} // namespace impl

/// @brief Obtains the raw pointer from a fancy pointer or pointer-like object.
///
/// This overload handles fancy pointers (such as
/// std::allocator_traits::pointer types). It first attempts to use
/// std::pointer_traits<T>::to_address if available, falling back to
/// invoking operator->() and then recursing through the raw pointer
/// overload.
///
/// @tparam T The fancy pointer or pointer-like type
/// @param ptr The fancy pointer or pointer-like object
/// @return The raw pointer represented by ptr
template<typename T>
constexpr auto to_address(const T& ptr) noexcept -> auto {
    if constexpr (impl::has_pointer_traits_to_address<T>::value) {
        return std::pointer_traits<T>::to_address(ptr);
    } else {
        return muc::to_address(ptr.operator->());
    }
}

} // namespace muc
