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

#include <iterator>
#include <numeric>
#include <ranges>
#include <utility>

namespace muc::ranges {

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              auto&&... args) const -> decltype(auto) {
        return std::accumulate(std::ranges::cbegin(r), std::ranges::cend(r),
                               std::forward<decltype(args)>(args)...);
    }
} accumulate{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              auto&&... args) const -> decltype(auto) {
        return std::reduce(std::ranges::cbegin(r), std::ranges::cend(r),
                           std::forward<decltype(args)>(args)...);
    }

    auto operator()(auto&& policy, std::ranges::forward_range auto&& r,
                    auto&&... args) const -> decltype(auto) {
        return std::reduce(std::forward<decltype(policy)>(policy),
                           std::ranges::cbegin(r), std::ranges::cend(r),
                           std::forward<decltype(args)>(args)...);
    }
} reduce{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              std::input_iterator auto i,
                              auto&&... args) const -> decltype(auto) {
        return std::transform_reduce(std::ranges::cbegin(r),
                                     std::ranges::cend(r), i,
                                     std::forward<decltype(args)>(args)...);
    }

    constexpr auto operator()(std::ranges::input_range auto&& r,
                              auto&&... args) const -> decltype(auto) {
        return std::transform_reduce(std::ranges::cbegin(r),
                                     std::ranges::cend(r),
                                     std::forward<decltype(args)>(args)...);
    }

    auto operator()(auto&& policy, std::ranges::forward_range auto&& r,
                    std::forward_iterator auto i,
                    auto&&... args) const -> decltype(auto) {
        return std::transform_reduce(
            std::forward<decltype(policy)>(policy), std::ranges::cbegin(r),
            std::ranges::cend(r), i, std::forward<decltype(args)>(args)...);
    }

    auto operator()(auto&& policy, std::ranges::forward_range auto&& r,
                    auto&&... args) const -> decltype(auto) {
        return std::transform_reduce(
            std::forward<decltype(policy)>(policy), std::ranges::cbegin(r),
            std::ranges::cend(r), std::forward<decltype(args)>(args)...);
    }
} transform_reduce{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              std::input_iterator auto i,
                              auto&&... args) const -> decltype(auto) {
        return std::inner_product(std::ranges::cbegin(r), std::ranges::cend(r),
                                  i, std::forward<decltype(args)>(args)...);
    }
} inner_product{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              /* std::output_iterator */ auto o,
                              auto&&... args) const -> decltype(auto) {
        return std::adjacent_difference(std::ranges::cbegin(r),
                                        std::ranges::cend(r), o,
                                        std::forward<decltype(args)>(args)...);
    }

    auto operator()(auto&& policy, std::ranges::forward_range auto&& r,
                    std::forward_iterator auto o,
                    auto&&... args) const -> decltype(auto) {
        return std::adjacent_difference(
            std::forward<decltype(policy)>(policy), std::ranges::cbegin(r),
            std::ranges::cend(r), o, std::forward<decltype(args)>(args)...);
    }
} adjacent_difference{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              /* std::output_iterator */ auto o,
                              auto&&... args) const -> decltype(auto) {
        return std::partial_sum(std::ranges::cbegin(r), std::ranges::cend(r), o,
                                std::forward<decltype(args)>(args)...);
    }
} partial_sum{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              /* std::output_iterator */ auto o,
                              auto&&... args) const -> decltype(auto) {
        return std::inclusive_scan(std::ranges::cbegin(r), std::ranges::cend(r),
                                   o, std::forward<decltype(args)>(args)...);
    }

    auto operator()(auto&& policy, std::ranges::forward_range auto&& r,
                    std::forward_iterator auto o,
                    auto&&... args) const -> decltype(auto) {
        return std::inclusive_scan(std::forward<decltype(policy)>(policy),
                                   std::ranges::cbegin(r), std::ranges::cend(r),
                                   o, std::forward<decltype(args)>(args)...);
    }
} inclusive_scan{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              /* std::output_iterator */ auto o,
                              auto&&... args) const -> decltype(auto) {
        return std::exclusive_scan(std::ranges::cbegin(r), std::ranges::cend(r),
                                   o, std::forward<decltype(args)>(args)...);
    }

    auto operator()(auto&& policy, std::ranges::forward_range auto&& r,
                    std::forward_iterator auto o,
                    auto&&... args) const -> decltype(auto) {
        return std::exclusive_scan(std::forward<decltype(policy)>(policy),
                                   std::ranges::cbegin(r), std::ranges::cend(r),
                                   o, std::forward<decltype(args)>(args)...);
    }
} exclusive_scan{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              /* std::output_iterator */ auto o,
                              auto&&... args) const -> decltype(auto) {
        return std::transform_inclusive_scan(
            std::ranges::cbegin(r), std::ranges::cend(r), o,
            std::forward<decltype(args)>(args)...);
    }

    auto operator()(auto&& policy, std::ranges::forward_range auto&& r,
                    std::forward_iterator auto o,
                    auto&&... args) const -> decltype(auto) {
        return std::transform_inclusive_scan(
            std::forward<decltype(policy)>(policy), std::ranges::cbegin(r),
            std::ranges::cend(r), o, std::forward<decltype(args)>(args)...);
    }
} transform_inclusive_scan{};

inline constexpr struct {
    constexpr auto operator()(std::ranges::input_range auto&& r,
                              /* std::output_iterator */ auto o,
                              auto&&... args) const -> decltype(auto) {
        return std::transform_exclusive_scan(
            std::ranges::cbegin(r), std::ranges::cend(r), o,
            std::forward<decltype(args)>(args)...);
    }

    auto operator()(auto&& policy, std::ranges::forward_range auto&& r,
                    std::forward_iterator auto o,
                    auto&&... args) const -> decltype(auto) {
        return std::transform_exclusive_scan(
            std::forward<decltype(policy)>(policy), std::ranges::cbegin(r),
            std::ranges::cend(r), o, std::forward<decltype(args)>(args)...);
    }
} transform_exclusive_scan{};

} // namespace muc::ranges
