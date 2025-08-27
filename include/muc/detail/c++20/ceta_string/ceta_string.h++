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

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>

namespace muc {

/// @brief ceta_string for 'consteval, template applicable string'
/// @tparam N size of the string (include end '\0')
/// @note Should always be constructed from string literals or nullptr.
template<std::size_t N>
struct ceta_string {
private:
    using cstring = const char[N];

public:
    consteval ceta_string(const cstring& string) :
        m_private_string_data{string} {}

    constexpr auto c_str() const -> const auto& {
        return m_private_string_data.m_data;
    }

    constexpr auto sv() const -> std::string_view {
        return {c_str(), N - 1};
    }

    auto s() const -> std::string {
        return {c_str(), N - 1};
    }

    constexpr operator const cstring&() const {
        return c_str();
    }

    constexpr operator std::string_view() const {
        return sv();
    }

    explicit operator std::string() const {
        return s();
    }

    constexpr auto operator==(const char* rhs) const -> bool {
        return sv() == std::string_view{rhs};
    }

    constexpr auto operator<=>(const char* rhs) const -> auto {
        return sv() <=> std::string_view{rhs};
    }

    constexpr auto operator==(std::nullptr_t) const -> bool {
        return not has_value();
    }

    constexpr operator bool() const {
        return has_value();
    }

    static constexpr auto has_value() -> bool {
        return true;
    }

private:
    struct private_string_data {
        consteval private_string_data(const cstring& string) :
            m_data{} {
            std::ranges::copy(string, m_data);
        }

        char m_data[N];
    };

public:
    const private_string_data m_private_string_data; // private by semantic
};

template<std::size_t N>
ceta_string(const char (&)[N]) -> ceta_string<N>;

template<>
struct ceta_string<0> {
    consteval ceta_string(std::nullptr_t) {}

    constexpr auto operator=(const ceta_string&) -> ceta_string& = delete;

    constexpr operator bool() const {
        return has_value();
    }

    constexpr auto operator==(std::nullptr_t) const -> bool {
        return not has_value();
    }

    static constexpr auto has_value() -> bool {
        return false;
    }
};

ceta_string() -> ceta_string<0>;

ceta_string(std::nullptr_t) -> ceta_string<0>;

template<std::size_t M, std::size_t N>
constexpr auto operator==(const ceta_string<M>& lhs,
                          const ceta_string<N>& rhs) -> bool {
    return lhs.sv() == rhs.sv();
}

template<std::size_t M, std::size_t N>
constexpr auto operator<=>(const ceta_string<M>& lhs,
                           const ceta_string<N>& rhs) -> auto {
    return lhs.sv() <=> rhs.sv();
}

} // namespace muc
