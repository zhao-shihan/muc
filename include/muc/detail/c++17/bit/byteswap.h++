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

#include "muc/detail/c++17/bit/bit_cast.h++"

#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>

namespace muc {

/// @brief Reverses the bytes in the given integer value.
/// @tparam T value type
/// @param value integer value
/// @return An integer value of type T whose object representation comprises the
/// bytes of that of value in reversed order.
template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
auto byteswap(T value) noexcept -> T {
    static_assert(std::has_unique_object_representations_v<T>,
                  "T may not have padding bits");
    auto bit{muc::bit_cast<std::array<std::byte, sizeof(T)>>(value)};
    std::reverse(bit.begin(), bit.end());
    return muc::bit_cast<T>(bit);
}

} // namespace muc
