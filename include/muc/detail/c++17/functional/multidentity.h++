// -*- C++ -*-
//
// Copyright 2021-2025  Shihan Zhao
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

#include <tuple>
#include <type_traits>
#include <utility>

namespace muc {

/// @brief muc::multidentity is a function object type whose operator() returns
/// its arguments in a `std::tuple`. The effect is the same as
/// `std::forward_as_tuple`.
/// @warning Passing temporary objects as arguments is errorous and will produce
/// `std::tuple` holding dangling references.
struct multidentity {
    template<typename... Ts, std::enable_if_t<sizeof...(Ts) >= 1, bool> = true>
    [[nodiscard]] constexpr auto
    operator()(Ts&&... ts) const noexcept -> std::tuple<Ts&&...> {
        return std::forward_as_tuple<Ts...>(ts...);
    }
};

} // namespace muc
