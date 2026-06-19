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

#include "muc/detail/c++17/memory/allocator_delete.h++"
#include "muc/detail/c++17/type_traits/type_identity.h++"

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace muc {

/// @brief Determines the return type of allocate_unique.
///
/// When Alloc is a reference type, the unique_ptr uses
/// muc::allocator_delete<T, Alloc> directly as the deleter. Otherwise, the
/// allocator is rebound to T via allocator_traits::rebind_alloc before being
/// used in the deleter.
///
/// @tparam T The type of the object to manage
/// @tparam Alloc The allocator type (may be a reference)
template<typename T, typename Alloc>
using unique_alloc_ptr = std::conditional_t<
    std::is_reference_v<Alloc>,
    muc::type_identity_t<std::unique_ptr<T, muc::allocator_delete<T, Alloc>>>,
    muc::type_identity_t<std::unique_ptr<
        T, muc::allocator_delete<T, typename std::allocator_traits<
                                        Alloc>::template rebind_alloc<T>>>>>;

/// @brief Allocates and constructs an object of type T using an allocator,
/// returning it in a std::unique_ptr.
///
/// This overload accepts an rvalue or lvalue reference to an allocator. The
/// allocator is copied (or moved) into the returned unique_ptr's deleter. For
/// stateful allocators that should not be copied, use the
/// std::reference_wrapper overload instead.
///
/// @tparam T The type of object to allocate and construct
/// @tparam Alloc The allocator type
/// @tparam Args The types of arguments to forward to T's constructor
/// @param alloc The allocator to use for allocation and construction
/// @param args Arguments to forward to T's constructor
///
/// @return A std::unique_ptr<T, Deleter> managing the newly created object.
/// The deleter type is determined by muc::unique_alloc_ptr<T, Alloc>
///
/// @see muc::unique_alloc_ptr
template<typename T, typename Alloc, typename... Args>
auto allocate_unique(Alloc&& alloc, Args&&... args)
    -> muc::unique_alloc_ptr<T, Alloc> {
    using traits =
        typename std::allocator_traits<Alloc>::template rebind_traits<T>;
    using alloc_t = typename traits::allocator_type;
    alloc_t my_alloc{std::forward<Alloc>(alloc)};
    auto hold_dealloc{[&my_alloc](auto p) {
        traits::deallocate(my_alloc, p, 1);
    }};
    using hold_t = std::unique_ptr<T, decltype(hold_dealloc)>;
    hold_t hold{traits::allocate(my_alloc, 1), std::move(hold_dealloc)};
    traits::construct(my_alloc, hold.get(), std::forward<Args>(args)...);
    return {hold.release(), muc::allocator_delete<T, alloc_t>{my_alloc}};
}

/// @brief Allocates and constructs an object of type T using an allocator
/// referred to by reference_wrapper, returning it in a std::unique_ptr.
///
/// This overload is selected when the caller wraps the allocator in
/// std::ref() or std::cref(). Unlike the primary overload, this version
/// stores a reference to the original allocator rather than copying it,
/// which is essential for stateful allocators.
///
/// @tparam T The type of object to allocate and construct
/// @tparam Alloc The allocator type
/// @tparam Args The types of arguments to forward to T's constructor
/// @param alloc A reference_wrapper referring to the allocator
/// @param args Arguments to forward to T's constructor
///
/// @return A std::unique_ptr<T, Deleter> managing the newly created object.
/// The deleter stores a reference to the original allocator
///
/// @note Use std::ref(alloc) to invoke this overload
template<typename T, typename Alloc, typename... Args>
auto allocate_unique(std::reference_wrapper<Alloc> alloc, Args&&... args)
    -> muc::unique_alloc_ptr<T, Alloc&> {
    using traits = std::allocator_traits<Alloc>;
    auto hold_dealloc{[&alloc](auto p) {
        traits::deallocate(alloc.get(), p, 1);
    }};
    using hold_t = std::unique_ptr<T, decltype(hold_dealloc)>;
    hold_t hold{traits::allocate(alloc.get(), 1), std::move(hold_dealloc)};
    traits::construct(alloc.get(), hold.get(), std::forward<Args>(args)...);
    return {hold.release(), muc::allocator_delete<T, Alloc&>{alloc}};
}

} // namespace muc
