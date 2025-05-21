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

#include "muc/detail/c++20/iterator/projected_value_t.h++"

#include <algorithm>
#include <iterator>
#include <ranges>
#include <utility>

namespace muc {

namespace impl {

struct contains_fn {
    template<std::input_iterator I, std::sentinel_for<I> S,
             typename Proj = std::identity,
             typename T = muc::projected_value_t<I, Proj>>
        requires std::indirect_binary_predicate<
            std::ranges::equal_to, std::projected<I, Proj>, const T*>
    constexpr auto operator()(I first, S last, const T& value,
                              Proj proj = {}) const -> bool {
        return std::ranges::find(std::move(first), last, value, proj) != last;
    }

    template<
        std::ranges::input_range R, typename Proj = std::identity,
        typename T = muc::projected_value_t<std::ranges::iterator_t<R>, Proj>>
        requires std::indirect_binary_predicate<
            std::ranges::equal_to,
            std::projected<std::ranges::iterator_t<R>, Proj>, const T*>
    constexpr auto operator()(R&& r, const T& value, Proj proj = {}) const
        -> bool {
        return std::ranges::find(std::move(std::ranges::begin(r)),
                                 std::ranges::end(r), value,
                                 proj) != std::ranges::end(r);
    }
};

} // namespace impl

inline constexpr impl::contains_fn contains{};

} // namespace muc
