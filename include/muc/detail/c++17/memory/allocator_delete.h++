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

#include "muc/detail/c++17/memory/to_address.h++"

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace muc {

template<typename T, typename Alloc>
struct allocator_delete : private std::remove_cv_t<Alloc> {
    using allocator_type = std::remove_cv_t<Alloc>;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;

    template<std::enable_if_t<
                 std::allocator_traits<allocator_type>::is_always_equal::value,
                 bool> = true>
    allocator_delete() :
        allocator_type{} {}

    template<typename OtherAlloc>
    allocator_delete(OtherAlloc&& other) :
        allocator_type{std::forward<OtherAlloc>(other)} {}

    auto operator()(pointer p) -> void {
        auto& alloc{get_allocator()};
        using traits = std::allocator_traits<allocator_type>;
        traits::destroy(alloc, muc::to_address(p));
        traits::deallocate(alloc, p, 1);
    }

    auto get_allocator() -> allocator_type& {
        return *this;
    }

    auto get_allocator() const -> const allocator_type& {
        return *this;
    }
};

template<typename T, typename Alloc>
struct allocator_delete<T, Alloc&> : private std::reference_wrapper<Alloc> {
    using allocator_type = std::remove_cv_t<Alloc>;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;

    allocator_delete(std::reference_wrapper<Alloc> alloc) :
        std::reference_wrapper<Alloc>{alloc} {}

    auto operator()(pointer p) -> void {
        auto& alloc{get_allocator()};
        using traits = std::allocator_traits<allocator_type>;
        traits::destroy(alloc, muc::to_address(p));
        traits::deallocate(alloc, p, 1);
    }

    auto get_allocator() const -> Alloc& {
        return std::reference_wrapper<Alloc>::get();
    }
};

template<typename T, typename Alloc>
struct allocator_delete<T[], Alloc> {};

} // namespace muc
