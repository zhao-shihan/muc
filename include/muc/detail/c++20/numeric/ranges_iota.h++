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

#include <ranges>
#include <utility>
#include <version>

#ifdef __cpp_lib_ranges_iota // C++23
#include <algorithm>
#include <numeric>
#else // backport
#include <iterator>
#endif

namespace muc::ranges {

#ifdef __cpp_lib_ranges_iota // C++23

struct iota_fn {
    template<std::input_or_output_iterator O, std::sentinel_for<O> S,
             std::weakly_incrementable T>
        requires std::indirectly_writable<O, const T&>
    constexpr auto operator()(O first, S last,
                              T value) const -> std::ranges::iota_result<O, T> {
        return std::ranges::iota(std::move(first), std::move(last),
                                 std::move(value));
    }

    template<std::weakly_incrementable T, std::ranges::output_range<const T&> R>
    constexpr auto operator()(R&& r, T value) const
        -> std::ranges::iota_result<std::ranges::borrowed_iterator_t<R>, T> {
        return std::ranges::iota(std::forward<decltype(r)>(r),
                                 std::move(value));
    }
};

#else // backport

template<typename O, typename T>
struct out_value_result {
    [[no_unique_address]] O out;
    [[no_unique_address]] T value;

    template<typename O2, typename T2>
        requires std::convertible_to<const O&, O2> and
                 std::convertible_to<const T&, T2>
    constexpr operator out_value_result<O2, T2>() const& {
        return {out, value};
    }

    template<typename O2, typename T2>
        requires std::convertible_to<O, O2> and std::convertible_to<T, T2>
    constexpr operator out_value_result<O2, T2>() && {
        return {std::move(out), std::move(value)};
    }
};

template<typename O, typename T>
using iota_result = out_value_result<O, T>;

struct iota_fn {
    template<std::input_or_output_iterator O, std::sentinel_for<O> S,
             std::weakly_incrementable T>
        requires std::indirectly_writable<O, const T&>
    constexpr auto operator()(O first, S last,
                              T value) const -> iota_result<O, T> {
        while (first != last) {
            *first++ = value++;
        }
        return {std::move(first), std::move(value)};
    }

    template<std::weakly_incrementable T, std::ranges::output_range<const T&> R>
    constexpr auto operator()(R&& r, T value) const
        -> iota_result<std::ranges::borrowed_iterator_t<R>, T> {
        return (*this)(std::ranges::begin(std::forward<decltype(r)>(r)),
                       std::ranges::end(std::forward<decltype(r)>(r)),
                       std::move(value));
    }
};

#endif

inline constexpr iota_fn iota{};

} // namespace muc::ranges
