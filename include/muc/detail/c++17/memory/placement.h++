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

#include "muc/detail/c++17/memory/construct_at.h++"

#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>

namespace muc {

template<typename T>
struct placement {
public:
    placement(const T& obj) :
        m_storage{} {
        muc::construct_at(pointer_to(m_storage), obj);
    }

    placement(T&& obj) :
        m_storage{} {
        muc::construct_at(pointer_to(m_storage), std::move(obj));
    }

    template<typename... Args>
    placement(Args&&... args) :
        m_storage{} {
        muc::construct_at(pointer_to(m_storage), std::forward<Args>(args)...);
    }

    placement(const placement& other) :
        placement{*other} {}

    placement(placement&& other) :
        placement{std::move(*other)} {}

    ~placement() {
        std::destroy_at(pointer_to(m_storage));
    }

    auto operator=(const placement& other) {
        alignas(T) std::byte buffer[sizeof(T)]{};
        muc::construct_at(pointer_to(buffer), *other);
        std::destroy_at(pointer_to(m_storage));
        std::memcpy(&m_storage, &buffer, sizeof(T));
    }

    auto operator=(placement&& other) {
        alignas(T) std::byte buffer[sizeof(T)]{};
        muc::construct_at(pointer_to(buffer), std::move(*other));
        std::destroy_at(pointer_to(m_storage));
        std::memcpy(&m_storage, &buffer, sizeof(T));
    }

    operator T&() & {
        return **this;
    }

    operator const T&() const& {
        return **this;
    }

    operator T&&() && {
        return std::move(**this);
    }

    operator const T&&() const&& {
        return std::move(**this);
    }

    auto operator*() & -> T& {
        return get();
    }

    auto operator*() const& -> const T& {
        return get();
    }

    auto operator*() && -> T&& {
        return std::move(get());
    }

    auto operator*() const&& -> const T&& {
        return std::move(get());
    }

    auto get() & -> T& {
        return *pointer_to(m_storage);
    }

    auto get() const& -> const T& {
        return *pointer_to(m_storage);
    }

    auto get() && -> T&& {
        return std::move(*pointer_to(m_storage));
    }

    auto get() const&& -> const T&& {
        return std::move(*pointer_to(m_storage));
    }

    auto operator->() -> T* {
        return pointer_to(m_storage);
    }

    auto operator->() const -> const T* {
        return pointer_to(m_storage);
    }

private:
    static auto pointer_to(std::byte* buffer) -> T* {
        return std::launder(reinterpret_cast<T*>(buffer));
    }

    static auto pointer_to(const std::byte* buffer) -> const T* {
        return std::launder(reinterpret_cast<const T*>(buffer));
    }

private:
    alignas(T) std::byte m_storage[sizeof(T)];
};

} // namespace muc
