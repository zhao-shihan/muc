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

#include "muc/detail/c++20/iterator/projected_value_t.h++"

#include <algorithm>
#include <iterator>
#include <ranges>
#include <utility>

namespace muc::ranges {

inline constexpr struct {
    template<std::input_iterator I, std::sentinel_for<I> S,
             typename Proj = std::identity,
             typename T = muc::projected_value_t<I, Proj>>
        requires std::indirect_binary_predicate<
            std::ranges::equal_to, std::projected<I, Proj>, const T*>
    constexpr auto operator()(I first, S last, const T& value,
                              Proj proj = {}) const -> bool {
        return std::ranges::find(std::move(first), last, value,
                                 std::move(proj)) != last;
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
                                 std::move(proj)) != std::ranges::end(r);
    }
} contains{};

inline constexpr struct {
    template<std::forward_iterator I1, std::sentinel_for<I1> S1,
             std::forward_iterator I2, std::sentinel_for<I2> S2,
             typename Pred = std::ranges::equal_to,
             typename Proj1 = std::identity, typename Proj2 = std::identity>
        requires std::indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
    constexpr auto operator()(I1 first1, S1 last1, I2 first2, S2 last2,
                              Pred pred = {}, Proj1 proj1 = {},
                              Proj2 proj2 = {}) const -> bool {
        return (first2 == last2) or
               not std::ranges::search(first1, last1, first2, last2,
                                       std::move(pred), std::move(proj1),
                                       std::move(proj2))
                       .empty();
    }

    template<std::ranges::forward_range R1, std::ranges::forward_range R2,
             typename Pred = std::ranges::equal_to,
             typename Proj1 = std::identity, typename Proj2 = std::identity>
        requires std::indirectly_comparable<std::ranges::iterator_t<R1>,
                                            std::ranges::iterator_t<R2>, Pred,
                                            Proj1, Proj2>
    constexpr auto operator()(R1&& r1, R2&& r2, Pred pred = {},
                              Proj1 proj1 = {}, Proj2 proj2 = {}) const
        -> bool {
        return std::ranges::empty(r2) or
               not std::ranges::search(
                       std::ranges::begin(r1), std::ranges::end(r1),
                       std::ranges::begin(r2), std::ranges::end(r2),
                       std::move(pred), std::move(proj1), std::move(proj2))
                       .empty();
    }
} contains_subrange{};

} // namespace muc::ranges
