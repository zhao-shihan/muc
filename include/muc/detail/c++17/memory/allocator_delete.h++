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

/// @brief A deleter for std::unique_ptr that uses an allocator to destroy and
/// deallocate the managed object.
///
/// This class privately inherits from the allocator (leveraging EBO when the
/// allocator is empty) and provides an operator() suitable as the deleter for
/// std::unique_ptr. When invoked, it destroys the object via
/// allocator_traits::destroy and then deallocates the memory via
/// allocator_traits::deallocate.
///
/// @tparam T The type of the managed object
/// @tparam Alloc The allocator type
///
/// @note A default constructor is provided when
/// std::allocator_traits<allocator_type>::is_always_equal is true.
/// @note The T[] specialization is poisoned.
///
/// @see muc::allocate_unique() for convenient construction of unique_ptr with
/// this deleter
template<typename T, typename Alloc>
struct allocator_delete : private std::remove_cv_t<Alloc> {
    /// @brief The allocator type (with top-level cv-qualifiers removed)
    using allocator_type = std::remove_cv_t<Alloc>;
    /// @brief The pointer type as defined by allocator_traits
    using pointer = typename std::allocator_traits<allocator_type>::pointer;

    /// @brief Default constructor, available only when the allocator is
    /// stateless
    template<std::enable_if_t<
                 std::allocator_traits<allocator_type>::is_always_equal::value,
                 bool> = true>
    allocator_delete() :
        allocator_type{} {}

    /// @brief Constructs the deleter by forwarding an allocator
    /// @tparam OtherAlloc Type of the allocator to forward
    /// @param other The allocator to forward
    template<typename OtherAlloc>
    allocator_delete(OtherAlloc&& other) :
        allocator_type{std::forward<OtherAlloc>(other)} {}

    /// @brief Destroys the managed object and deallocates its memory
    /// @param p Pointer to the managed object
    auto operator()(pointer p) -> void {
        auto& alloc{get_allocator()};
        using traits = std::allocator_traits<allocator_type>;
        traits::destroy(alloc, muc::to_address(p));
        traits::deallocate(alloc, p, 1);
    }

    /// @brief Returns a reference to the stored allocator
    /// @return Mutable reference to the allocator
    auto get_allocator() -> allocator_type& {
        return *this;
    }

    /// @brief Returns a const reference to the stored allocator
    /// @return Const reference to the allocator
    auto get_allocator() const -> const allocator_type& {
        return *this;
    }
};

/// @brief Specialization of allocator_delete for allocator references.
///
/// This specialization stores a reference to an externally-owned allocator
/// without copying it.
///
/// @tparam T The type of the managed object
/// @tparam Alloc The allocator type (referred to by reference)
template<typename T, typename Alloc>
struct allocator_delete<T, Alloc&> : private std::reference_wrapper<Alloc> {
    /// @brief The allocator type (with top-level cv-qualifiers removed)
    using allocator_type = std::remove_cv_t<Alloc>;
    /// @brief The pointer type as defined by allocator_traits
    using pointer = typename std::allocator_traits<allocator_type>::pointer;

    /// @brief Constructs the deleter from a reference_wrapper to an allocator
    /// @param alloc A reference_wrapper referring to the allocator
    allocator_delete(std::reference_wrapper<Alloc> alloc) :
        std::reference_wrapper<Alloc>{alloc} {}

    /// @brief Destroys the managed object and deallocates its memory
    /// @param p Pointer to the managed object
    auto operator()(pointer p) -> void {
        auto& alloc{get_allocator()};
        using traits = std::allocator_traits<allocator_type>;
        traits::destroy(alloc, muc::to_address(p));
        traits::deallocate(alloc, p, 1);
    }

    /// @brief Returns a reference to the stored allocator
    /// @return Reference to the allocator
    auto get_allocator() const -> Alloc& {
        return std::reference_wrapper<Alloc>::get();
    }
};

/// @brief Empty specialization to prevent array usage of allocator_delete
/// @tparam T The array element type
/// @tparam Alloc The allocator type
template<typename T, typename Alloc>
struct allocator_delete<T[], Alloc> {};

} // namespace muc
