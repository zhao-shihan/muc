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

#include "muc/detail/c++17/memory/allocator_deleter.h++"
#include "muc/detail/c++17/type_traits/type_identity.h++"

#include <memory>
#include <type_traits>
#include <utility>

namespace muc {

template<typename T, typename Alloc>
using unique_alloc_ptr = std::conditional_t<
    std::is_reference_v<Alloc>,
    muc::type_identity_t<std::unique_ptr<T, muc::allocator_delete<T, Alloc>>>,
    muc::type_identity_t<std::unique_ptr<
        T, muc::allocator_delete<T, typename std::allocator_traits<
                                        Alloc>::template rebind_alloc<T>>>>>;

template<typename T, typename Alloc, typename... Args>
auto allocate_unique(Alloc&& alloc, Args&&... args) -> auto {
    using traits =
        typename std::allocator_traits<Alloc>::template rebind_traits<T>;
    using alloc_t = typename traits::allocator_type;
    alloc_t my_alloc{std::forward<Alloc>(alloc)};
    auto hold_deleter{[&my_alloc](auto p) {
        traits::deallocate(my_alloc, p, 1);
    }};
    using hold_t = std::unique_ptr<T, decltype(hold_deleter)>;
    hold_t hold{traits::allocate(my_alloc, 1), hold_deleter};
    traits::construct(my_alloc, hold.get(), std::forward<Args>(args)...);
    muc::allocator_delete<T, alloc_t> deleter{my_alloc};
    return std::unique_ptr<T, decltype(deleter)>{hold.release(),
                                                 std::move(deleter)};
}

template<typename T, typename Alloc, typename... Args>
auto allocate_unique(std::reference_wrapper<Alloc> alloc, Args&&... args)
    -> auto {
    using traits = std::allocator_traits<Alloc>;
    auto hold_deleter{[&alloc](auto p) {
        traits::deallocate(alloc.get(), p, 1);
    }};
    using hold_t = std::unique_ptr<T, decltype(hold_deleter)>;
    hold_t hold{traits::allocate(alloc.get(), 1), hold_deleter};
    traits::construct(alloc.get(), hold.get(), std::forward<Args>(args)...);
    muc::allocator_delete<T, Alloc&> deleter{alloc};
    return std::unique_ptr<T, decltype(deleter)>{hold.release(),
                                                 std::move(deleter)};
}

} // namespace muc
