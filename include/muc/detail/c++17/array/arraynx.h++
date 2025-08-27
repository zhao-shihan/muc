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

#include <array>
#include <complex>
#include <cstdint>

namespace muc {

template<typename T>
using array2 = std::array<T, 2>;
template<typename T>
using array3 = std::array<T, 3>;
template<typename T>
using array4 = std::array<T, 4>;

template<std::size_t N>
using arrayi = std::array<int, N>;
template<std::size_t N>
using arrayl = std::array<long, N>;
template<std::size_t N>
using arrayll = std::array<long long, N>;
template<std::size_t N>
using arrayu = std::array<unsigned, N>;
template<std::size_t N>
using arrayul = std::array<unsigned long, N>;
template<std::size_t N>
using arrayull = std::array<unsigned long long, N>;
template<std::size_t N>
using arrayi8 = std::array<std::int8_t, N>;
template<std::size_t N>
using arrayi16 = std::array<std::int16_t, N>;
template<std::size_t N>
using arrayi32 = std::array<std::int32_t, N>;
template<std::size_t N>
using arrayi64 = std::array<std::int64_t, N>;
template<std::size_t N>
using arrayu8 = std::array<std::uint8_t, N>;
template<std::size_t N>
using arrayu16 = std::array<std::uint16_t, N>;
template<std::size_t N>
using arrayu32 = std::array<std::uint32_t, N>;
template<std::size_t N>
using arrayu64 = std::array<std::uint64_t, N>;
template<std::size_t N>
using arrayf = std::array<float, N>;
template<std::size_t N>
using arrayd = std::array<double, N>;
template<std::size_t N>
using arrayld = std::array<long double, N>;
template<std::size_t N>
using arraycf = std::array<std::complex<float>, N>;
template<std::size_t N>
using arraycd = std::array<std::complex<double>, N>;
template<std::size_t N>
using arraycld = std::array<std::complex<long double>, N>;

using array2i = arrayi<2>;
using array2l = arrayl<2>;
using array2ll = arrayll<2>;
using array2u = arrayu<2>;
using array2ul = arrayul<2>;
using array2ull = arrayull<2>;
using array2i8 = arrayi8<2>;
using array2i16 = arrayi16<2>;
using array2i32 = arrayi32<2>;
using array2i64 = arrayi64<2>;
using array2u8 = arrayu8<2>;
using array2u16 = arrayu16<2>;
using array2u32 = arrayu32<2>;
using array2u64 = arrayu64<2>;
using array2f = arrayf<2>;
using array2d = arrayd<2>;
using array2ld = arrayld<2>;
using array2cf = arraycf<2>;
using array2cd = arraycd<2>;
using array2cld = arraycld<2>;

using array3i = arrayi<3>;
using array3l = arrayl<3>;
using array3ll = arrayll<3>;
using array3u = arrayu<3>;
using array3ul = arrayul<3>;
using array3ull = arrayull<3>;
using array3i8 = arrayi8<3>;
using array3i16 = arrayi16<3>;
using array3i32 = arrayi32<3>;
using array3i64 = arrayi64<3>;
using array3u8 = arrayu8<3>;
using array3u16 = arrayu16<3>;
using array3u32 = arrayu32<3>;
using array3u64 = arrayu64<3>;
using array3f = arrayf<3>;
using array3d = arrayd<3>;
using array3ld = arrayld<3>;
using array3cf = arraycf<3>;
using array3cd = arraycd<3>;
using array3cld = arraycld<3>;

using array4i = arrayi<4>;
using array4l = arrayl<4>;
using array4ll = arrayll<4>;
using array4u = arrayu<4>;
using array4ul = arrayul<4>;
using array4ull = arrayull<4>;
using array4i8 = arrayi8<4>;
using array4i16 = arrayi16<4>;
using array4i32 = arrayi32<4>;
using array4i64 = arrayi64<4>;
using array4u8 = arrayu8<4>;
using array4u16 = arrayu16<4>;
using array4u32 = arrayu32<4>;
using array4u64 = arrayu64<4>;
using array4f = arrayf<4>;
using array4d = arrayd<4>;
using array4ld = arrayld<4>;
using array4cf = arraycf<4>;
using array4cd = arraycd<4>;
using array4cld = arraycld<4>;

} // namespace muc
