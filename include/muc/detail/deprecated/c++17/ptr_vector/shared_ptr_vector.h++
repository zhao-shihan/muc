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

#include "muc/detail/deprecated/c++17/ptr_vector/impl/ptr_vector_base.h++"

#include <initializer_list>
#include <iterator>
#include <memory>
#include <memory_resource>
#include <type_traits>
#include <utility>
#include <vector>

namespace muc {

template<typename T, typename Allocator = typename std::allocator<T>>
class shared_ptr_vector :
    public impl::ptr_vector_base<
        shared_ptr_vector<T, Allocator>,
        std::vector<std::shared_ptr<T>,
                    typename std::allocator_traits<Allocator>::
                        template rebind_alloc<std::shared_ptr<T>>>> {
    static_assert(not std::is_reference_v<T>,
                  "value type of muc::shared_ptr_vector cannot be a reference");
    static_assert(not std::is_array_v<T>,
                  "value type of muc::shared_ptr_vector cannot be an array");
    static_assert(std::is_same_v<typename Allocator::value_type, T>,
                  "muc::shared_ptr_vector must have the same value_type as its "
                  "allocator");

private:
    using base = impl::ptr_vector_base<
        shared_ptr_vector<T, Allocator>,
        std::vector<std::shared_ptr<T>,
                    typename std::allocator_traits<
                        Allocator>::template rebind_alloc<std::shared_ptr<T>>>>;

    friend base; // for access allocate_ptr

public:
    shared_ptr_vector() noexcept(noexcept(Allocator{})) = default;

    explicit shared_ptr_vector(const Allocator& alloc) noexcept :
        base{typename base::raw_ptr_vector::allocator_type{alloc}} {}
#if false
    shared_ptr_vector(size_type count, const T& value,
                      const Allocator& alloc = {}) :
        base{count, alloc} {
        for (auto&& ptr : this->m_ptr_vector) {
            ptr = allocate_ptr(value);
        }
    }

    shared_ptr_vector(size_type count, const Allocator& alloc = {}) :
        base{count, alloc} {
        for (auto&& ptr : this->m_ptr_vector) {
            ptr = allocate_ptr();
        }
    }

    template<typename InputIt>
    shared_ptr_vector(InputIt first, InputIt last,
                      const Allocator& alloc = {}) :
        base{alloc} {
        this->insert(this->cend(), first, last);
    }
#endif
    shared_ptr_vector(const shared_ptr_vector&) = default;

    shared_ptr_vector(shared_ptr_vector&&) noexcept = default;

    shared_ptr_vector(const shared_ptr_vector& other, const Allocator& alloc) :
        base{other.m_ptr_vector,
             typename base::raw_ptr_vector::allocator_type{alloc}} {}

    shared_ptr_vector(shared_ptr_vector&& other, const Allocator& alloc) :
        base{std::move(other.m_ptr_vector),
             typename base::raw_ptr_vector::allocator_type{alloc}} {}

    shared_ptr_vector(const typename base::raw_ptr_vector& ptr_vector,
                      const Allocator& alloc = {}) :
        base{ptr_vector, typename base::raw_ptr_vector::allocator_type{alloc}} {
    }

    shared_ptr_vector(typename base::raw_ptr_vector&& ptr_vector,
                      const Allocator& alloc = {}) :
        base{std::move(ptr_vector),
             typename base::raw_ptr_vector::allocator_type{alloc}} {}

    template<
        typename C,
        std::enable_if_t<std::is_lvalue_reference_v<const C&>, bool> = true,
        typename = std::void_t<decltype(std::begin(std::declval<const C&>()))>,
        typename = std::void_t<decltype(std::end(std::declval<const C&>()))>,
        std::enable_if_t<
            std::is_constructible_v<typename base::raw_ptr_vector::value_type,
                                    decltype(*std::begin(std::declval<C>())++)>,
            bool> = true>
    shared_ptr_vector(const C& ptr_vector, const Allocator& alloc = {}) :
        shared_ptr_vector{alloc} {
        auto first{std::begin(ptr_vector)};
        const auto last{std::end(ptr_vector)};
        this->reserve(std::distance(first, last));
        while (first != last) {
            this->m_ptr_vector.push_back(
                typename base::raw_ptr_vector::value_type{*first++});
        }
    }

    template<typename C,
             std::enable_if_t<std::is_rvalue_reference_v<C&&>, bool> = true,
             typename = std::void_t<decltype(std::begin(std::declval<C>()))>,
             typename = std::void_t<decltype(std::end(std::declval<C>()))>,
             std::enable_if_t<
                 std::is_constructible_v<
                     typename base::raw_ptr_vector::value_type,
                     decltype(std::move(*std::begin(std::declval<C>())++))>,
                 bool> = true>
    shared_ptr_vector(C&& ptr_vector, const Allocator& alloc = {}) :
        shared_ptr_vector{alloc} {
        auto first{std::begin(ptr_vector)};
        const auto last{std::end(ptr_vector)};
        this->reserve(std::distance(first, last));
        while (first != last) {
            this->m_ptr_vector.push_back(
                typename base::raw_ptr_vector::value_type{std::move(*first++)});
        }
    }
#if false
    shared_ptr_vector(std::initializer_list<T> init,
                      const Allocator& alloc = {}) :
        base{alloc} {
        this->insert(this->cend(), std::move(init));
    }
#endif
private:
    template<typename... Args>
    auto allocate_ptr(Args&&... args) const -> std::shared_ptr<T> {
        return std::allocate_shared<T>(this->get_allocator(),
                                       std::forward<Args>(args)...);
    }
};

template<typename T, typename Allocator>
auto swap(shared_ptr_vector<T, Allocator>& lhs,
          shared_ptr_vector<T, Allocator>&
              rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
    lhs.swap(rhs);
}

namespace pmr {

template<typename T>
using shared_ptr_vector =
    ::muc::shared_ptr_vector<T, std::pmr::polymorphic_allocator<T>>;

} // namespace pmr

} // namespace muc
